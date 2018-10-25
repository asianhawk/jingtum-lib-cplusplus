#include "pch.h"
#include "jingtumlib.h"
#include "Serializer.h"

jingtumlib::Transaction::Transaction(jingtumlib::Remote *r) {
	rem = r;
	tx_json.put("Flags", 0);
	tx_json.put("Fee", fee);
}

void jingtumlib::Transaction::setSecret(std::string secret) {
	if (!isValidSecret(secret)) {
		tx_json.put("_secret", "Exception13:valid secret!");
		return;
	}
	_secret = secret;
}

void jingtumlib::Transaction::setFlags(std::string flag) {
	if (flag == "") return;
	if (isNum(flag)) { 
		std::stringstream str(flag);
		int f;
		str >> f;
		tx_json.put("Flags", f); 
		return;
	}
	boost::property_tree::ptree transaction_flags;
	try {
		transaction_flags = TransactionFlags().get_child(tx_json.get<std::string>("TransactionType"));
	}
	catch(boost::property_tree::ptree_error ex) {}

	boost::property_tree::ptree flag_set;
	int f = 0;
	try {
		std::stringstream str(flag);
		boost::property_tree::read_json(str, flag_set);
		f = 1;
	}	
	catch(boost::property_tree::ptree_error ex){}

	int index = 0;
	if (f == 1) {		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, flag_set) {
			std::string ff = v.second.data();
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v1, transaction_flags) {
				if (v1.first == ff) {
					std::stringstream s(v1.second.data());
					int a;
					s >> a;
					index += a;
				}
			}
		}
	}
	else {
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v1, transaction_flags) {
			if (v1.first == flag) {
				std::stringstream s(v1.second.data());
				int a;
				s >> a;
				index += a;
			}
		}
	}
	int Flags=0;
	try {
		Flags = tx_json.get<int>("Flags");
	}
	catch (boost::property_tree::ptree_error ex) {}
	Flags += index;
	tx_json.put("Flags", Flags);
}

std::string jingtumlib::Transaction::getAccount() {
	return tx_json.get<std::string>("Account");
}

std::string jingtumlib::Transaction::getTransactionType() {
	return tx_json.get<std::string>("TransactionType");
}

void jingtumlib::Transaction::setSendMax(boost::property_tree::ptree amount) {
	try {
		if (!isValidAmount(amount)) throw std::string("invalid send max amount");
		else tx_json.put_child("SendMax", amount);
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }

}

