#include "pch.h"
#include "jingtumlib.h"

std::string OfferTypes[2] = { "Sell", "Buy" };
std::string RelationTypes[4] = { "trust", "authorize", "freeze", "unfreeze" };
std::string AccountSetTypes[3] = { "property", "delegate", "signer" };

jingtumlib::Remote::Remote(std::string serverInfo) {

	std::stringstream str(serverInfo);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str, pt);		
	}
	catch (boost::property_tree::ptree_error ex) {
		std::cout << ex.what() + std::string("Server Information Parse Error!") <<std::endl;
		exit(0);
	}
	
	_url = pt.get<std::string>("server");
	_localsign = pt.get<bool>("local_sign");
	parseUrl(_url);

	if (protocol != "wss" && protocol != "ws") {
		std::cout << "Please Check the Protocol!" << std::endl;
		exit(0);
	}

	_status.put("ledger_index", 0);
	_cache = new LRUCache(100);
	_paths = new LRUCache(2100); 
};
	
jingtumlib::Remote::~Remote() {
	if (server) {
		delete server;
	}
	delete _cache;
	delete _paths;
}

void jingtumlib::Remote::connect(void (* callback)(std::string, std::string, Remote *)) {
	if (server == NULL) {
		try {
			server = new Server(protocol, domain, port);
		}
		catch (std::exception ex) {
			std::cout << "Server not ready!" << std::endl;
			exit(0);
		}

		server->connect(this, callback);
	}
	else {
		std::cout << "Connection has established!" << std::endl;
	}	
}

void jingtumlib::Remote::disConnect() {
	if (server == NULL) return;
	server->disconnect();
}

void jingtumlib::Remote::_submit(std::string command, std::string data, para pa, std::string(*filter)(std::string, para)) {
	try {
		int id = server->sendMessage(command, data);
		std::cout << "Request Serial Number:" << id << std::endl;
		_handleMessage(server->bmsg[id].backmsg);
		if (server->bmsg[id].datastatus == "success") {
			if (filter != NULL) {
				server->bmsg[id].dataresult = filter(server->bmsg[id].dataresult, pa);
			}
		}
	}
	catch (std::exception ex) {
		std::cout << "something wrong" << std::endl;
		std::cout << ex.what() << "Backend Message Error!" << std::endl;
		exit(0);
	}
}

void jingtumlib::Remote::_submit(std::string command, std::string data, para pa, std::string (* filter)(std::string, para), void(* callback)(std::string, std::string)) {
	try {
		int id = server->sendMessage(command, data);
		std::cout << "Request Serial Number:" << id << std::endl;
		_handleMessage(server->bmsg[id].backmsg);
		if (server->bmsg[id].datastatus == "success") {
			if (filter != NULL) {	
				server->bmsg[id].dataresult = filter(server->bmsg[id].dataresult, pa);
			}
		}				

		if (callback != NULL) {
			callback("", server->bmsg[id].dataresult);
		}
	}
	catch (std::exception ex) {
		std::cout << "something wrong" << std::endl;
		std::cout << ex.what() << "Backend Message Error!" << std::endl;
		callback(ex.what(), "");
		exit(0);
	}
}

void jingtumlib::Remote::_submit(std::string command, std::string data, para pa, std::string(*filter)(std::string, para), void(*callback)(std::string, std::string, int)) {
	try {
		int id = server->sendMessage(command, data);
		std::cout << "Request Serial Number:" << id << std::endl;
		std::cout << "remote submit3:" << std::endl;
		_handleMessage(server->bmsg[id].backmsg);
		std::cout << "step 1 finish:" << std::endl;
		if (server->bmsg[id].datastatus == "success") {
			if (filter != NULL) {
				server->bmsg[id].dataresult = filter(server->bmsg[id].dataresult, pa);
			}
		}
		std::cout << "step 2 finish:" << std::endl;
		if (callback != NULL) {
			callback("", server->bmsg[id].dataresult, pa.addr);
		}
		std::cout << "step 3 finish:" << std::endl;
	}
	catch (std::exception ex) {
		std::cout << "something wrong" << std::endl;
		std::cout << ex.what() << "Backend Message Error!" << std::endl;
		callback(ex.what(), "", pa.addr);
		exit(0);
	}
}

jingtumlib::Request jingtumlib::Remote::subscribe(std::string text) {
	Request req(this, "subscribe", NULL);
	req._message = "\"streams\":" + text;
	return req;
}

jingtumlib::Request jingtumlib::Remote::requestServerInfo() {
	Request req(this, "server_info", &callback_requestServerInfo);
	req._message = "";
	return req;
}

jingtumlib::Request jingtumlib::Remote::requestLedgerClosed() {
	Request req(this, "ledger_closed", &callback_requestLedgerClosed);
	req._message = "";
	return req;
}

