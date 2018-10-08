#include "pch.h"
#include "jingtumlib.h"
#include "Serializer.h"

jingtumlib::Transaction::Transaction(jingtumlib::Remote *r) {
	rem = r;
	tx_json.put("Flags", 0);
	tx_json.put("Fee", fee);
	
	pa.fg = true;
	pa.str = "";
}

void jingtumlib::Transaction::setSecret(std::string secret) {
	if (!isValidSecret(secret)) {
		tx_json.put("_secret", "Exception13:valid secret!");
		return;
	}
	_secret = secret;
}

void jingtumlib::Transaction::addMemo(std::string memo) {
	if (memo.length() > 2048) {
		tx_json.put("memo_len", "Exception17:memo is too long!");
		return;
	}
	boost::property_tree::ptree _memo;
	_memo.put("MemoData", StringToHex(string_To_UTF8(memo)));
	boost::property_tree::ptree me;
	try {
		me = tx_json.get_child("Memos");
	}
	catch (boost::property_tree::ptree_error ex) {}
	me.push_back(std::make_pair("", _memo));
	tx_json.put_child("Memos", me);
}
//
//std::string jingtumlib::Transaction::signing() {
//
//}


void jingtumlib::Transaction::submit(void(*callback)(std::string, std::string, Remote *)) {
	std::stringstream tj;
	boost::property_tree::write_json(tj, tx_json);
	std::string ss = tj.str();
	std::size_t found = ss.find("Exception");
	int k;
	if (found != std::string::npos) {
		std::stringstream t(ss.substr(found + 9, 2));
		t >> k;
		callback(ss.substr(found + 12, k), "", NULL);
	}
	else {
		if (tx_json.get<std::string>("TransactionType") == "Signer") {
			std::string data = "{\"tx_blog\":\"" + tx_json.get<std::string>("blob") + "\"";
			rem->_submit("submit", data, pa, NULL, callback);
		}
		else if (rem->_localsign) {
			// not done
		}
		else {
			boost::property_tree::ptree data;
			data.put("secret", _secret);
			data.put_child("tx_json", tx_json);
			std::stringstream da;
			boost::property_tree::write_json(da, data);
			rem->_submit("submit", da.str(), pa, NULL, callback);
		}
		
		
		
		
		
	}

}