void jingtumlib::Transaction::setTransferRate(double rate) {
	try {
		if (rate < 0 || rate > 1) throw std::string("invalid transfer rate");
		else tx_json.put("TransferRate", (rate+1)*1000000000);
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
}

void jingtumlib::Transaction::setPath(std::string key) {
	try {
		if (key.length() != 40) throw std::string("invalid path key");
		std::string itemS = rem->_paths->get(key);
		if (itemS == "") throw std::string("non exists path key");
		boost::property_tree::ptree item;
		std::stringstream str(itemS);
		boost::property_tree::read_json(str, item);
		std::string path;
		try {
			path = item.get<std::string>("path");
		}
		catch(boost::property_tree::ptree_error ex) {}
		if (path == "[]") return;
		tx_json.put("Paths", path);
		boost::property_tree::ptree amount = MaxAmount(item.get<std::string>("choice"));
		tx_json.put_child("SendMax", amount);

	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
}

boost::property_tree::ptree MaxAmount(std::string amountS) {
	try {
		std::stringstream str(amountS);
		boost::property_tree::ptree amount;
		boost::property_tree::read_json(str, amount);
		if (amount.size() == 0) {
			std::stringstream s(amountS);
			double _amount;
			s >> _amount;
			boost::property_tree::ptree str;
			str.put("String", std::to_string((int)(_amount*1.0001)));
			return str;
		}
		else if (amount.size() > 0 && isValidAmount(amount)) {
			boost::property_tree::ptree am;
			am.put("value", std::to_string(amount.get<double>("value")*1.0001));
			return am;
		}
		else {
			throw std::string("invalid amount to max");
		}
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
}

void jingtumlib::Transaction::addMemo(std::string memo) {
	if (memo.length() > 2048) {
		tx_json.put("memo_len", "Exception17:memo is too long!");
		return;
	}
	boost::property_tree::ptree _memo, Memo;
	std::string aa = string_To_UTF8(memo);
	_memo.put("MemoData", StringToHex(string_To_UTF8(memo)));
	Memo.put_child("Memo",_memo);
	boost::property_tree::ptree me;
	try {
		me = tx_json.get_child("Memos");
	}
	catch (boost::property_tree::ptree_error ex) {}
	me.push_back(std::make_pair("", Memo));
	tx_json.put_child("Memos", me);
}

void jingtumlib::Transaction::submit(void(*callback)(std::string, std::string)) {
	std::stringstream tj;
	boost::property_tree::write_json(tj, tx_json);
	std::string ss = getPureStr(tj.str());
	std::size_t found = ss.find("Exception");
		
	int k;
	if (found != std::string::npos) {
		std::stringstream t(ss.substr(found + 9, 2));
		t >> k;
		callback(ss.substr(found + 12, k), "");
	}
	else {
		if (tx_json.get<std::string>("TransactionType") == "Signer") {
			std::cout << "transaction submit1:" << ss << std::endl;
			std::string data = "\"tx_blog\":\"" + tx_json.get<std::string>("blob") + "\"";
			rem->_submit("submit", data, pa, NULL, callback);
			
		}
		else if (rem->_localsign) {
			std::cout << "transaction submit2:" << ss << std::endl;
			pa._remcs = callback;
			pa._reqcs = &remote_callback_submit;
			pa.addr = (int)this;
			sign(&remote_callback_submit);
		}
		else {
			std::cout << "transaction submit3:" << ss << std::endl;
			std::string data = "\"secret\":\""+tx_json.get<std::string>("_secret")+"\",\"tx_blob\":\"" + tx_json.get<std::string>("blob") + "\"";
			rem->_submit("submit", getPureStr(data), pa, NULL, callback);
		}
	}
}

void jingtumlib::Transaction::sign(void(* callback)(std::string, std::string, int)) {
	int flag;
	int seq;
	try {
		seq = tx_json.get<int>("Sequence");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		callback("", signing(), 0);
	}
	else {
		std::string account = tx_json.get<std::string>("Account");
		Request req = rem->requestAccountInfo("{\"account\":\""+account +"\",\"type\":\"trust\"}");		
		req.pa.addr = (int)this;
		req.submit(&req_callback_submit);		
	}
}

void req_callback_submit(std::string err, std::string data, int addr) {

	jingtumlib::Transaction *tx = (jingtumlib::Transaction *)addr;

	if (err != "") {
		std::cout << "sign error" << std::endl;
		return tx->pa._reqcs(err, "", tx->pa.addr);
	}
	else {
		std::cout << "sign enter" << std::endl;
		std::stringstream str(data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(str, pt);
		std::cout << "sign parse ok" << std::endl;
		tx->tx_json.put("Sequence", pt.get_child("account_data").get<std::string>("Sequence"));
		tx->pa._reqcs("", tx->signing(), tx->pa.addr);
	}
}

void remote_callback_submit(std::string err, std::string blob, int addr) {
	jingtumlib::Transaction *tx;
	tx = (jingtumlib::Transaction *)addr;

	if (err != "") {
		std::cout << err << std::endl;
		return tx->pa._remcs(err, "");
	}
	else {
		std::string data = "\"tx_blob\":\""+ tx->tx_json.get<std::string>("blob")+"\"";
		tx->rem->_submit("submit", data, tx->pa, NULL, tx->pa._remcs);
	}
}

std::string jingtumlib::Transaction::signing() {
	std::cout << "signing enter" << std::endl;
	double fe = tx_json.get<double>("Fee")/1000000;
	tx_json.put("Fee", std::to_string(fe));
	
	boost::property_tree::ptree am;	
	int flag;
	try {
		am = tx_json.get_child("Amount");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		if (am.size() == 0) {
			double amount;
			amount = tx_json.get<double>("Amount")/1000000;
			tx_json.put("Amount", amount);
		}
		else {
			double amd = am.get<double>("String")/1000000;
			tx_json.erase("Amount");
			tx_json.put("Amount", std::to_string(amd)); 
		}
	}
	
	//未对memo进行解码
	//flag = 0;
	//boost::property_tree::ptree Memos;
	//boost::property_tree::ptree memos;
	//try {
	//	Memos = tx_json.get_child("Memos");
	//	flag = 1;
	//}
	//catch (boost::property_tree::ptree_error ex) {}
	//if (flag == 1) {
	//	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, Memos) {
	//		boost::property_tree::ptree memo = v.second.get_child("Memo");
	//		std::cout << "ok!" << std::endl;
	//		for (auto item : memo) {
	//			try {
	//				memo.put(item.first, UTF8_To_string(HexToString(item.second.data())));
	//			}
	//			catch (boost::property_tree::ptree_error ex) {}
	//		}
	//		memos.push_back(std::make_pair("", memo));
	//	}
	//	tx_json.put_child("Memos", memos);
	//}

	flag = 0;
	double sendmax;
	try {
		sendmax = tx_json.get<double>("SendMax");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (flag == 1) {
		sendmax /= 1000000;
		tx_json.erase("SendMax");
		tx_json.put("SendMax", sendmax);
	}	
	
	boost::property_tree::ptree tp;
	double takerpays;
	flag = 0;
	try {
		tp = tx_json.get_child("TakerPays");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { }
	if (flag == 1 && tp.size() == 1) {
		takerpays = tp.get<double>("String") / 1000000;
		tx_json.erase("TakerPays");
		tx_json.put("TakerPays", std::to_string(takerpays));
	}
	
	boost::property_tree::ptree tg;
	double takergets;
	flag = 0;
	try {
		tg = tx_json.get_child("TakerGets");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) {}
	if (flag == 1 && tg.size() == 1) {
		takergets = tg.get<double>("String") / 1000000;
		tx_json.erase("TakerGets");
		tx_json.put("TakerGets", std::to_string(takerpays));
	}
	
	std::stringstream str0;
	boost::property_tree::write_json(str0, tx_json);
	std::cout << "json0:" << str0.str() << std::endl;
	
	Wallet wt(_secret);
	tx_json.put("SigningPubKey", wt.getPublicKey());
	int prefix = 0x53545800;
	Serializer jser;
	jser.from_json(tx_json);
	std::cout << "signing step6" << std::endl;
	std::string ha = jser.hash(prefix);
	tx_json.put("TxnSignature", wt.signTx(ha));

	std::stringstream str1;
	boost::property_tree::write_json(str1, tx_json);
	std::cout << "json1:" << str1.str() << std::endl;

	jser.from_json(tx_json);
	std::string blob = jser.to_hex();
	std::transform(blob.begin(), blob.end(), blob.begin(), ::toupper);
	tx_json.put("blob", blob);

	std::stringstream str2;
	boost::property_tree::write_json(str2, tx_json);
	std::cout << "json2:" << str2.str() << std::endl;

	_local_sign = true;

	return blob;
}