jingtumlib::Request jingtumlib::Remote::requestLedger(std::string option) {	
	Request req(this, "ledger", &callback_requestLedger);
	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		req._message = req._message + "\"type\":\"Exception21:invalid options type!\"";
		return req;
	}

	req.pa.fg = true;
	if (option == "{}") {
		req._message = "";
	}
	else {
		bool sf;
		int index;
		std::string st;
		int flag = 0;
		try {
			index = pt.get<int>("ledger_index");
			flag = 1;
		}
		catch (boost::property_tree::ptree_error ex) { flag = 0; }
		if ( flag == 1 ) req._message = req._message + ",\"ledger_index\":" + std::to_string(index);

		try {
			st = pt.get<std::string>("ledger_hash");
			flag = 1;
		}
		catch (boost::property_tree::ptree_error ex) { flag = 0; }
		
		if (flag == 1) { 
			if (isValidHash(st)) {
				req._message = req._message + ",\"ledger_hash\":\"" + st + "\"";
			}			
		}
		
		try {
			sf = pt.get<bool>("full");
			flag = 1;
		}
		catch (boost::property_tree::ptree_error ex) { flag = 0; }
		if (flag == 1) {
			req._message = req._message + ",\"full\":" + (sf ? "true" : "false");
			req.pa.fg = false;
		}

		try {
			sf = pt.get<bool>("expand");
			flag = 1;
		}
		catch (boost::property_tree::ptree_error ex) { flag = 0; }
		if (flag == 1) {
			req._message = req._message + ",\"expand\":" + (sf ? "true" : "false");
			req.pa.fg = false;
		}

		try {
			sf = pt.get<bool>("transactions");
			flag = 1;
		}
		catch (boost::property_tree::ptree_error ex) { flag = 0; }
		if (flag == 1) {
			req._message = req._message + ",\"transactions\":" + (sf ? "true" : "false");
			req.pa.fg = false;
		}

		try {
			sf = pt.get<bool>("accounts");
			flag = 1;
		}
		catch (boost::property_tree::ptree_error ex) { flag = 0; }
		if (flag == 1) {
			req._message = req._message + ",\"accounts\":" + (sf ? "true" : "false");
			req.pa.fg = false;
		}
	}
		
	return req;
}

jingtumlib::Request jingtumlib::Remote::requestTx(std::string option) {
	Request req(this, "tx", NULL);
	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		req._message = req._message + "\"type\":\"Exception21:invalid options type!\"";
		return req;
	}

	int flag = 0;
	std::string st;
	try {
		st = pt.get<std::string>("hash");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		if (!isValidHash(st)) {
			req._message = req._message + ",\"hash\":\"Exception16:invalid tx hash!\"";
			return req;
		}
		else {
			req._message = req._message + ",\"transaction\":\"" + st+"\"";
		}
	}
	return req;
}

void jingtumlib::Remote::_requestAccount(std::string option, Request *req) {
	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;	
	boost::property_tree::read_json(str_stream, pt);

	int flag = 0;
	std::string st;
	try {
		st = pt.get<std::string>("type");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {	
		int tp = getRelationType(st);
		if (tp != -1) {
			req->_message = req->_message + ",\"relation_type\":" + std::to_string(getRelationType(st));
		}
	}

	try {
		st = pt.get<std::string>("account");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		if (isValidAddress(st)) {
			req->_message = req->_message + ",\"account\":\"" + st + "\"";
		}
		else {
			req->_message = req->_message + ",\"account\":\"Exception16:invalid account!\"";
			return;
		}
	}

	req->selectLedger(pt);
	
	try {
		st = pt.get<std::string>("peer");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		if (isValidAddress(st)) {
			req->_message = req->_message + ",\"peer\":\"" + st + "\"";
		}
		else {
			std::cout << "invalid address" << std::endl;
		}
	}

	int limit;
	try {
		limit = pt.get<int>("limit");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		if (limit < 0) limit = 0;
		if (limit > 1e9) limit = (int)1e9;
		req->_message = req->_message + ",\"limit\":" + std::to_string(limit);
	}

	try {
		st = pt.get<std::string>("market");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		req->_message = req->_message + ",\"market\":\"" + st + "\"";
	}

}

jingtumlib::Request jingtumlib::Remote::requestAccountInfo(std::string option) {
	Request req(this, "account_info", NULL);

	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		req._message = req._message + "\"type\":\"Exception21:invalid options type!\"";
		return req;
	}

	_requestAccount(option, &req);
	return req;
}

jingtumlib::Request jingtumlib::Remote::requestAccountTums(std::string option) {
	Request req(this, "account_currencies", NULL);

	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		req._message = req._message + "\"type\":\"Exception21:invalid options type!\"";
		return req;
	}

	_requestAccount(option, &req);
	return req;
}

