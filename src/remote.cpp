#include "pch.h"
#include "jingtumlib.h"

std::string OfferTypes[2] = { "Sell", "Buy" };
std::string RelationTypes[4] = { "trust", "authorize", "freeze", "unfreeze" };
std::string AccountSetTypes[3] = { "property", "delegate", "signer" };

jingtumlib::Remote::Remote(std::string serverInfo) {

	std::stringstream str_stream(serverInfo);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);		
	}
	catch (std::exception ex) {
		std::cout << ex.what() << "Server Information Parse Error!" <<std::endl;
		exit(0);
	}

	_url = pt.get<std::string>("server");
	_localsign = pt.get<bool>("local_sign");
	parseUrl(_url);

	if (protocol != "wss" && protocol != "ws") {
		std::cout << "Please Check the Protocol!" << std::endl;
		exit(0);
	}
};
	
jingtumlib::Remote::~Remote() {
	if (server) {
		delete server;
	}
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

		try {
			if (server->connect() == true) {
				server->_opened = true;
				std::string text = "[\"ledger\",\"server\"]";
				Request req = subscribe(text);
				req.submit(NULL);				
				callback("", server->bmsg[0].backmsg, this);
			}
		}
		catch (std::exception ex) {
			std::cout << ex.what() << "Server Connecting Wrong!" << std::endl;
			callback(ex.what(), "", NULL);
			exit(0);
		}
	}
	else {
		std::cout << "Connection has established!" << std::endl;
	}	
}

void jingtumlib::Remote::disConnect() {
	if (server != NULL && server->_connected == true) {
		server->disconnect();
	}
}

void jingtumlib::Remote::_submit(std::string command, std::string data, para req, std::string (* filter)(std::string, para), void(* callback)(std::string, std::string, Remote *)) {
	try {
		int id = server->sendMessage(command, data);
		std::cout << "Request Serial Number:" << id << std::endl;

		_handleMessage(id, server->bmsg[id].datatype);

		if (server->bmsg[id].datastatus == "success") {
			if (filter != NULL) {	
				server->bmsg[id].backmsg = filter(server->bmsg[id].backmsg, req);
			}
		}				
	
		if (callback != NULL) {
			callback("", server->bmsg[id].backmsg, this);
		}
	}
	catch (std::exception ex) {
		std::cout << ex.what() << "Backend Message Error!" << std::endl;
		callback(ex.what(), "", NULL);
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
		long int index;
		std::string st;
		int flag = 0;
		try {
			index = pt.get<long int>("ledger_index");
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
		req->_message = req->_message + ",\"relation_type\":" + std::to_string(getRelationType(st));
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

	long int limit;
	try {
		limit = pt.get<long int>("limit");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		if (limit < 0) limit = 0;
		if (limit > 1e9) limit = (long int)1e9;
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
	long int k;
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
		k = pt.get<long int>("ledger_min");
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
		k = pt.get<long int>("ledger_max");
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
		k = pt.get<long int>("limit");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		req._message = req._message + ",\"limit\":" + std::to_string(k);
	}

	try {
		k = pt.get<long int>("offset");
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
			k = market.get<long int>("ledger");
			flag = 2;
		}
		catch (boost::property_tree::ptree_error ex) { flag = 0; }
		if (flag == 2 && k != NAN) {
			try {
				k = market.get<long int>("seq");
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

void jingtumlib::Remote::_handleMessage(int id, std::string datatype) {
	
	if (datatype == "") {
		std::cout << "Unknow backend data type!" << std::endl;
		exit(0);
	}
	else {
		if (datatype == "ledgerClosed") {
			_handleLedgerClosed();
		}
		else if (datatype == "serverStatus") {
			_handleServerStatus();
		}
		else if (datatype == "response") {
			_handleResponse(id);
		}
		else if (datatype == "transaction") {
			_handleTransaction();
		}
		else if (datatype == "path_find") {
			_handlePathFind();
		}
	}
}

void jingtumlib::Remote::_handleLedgerClosed() {

}

void jingtumlib::Remote::_handleServerStatus() {

}

void jingtumlib::Remote::_handleResponse(int id) {
	
	if (server->bmsg[id].dataid != (int)server->bmsg[id].dataid || server->bmsg[id].dataid < 0) {
		return;
	}

	if (server->bmsg[id].dataresult != "" && server->bmsg[id].datastatus == "success" && server->bmsg[id].dataresultServerStatus != "" ) {
		_updateServerStatus(id);
	}
}

void jingtumlib::Remote::_handleTransaction() {

}

void jingtumlib::Remote::_handlePathFind() {

}

void jingtumlib::Remote::_updateServerStatus(int id) {
	std::stringstream str_stream(server->bmsg[id].dataresult);
	boost::property_tree::ptree res;
	/*try {*/
		boost::property_tree::read_json(str_stream, res);
	/*}
	catch (std::exception ex) {
		std::cout << ex.what()  << std::endl;
		exit(0);
	}*/

	_status.put("load_base", res.get<std::string>("load_base"));
	_status.put("load_factor", res.get<std::string>("load_factor"));
	try {
		_status.put("pubkey_node", res.get<std::string>("pubkey_node"));
	}
	catch (std::exception ex) {};
	_status.put("server_status", res.get<std::string>("server_status"));

	int index;
	for (unsigned int i = 0; i < 6; ++i) {
		if (res.get<std::string>("server_status") == server->onlineStates[i]) index = i;
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