jingtumlib::Request jingtumlib::Remote::requestAccountRelations(std::string option) {
	
	Request req(this);

	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		req._message = req._message + ",\"type\":\"Exception21:invalid options type!\"";
		return req;
	}

	std::string st;
	int flag;
	try {
		st = pt.get<std::string>("type");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	int index;
	std::cout << "st:" << st << std::endl;
	for (unsigned int i = 0; i < 4; ++i)
		if (st == RelationTypes[i]) index = i;
	if ( !~index )
	{
		req._message = req._message + ",\"relation_type\":\"Exception22:invalid relation type!\"";
		return req;
	}

	if (st == "trust") {
		req._command = "account_lines";
	}
	else if (st == "authorize" || st == "freeze") 
	{
		req._command = "account_relation";
	}

	_requestAccount(option, &req);
	
	return req;
}

jingtumlib::Request jingtumlib::Remote::requestAccountOffers(std::string option) {
	Request req(this, "account_offers", NULL);

	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		req._message = req._message + "\"type\":\"Exception21:invalid options type!\"";
		return req;
	}

	_requestAccount(option, &req);
	return req;
}

jingtumlib::Request jingtumlib::Remote::requestAccountTx(std::string option) {
	Request req(this, "account_tx", &callback_requestAccountTx);	

	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		req._message = req._message + "\"type\":\"Exception21:invalid options type!\"";
		return req;
	}

	std::string st;
	int flag;
	int k;
	try {
		st = pt.get<std::string>("account");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		if (isValidAddress(st)) {
			req._message = req._message + ",\"account\":\"" + st + "\"";
			req.pa.str = st;
		}
		else {
			req._message = req._message + ",\"account\":\"Exception16:invalid account!\"";
			return req;
		}
	}

	try {
		k = pt.get<int>("ledger_min");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }	
	if (flag == 1) {
		req._message = req._message + ",\"ledger_index_min\":" + std::to_string(k);
	}
	else {
		req._message = req._message + ",\"ledger_index_min\":0";
	}

	try {
		k = pt.get<int>("ledger_max");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		req._message = req._message + ",\"ledger_index_max\":" + std::to_string(k);
	}
	else {
		req._message = req._message + ",\"ledger_index_max\":-1";
	}

	try {
		k = pt.get<int>("limit");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		req._message = req._message + ",\"limit\":" + std::to_string(k);
	}

	try {
		k = pt.get<int>("offset");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		req._message = req._message + ",\"offset\":" + std::to_string(k);
	}

	boost::property_tree::ptree market;
	try {
		market = pt.get_child("market");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) {	flag = 0; }
	if (flag == 1) {
		try {
			k = market.get<int>("ledger");
			flag = 2;
		}
		catch (boost::property_tree::ptree_error ex) { flag = 0; }
		if (flag == 2 && k != NAN) {
			try {
				k = market.get<int>("seq");
				flag = 3;
			}
			catch (boost::property_tree::ptree_error ex) { flag = 0; }
			if (flag == 3 && k != NAN) {
				std::stringstream tmp;
				boost::property_tree::write_json(tmp, market);
				req._message = req._message + "\"market\":\"" + getPureStr(tmp.str())+"\"";
			}
		}
	}	
		
	bool forward;
	try {
		forward = pt.get<bool>("forward");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		req._message = req._message + ",\"forward\":" + std::to_string(forward);
	}
	
	return req;
}

jingtumlib::Request jingtumlib::Remote::requestOrderBook(std::string option) {
	Request req(this, "book_offers", NULL);
	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		req._message = req._message + "\"type\":\"Exception21:invalid options type!\"";
		return req;
	}

	boost::property_tree::ptree taker_gets, taker_pays;
	try {
		taker_gets = pt.get_child("taker_gets");
	}
	catch (boost::property_tree::ptree_error ex) {
		taker_gets = pt.get_child("pays");
	}
	if (!isValidAmount0(taker_gets)) {
		req._message = req._message + ",\"type\":\"Exception26:invalid taker gets amount!\"";
		return req;
	}

	try {
		taker_pays = pt.get_child("taker_pays");
	}
	catch (boost::property_tree::ptree_error ex) {
		taker_pays = pt.get_child("gets");
	}
	if (!isValidAmount0(taker_pays)) {
		req._message = req._message + ",\"type\":\"Exception26:invalid taker gets amount!\"";
		return req;
	}

	std::stringstream ss1, ss2;
	boost::property_tree::write_json(ss1, taker_gets);
	req._message = req._message + ",\"taker_gets\":" + ss1.str();
	boost::property_tree::write_json(ss2, taker_pays);
	req._message = req._message + ",\"taker_pays\":" + ss2.str();
	std::string taker;
	try {
		taker = pt.get<std::string>("taker");
	}
	catch (boost::property_tree::ptree_error ex) {
		taker = ACCOUNT_ONE;
	}
	req._message = req._message + ",\"taker\":\"" + taker + "\"";	

	int limit;
	try {
		limit = pt.get<int>("limit");
		req._message = req._message + ",\"limit\":\"" + std::to_string(limit) + "\"";
	}
	catch (boost::property_tree::ptree_error ex) {}

	return req;
}

jingtumlib::Transaction jingtumlib::Remote::buildPaymentTx(std::string option) {
	Transaction tx(this);
	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		std::cout << ex.what() << "invalid options type!" << std::endl;
		tx.tx_json.put("obj", "Exception21:invalid options type!");
		return tx;
	}

	std::string src, dst;
	boost::property_tree::ptree amount;
	try {
		src = pt.get<std::string>("source");
	}
	catch (boost::property_tree::ptree_error ex) { }
	try {
		src = pt.get<std::string>("from");
	}
	catch (boost::property_tree::ptree_error ex) { }
	try {
		src = pt.get<std::string>("account");
	}
	catch (boost::property_tree::ptree_error ex) { }
	if (!isValidAddress(src)) {
		tx.tx_json.put("src", "Exception23:invalid source address!");
		return tx;
	}

	try {
		dst = pt.get<std::string>("destination");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		dst = pt.get<std::string>("to");
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (!isValidAddress(dst)) {
		tx.tx_json.put("dst", "Exception28:invalid destination address!");
		return tx;
	}

	amount = pt.get_child("amount");
	if (!isValidAmount(amount)){
		tx.tx_json.put("amount", "Exception15:invalid amount!");
		return tx;
	}

	tx.tx_json.put("TransactionType", "Payment");
	tx.tx_json.put("Account", src);
	tx.tx_json.put_child("Amount", ToAmount(amount));
	tx.tx_json.put("Destination", dst);

	return tx;
}

jingtumlib::Transaction jingtumlib::Remote::buildRelationTx(std::string option) {
	Transaction tx(this);
	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		std::cout << ex.what() << "invalid options type!" << std::endl;
		tx.tx_json.put("obj", "Exception21:invalid options type!");
		return tx;
	}

	std::string type = pt.get<std::string>("type");
	int nt = sizeof(RelationTypes) / sizeof(RelationTypes[0]);
	int index = -1;
	for (int i = 0; i < nt; ++i) {
		if (type == RelationTypes[i]) {
			index = i;
			break;
		}
	}
	if (index == -1) {
		tx.tx_json.put("type", "Exception21:invalid relation type");
		return tx;
	}

	if (type == "trust") {
		_buildTrustSet(pt, &tx);
		return tx;
	}
	else if (type == "authorize" || type == "freeze" || type == "unfreeze") {
		_buildRelationSet(pt, &tx);
		return tx;
	}

	tx.tx_json.put("msg", "Exception37:build relation set should not go here");
	return tx;
}

void jingtumlib::Remote::_buildTrustSet(boost::property_tree::ptree option, jingtumlib::Transaction *tx) {
	std::string src;
	try {
		src = option.get<std::string>("source");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		src = option.get<std::string>("from");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		src = option.get<std::string>("account");
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (!isValidAddress(src)) {
		tx->tx_json.put("src", "Exception23:invalid source address!");
	}	
	tx->tx_json.put("Account", src);

	boost::property_tree::ptree limit;
	int flag;
	try {
		limit = option.get_child("limit");
		flag = 1;
	}
	catch (boost::property_tree::ptree ex) { flag = 0; }
	if (flag == 1) {
		if (!isValidAmount(limit)) {
			tx->tx_json.put("limit", "Exception14:invalid amount");
		}
		else tx->tx_json.put_child("LimitAmount", limit);
	}
	
	std::string quality_out, quality_in;
	flag = 0;
	try {
		quality_out = option.get<std::string>("quality_out");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (flag == 1) tx->tx_json.put("QualityOut", quality_out);

	flag = 0;
	try {
		quality_in = option.get<std::string>("quality_in");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (flag == 1) tx->tx_json.put("QualityIn", quality_in);

	tx->tx_json.put("TransactionType", "TrustSet");
}

void jingtumlib::Remote::_buildRelationSet(boost::property_tree::ptree option, jingtumlib::Transaction *tx) {
	std::string src;
	try {
		src = option.get<std::string>("source");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		src = option.get<std::string>("from");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		src = option.get<std::string>("account");
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (!isValidAddress(src)) {
		tx->tx_json.put("src", "Exception23:invalid source address!");
		return;
	}
	tx->tx_json.put("Account", src);

	std::string des;
	try {
		des = option.get<std::string>("target");
	}
	catch (boost::property_tree::ptree ex) {}
	if (!isValidAddress(src)) {
		tx->tx_json.put("des", "Exception23:invalid source address!");
		return;
	}
	tx->tx_json.put("Target", des);

	boost::property_tree::ptree limit;
	int flag;
	try {
		limit = option.get_child("limit");
		flag = 1;
	}
	catch (boost::property_tree::ptree ex) {}
	if (flag == 1) {
		if (!isValidAmount(limit)) {
			tx->tx_json.put("limit", "Exception14:invalid amount");
		}
		else tx->tx_json.put_child("LimitAmount", limit);
	}
	
	

	std::string type = option.get<std::string>("type");
	tx->tx_json.put("TransactionType", type == "unfreeze" ? "RelationDel" : "RelationSet");
	tx->tx_json.put("RelationType", type == "authorize" ? 1 : 3);
}

jingtumlib::Transaction jingtumlib::Remote::buildAccountSetTx(std::string option) {
	Transaction tx(this);
	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		std::cout << ex.what() << "invalid options type!" << std::endl;
		tx.tx_json.put("obj", "Exception21:invalid options type!");
		return tx;
	}

	std::string type = pt.get<std::string>("type");
	int nt = sizeof(AccountSetTypes) / sizeof(AccountSetTypes[0]);
	int index = -1;
	for (int i = 0; i < nt; ++i) {
		if (type == AccountSetTypes[i]) {
			index = i;
			break;
		}
	}
	if (index == -1) {
		tx.tx_json.put("type", "Exception21:invalid account set type");
		return tx;
	}

	if (type == "property") {
		_buildAccountSet(pt, &tx);
		return tx;
	}
	else if (type == "delegate") {
		_buildDelegateKeySet(pt, &tx);
		return tx;
	}
	else if (type == "signer") {
		_buildSignerSet(pt, &tx);
		return tx;
	}
	else {
		tx.tx_json.put("msg", "Exception37:build relation set should not go here");
	}

	return tx;
}

void jingtumlib::Remote::_buildAccountSet(boost::property_tree::ptree option, jingtumlib::Transaction *tx) {
	std::string src;
	try {
		src = option.get<std::string>("source");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		src = option.get<std::string>("from");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		src = option.get<std::string>("account");
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (!isValidAddress(src)) {
		tx->tx_json.put("src", "Exception23:invalid source address!");
		return;
	}
	tx->tx_json.put("Account", src);
	tx->tx_json.put("TransactionType", "AccountSet");

	int flag;
	std::string set_flag;
	try {
		set_flag = option.get<std::string>("set_flag");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	try {
		set_flag = option.get<std::string>("set");
		flag = 2;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	set_flag = prepareFlag(set_flag);
	if (flag != 0 && set_flag != "") {
		tx->tx_json.put("SetFlag", set_flag);
	}

	flag = 0;
	std::string clear_flag;
	try {
		clear_flag = option.get<std::string>("clear_flag");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		clear_flag = option.get<std::string>("clear");
		flag = 2;
	}
	catch (boost::property_tree::ptree_error ex) {}
	clear_flag = prepareFlag(clear_flag);
	if (flag != 0 && clear_flag != "") {
		tx->tx_json.put("ClearFlag", clear_flag);
	}

}

std::string prepareFlag(std::string flag) {
	boost::property_tree::ptree SetClearFlags = TransactionSetClearFlags().get_child("AccountSet");
	if (isNum(flag)) return flag;
	else if (flag.find("asf") != 0) {
		flag = "asf" + flag;
	}
	int f = 0;
	try {
		flag = SetClearFlags.get<std::string>(flag);
		f = 1;
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (f == 1) return flag;
	else return "";
}

void jingtumlib::Remote::_buildDelegateKeySet(boost::property_tree::ptree option, jingtumlib::Transaction *tx) {
	std::string src;
	try {
		src = option.get<std::string>("source");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		src = option.get<std::string>("from");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		src = option.get<std::string>("account");
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (!isValidAddress(src)) {
		tx->tx_json.put("src", "Exception23:invalid source address!");
		return;
	}
	tx->tx_json.put("Account", src);
	tx->tx_json.put("TransactionType", "SetRegulatKey");

	std::string delegate_key;
	try {
		delegate_key = option.get<std::string>("delegate_key");
	}
	catch(boost::property_tree::ptree_error ex){}
	if (!isValidAddress(delegate_key)) {
		tx->tx_json.put("delegate_key", "Exception23:invalid regular key address!");
		return;
	}
	tx->tx_json.put("RegularKey", delegate_key);
}

void jingtumlib::Remote::_buildSignerSet(boost::property_tree::ptree option, jingtumlib::Transaction *tx) {

}

jingtumlib::Transaction jingtumlib::Remote::buildOfferCreateTx(std::string option) {
	Transaction tx(this);
	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		std::cout << ex.what() << "invalid options type!" << std::endl;
		tx.tx_json.put("obj", "Exception21:invalid options type!");
		return tx;
	}

	std::string src;
	try {
		src = pt.get<std::string>("source");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		src = pt.get<std::string>("from");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		src = pt.get<std::string>("account");
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (!isValidAddress(src)) {
		tx.tx_json.put("src", "Exception23:invalid source address!");
		return tx;
	}
	tx.tx_json.put("Account", src);

	std::string offer_type;
	try {
		offer_type = pt.get<std::string>("type");
	}
	catch(boost::property_tree::ptree_error ex) {}
	int nt = sizeof(OfferTypes) / sizeof(OfferTypes[0]);
	int index = -1;
	for (int i = 0; i < nt; ++i) {
		if (offer_type == OfferTypes[i]) {
			index = i;
			break;
		}
	}
	if (index == -1) {
		tx.tx_json.put("offer_type", "Exception18:invalid offer type");
		return tx;
	}

	boost::property_tree::ptree taker_gets;
	try {
		taker_gets = pt.get_child("taker_gets");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		taker_gets = pt.get_child("pays");
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (taker_gets.size() == 0) {
		if (!isNum(taker_gets.data())) {
			tx.tx_json.put("taker_gets2", "Exception22:invalid to pays amount");
			return tx;
		}
	}
	else {
		if (!isValidAmount(taker_gets)) {
			tx.tx_json.put("taker_gets2", "Exception29:invalid to pays amount object");
			return tx;
		}
	}

	boost::property_tree::ptree taker_pays;
	try {
		taker_pays = pt.get_child("taker_pays");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		taker_pays = pt.get_child("gets");
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (taker_pays.size() == 0) {
		if (!isNum(taker_pays.data())) {
			tx.tx_json.put("taker_pays2", "Exception22:invalid to gets amount");
			return tx;
		}
	}
	else {
		if (!isValidAmount(taker_pays)) {
			tx.tx_json.put("taker_pays2", "Exception29:invalid to gets amount object");
			return tx;
		}
	}

	tx.tx_json.put("TransactionType", "OfferCreate");
	if (offer_type == "Sell") tx.setFlags(offer_type);
	tx.tx_json.put_child("TakerPays", ToAmount(taker_pays));
	tx.tx_json.put_child("TakerGets", ToAmount(taker_gets));
	return tx;
}

jingtumlib::Transaction jingtumlib::Remote::buildOfferCancelTx(std::string option) {
	Transaction tx(this);
	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		std::cout << ex.what() << "invalid options type!" << std::endl;
		tx.tx_json.put("obj", "Exception21:invalid options type!");
		return tx;
	}

	std::string src;
	try {
		src = pt.get<std::string>("source");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		src = pt.get<std::string>("from");
	}
	catch (boost::property_tree::ptree_error ex) {}
	try {
		src = pt.get<std::string>("account");
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (!isValidAddress(src)) {
		tx.tx_json.put("src", "Exception23:invalid source address!");
		return tx;
	}
	tx.tx_json.put("Account", src);

	int sequence;
	try {
		sequence = pt.get<int>("sequence");
	}
	catch (boost::property_tree::ptree ex) { 
		tx.tx_json.put("sequence", "Exception22:invalid sequence param!");
		return tx;
	}

	tx.tx_json.put("OfferSequence", sequence);
	tx.tx_json.put("TransactionType", "OfferCancel");
	return tx;
}

jingtumlib::Transaction jingtumlib::Remote::deployContractTx(std::string option) {
	Transaction tx(this);
	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		std::cout << ex.what() << "invalid options type!" << std::endl;
		tx.tx_json.put("obj", "Exception21:invalid options type!");
		return tx;
	}

	std::string account;
	try {
		account = pt.get<std::string>("account");
	}
	catch(boost::property_tree::ptree ex){}
	if (!isValidAddress(account)) {
		tx.tx_json.put("account", "Exception16:invalid address!");
		return tx;
	}
	tx.tx_json.put("Account", account);

	double amount;
	try {
		amount = pt.get<double>("amount");
	}
	catch (boost::property_tree::ptree ex) { 
		tx.tx_json.put("amount", "Exception15:invalid amount!");
		return tx;
	}
	tx.tx_json.put("Amount", amount*1000000);

	std::string payload;
	try {
		payload = pt.get<std::string>("payload");
	}
	catch (boost::property_tree::ptree ex) {
		tx.tx_json.put("payload", "Exception28:invalid payload: type error!");
		return tx;
	}
	payload = processContract(payload);
	tx.tx_json.put("Payload", payload);

	boost::property_tree::ptree params;
	try {
		params = pt.get_child("params");
	}
	catch (boost::property_tree::ptree ex) {
		tx.tx_json.put("params", "Exception21:invalid options type!");
		return tx;
	}
	
	boost::property_tree::ptree c;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, params) {
		boost::property_tree::ptree a;
		boost::property_tree::ptree b;
		a.put("Parameter", StringToHex(v.second.data()));
		b.put_child("Arg", a);
		c.push_back(std::make_pair("", b));
	}
	tx.tx_json.put_child("Args", c);

	tx.tx_json.put("TransactionType", "ConfigContract");
	tx.tx_json.put("Method", 0);
	return tx;
}

jingtumlib::Transaction jingtumlib::Remote::callContractTx(std::string option) {
	Transaction tx(this);
	std::stringstream str_stream(option);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (boost::property_tree::ptree_error ex) {
		std::cout << ex.what() << "invalid options type!" << std::endl;
		tx.tx_json.put("obj", "Exception21:invalid options type!");
		return tx;
	}

	std::string account;
	try {
		account = pt.get<std::string>("account");
	}
	catch (boost::property_tree::ptree ex) {}
	if (!isValidAddress(account)) {
		tx.tx_json.put("account", "Exception16:invalid address!");
		return tx;
	}
	tx.tx_json.put("Account", account);

	std::string des;
	try {
		des = pt.get<std::string>("destination");
	}
	catch (boost::property_tree::ptree ex) {}
	if (!isValidAddress(des)) {
		tx.tx_json.put("des", "Exception20:invalid destination!");
		return tx;
	}
	tx.tx_json.put("Destination", des);

	boost::property_tree::ptree params;
	int flag = 0;
	try {
		params = pt.get_child("params");
		flag = 1;
	}
	catch (boost::property_tree::ptree ex) {}
	try {		
		if ( flag == 1 && params.size() > 0 ) {
			int f = 0;
			boost::property_tree::ptree c;
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v, params) {
				if (v.first != "") {
					f = 1; 
					break;
				}
				if (v.second.data() == "") {
					tx.tx_json.put("params", "Exception21:params must be string");
					return tx;
				}
				boost::property_tree::ptree a;
				boost::property_tree::ptree b;
				a.put("Parameter", StringToHex(v.second.data()));
				b.put_child("Arg", a);
				c.push_back(std::make_pair("", b));
			}
			if (f == 1) {
				tx.tx_json.put("params", "Exception20:invalid option type!");
				return tx;
			}
			tx.tx_json.put_child("Args", c);
		}
	}
	catch(std::exception ex) {}

	std::string foo;
	try {
		foo = pt.get<std::string>("foo");
	}
	catch (boost::property_tree::ptree ex) {
		tx.tx_json.put("foo", "Exception18:foo must be string");
		return tx;
	}
	tx.tx_json.put("ContractMethod", StringToHex(foo));

	tx.tx_json.put("TransactionType", "ConfigContract");
	tx.tx_json.put("Method", 1);

	return tx;
}

void jingtumlib::Remote::_handleMessage(std::string data) {
	
	std::stringstream str(data);
	boost::property_tree::ptree datajs;
	try {
		boost::property_tree::read_json(str, datajs);
	}
	catch (boost::property_tree::ptree_error ex) { std::cout << ex.what() << std::endl; return; }
	if (datajs.size() == 0) return;
	std::string datatype = datajs.get<std::string>("type");
	if (datatype == "ledgerClosed") {
		_handleLedgerClosed(datajs);
	}
	else if (datatype == "serverStatus") {
		_handleServerStatus(datajs);
	}
	else if (datatype == "response") {
		_handleResponse(datajs);
	}
	else if (datatype == "transaction") {
		_handleTransaction(datajs);
	}
	else if (datatype == "path_find") {
		_handlePathFind(datajs);
	}
	
}

void jingtumlib::Remote::_handleLedgerClosed(boost::property_tree::ptree data) {
	if (data.get<int>("ledger_index") > _status.get<int>("ledger_index")) {
		_status.put("ledger_index", data.get<int>("ledger_index"));
		_status.put("ledger_time", data.get<int>("ledger_time"));
		_status.put("reserve_base", data.get<int>("reserve_base"));
		_status.put("reserve_inc", data.get<int>("reserve_inc"));
		_status.put("fee_base", data.get<int>("fee_base"));
		_status.put("fee_ref", data.get<int>("fee_ref"));
		//注意，事件绑定时可能要修改
		Request req = subscribe("ledger_closed");
		req.submit();
	}
}

void jingtumlib::Remote::_handleServerStatus(boost::property_tree::ptree data) {
	_updateServerStatus(data);
	//注意，事件绑定时可能要修改
	/*Request req = subscribe("server_status");
	req.submit(NULL);*/
}

void jingtumlib::Remote::_handleResponse(boost::property_tree::ptree data) {
	
	int req_id,flag;
	try { req_id = data.get<int>("id"); flag = 1; }
	catch (boost::property_tree::ptree ex) { flag = 0; };
	if ( flag != 1 || req_id < 0 || (unsigned int)req_id > server->bmsg.size()) {
		return;
	}

	boost::property_tree::ptree result;
	std::string server_status;
	std::string status;
	try {
		result = data.get_child("result");
		server_status = result.get<std::string>("server_status");
		status = data.get<std::string>("status");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if ( flag == 1 && status == "success") {
		_updateServerStatus(result);
	}

}

void jingtumlib::Remote::_handleTransaction(boost::property_tree::ptree data) {
	std::string tx = data.get_child("transaction").get<std::string>("hash");
	if (_cache->get(tx) != "") return;
	_cache->set(tx, std::to_string(1));
	Request req = subscribe("transactions");
	req.submit();

}

void jingtumlib::Remote::_handlePathFind(boost::property_tree::ptree data) {
	/*Request req = subscribe("path_find");
	req.submit(NULL);*/
}

void jingtumlib::Remote::_updateServerStatus(boost::property_tree::ptree data) {
	
	_status.put("load_base", data.get<std::string>("load_base"));
	_status.put("load_factor", data.get<std::string>("load_factor"));
	try {
		_status.put("pubkey_node", data.get<std::string>("pubkey_node"));
	}
	catch (std::exception ex) { std::cout << ex.what() << std::endl; };
	_status.put("server_status", data.get<std::string>("server_status"));

	int index;
	for (unsigned int i = 0; i < 6; ++i) {
		if (data.get<std::string>("server_status") == server->onlineStates[i]) index = i;
	}
	server->setState(~index ? "online" : "offline");
}


void jingtumlib::Remote::parseUrl(std::string url) {
	boost::regex ex("(ws|wss)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
	boost::cmatch what;
	if (regex_match(url.c_str(), what, ex))
	{
		protocol = std::string(what[1].first, what[1].second);
		domain = std::string(what[2].first, what[2].second);
		port = std::string(what[3].first, what[3].second);
	}
}

// requestServerInfo 回调函数
std::string callback_requestServerInfo(std::string msg, para data) {
	std::stringstream str_stream(msg);
	boost::property_tree::ptree pt;
	/*try {*/
		boost::property_tree::read_json(str_stream, pt);
	/*}
	catch (std::exception ex) {
		std::cout << ex.what() << "Backend message parse error!" << std::endl;
		exit(0);
	}*/
	boost::property_tree::ptree tmp;
	tmp = pt.get_child("info");

	boost::property_tree::ptree res;

	res.put("complete_ledgers", tmp.get<std::string>("complete_ledgers"));
	res.put("ledger", tmp.get_child("validated_ledger").get<std::string>("hash"));
	res.put("public_key", tmp.get<std::string>("pubkey_node"));
	res.put("state", tmp.get<std::string>("server_state"));
	res.put("peers", tmp.get<int>("peers"));
	res.put("version", "skywelld-" + tmp.get<std::string>("build_version"));

	std::stringstream output;
	boost::property_tree::write_json(output, res);
	return output.str();
}

// requestLedgerClosed 回调函数
std::string callback_requestLedgerClosed(std::string msg, para data) {
	std::stringstream str_stream(msg);
	boost::property_tree::ptree pt;
	/*try {*/
		boost::property_tree::read_json(str_stream, pt);
	/*}
	catch (std::exception ex) {
		std::cout << ex.what() << "Backend message parse error!" << std::endl;
		exit(0);
	}*/

	boost::property_tree::ptree res;

	res.put("ledger_hash", pt.get<std::string>("ledger_hash"));
	res.put("ledger_index", pt.get<std::string>("ledger_index"));

	std::stringstream output;
	boost::property_tree::write_json(output, res);
	return output.str();
}

// requestLedger 回调函数
std::string callback_requestLedger(std::string msg, para data) {
	std::stringstream str_stream(msg);
	boost::property_tree::ptree pt;
	/*try {*/
		boost::property_tree::read_json(str_stream, pt);
	/*}
	catch (std::exception ex) {
		std::cout <<  "Backend message parse error!" << std::endl;
		exit(0);
	}*/

	boost::property_tree::ptree ledger;
	boost::property_tree::ptree closedledger;
	bool flag;
	try {
		ledger = pt.get_child("ledger");
		flag = true;
	}
	catch (std::exception ex) {}
	try {
		closedledger = pt.get_child("closed").get_child("ledger");
		flag = false;
	}
	catch (std::exception ex) {}
	ledger = flag ? ledger : closedledger;

	std::stringstream res;
	if (!data.fg) {		
		boost::property_tree::write_json(res, ledger);
		return res.str();
	}
	else {
		boost::property_tree::ptree tmp;
		tmp.put("accepted", ledger.get<std::string>("accepted"));
		tmp.put("ledger_hash", ledger.get<std::string>("ledger_hash"));
		tmp.put("ledger_index", ledger.get<std::string>("ledger_index"));
		tmp.put("parent_hash", ledger.get<std::string>("parent_hash"));
		tmp.put("close_time", ledger.get<std::string>("close_time_human"));
		tmp.put("total_coins", ledger.get<std::string>("total_coins"));
		boost::property_tree::write_json(res, tmp);
		return res.str();
	}
}

// requestAccountTx 回调函数
std::string callback_requestAccountTx(std::string msg, para data) {

	boost::property_tree::ptree result;
	std::stringstream str_stream(msg);
	boost::property_tree::ptree pt;
	boost::property_tree::read_json(str_stream, pt);

	boost::property_tree::ptree transactions;
	transactions = pt.get_child("transactions");	
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, transactions) {
		boost::property_tree::ptree _tx = processTx(v.second, data.str);
		result.push_back(std::make_pair("", _tx));  
	}	

	pt.erase("transactions");  
	pt.put_child("transactions", result);  
	std::stringstream out;
	boost::property_tree::write_json(out, pt);

	return out.str();
}

int getRelationType(std::string type) {
	if (type == "trustline") {
		return 0;
	}
	else if (type == "authorize") {
		return 1;
	}
	else if (type == "freeze") {
		return 2;
	}
	return -1;
}

std::string processContract(std::string str) {
	return StringToHex(str);
}



