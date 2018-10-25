#include "pch.h"
#include "jingtumlib.h"

std::string LEDGER_STATES[3] = { "current", "closed", "validated" };

jingtumlib::Request::Request(jingtumlib::Remote *r) {
	rem = r;
	_command = "";
	_filter = NULL;
	_message = "";
	pa.fg = true;
	pa.str = "";
	pa.addr = 0;
}

jingtumlib::Request::Request(jingtumlib::Remote *r, std::string command, std::string (* filter)(std::string, para) ) {
	rem = r;
	_command = command;	
	_filter = filter;
	_message = "";	
	pa.fg = true;
	pa.str = "";
	pa.addr = 0;
};

void jingtumlib::Request::submit() {
	std::cout << "request submit1:" << std::endl;
	rem->_submit(_command, _message, pa, _filter);
}

void jingtumlib::Request::submit(void(* callback)(std::string, std::string)) {	
	std::cout << "request submit2:" << std::endl;

	std::size_t found = _message.find("Exception");
	int k;
	if (found != std::string::npos) {
		std::stringstream t(_message.substr(found + 9, 2));
		t >> k;
		callback(_message.substr(found + 12,k), "");
	}
	else {
		rem->_submit(_command, _message, pa, _filter, callback);
	}	
}

void jingtumlib::Request::submit(void(*callback)(std::string, std::string, int)) {
	std::cout << "request submit3:" << std::endl;

	std::size_t found = _message.find("Exception");
	int k;
	if (found != std::string::npos) {
		std::stringstream t(_message.substr(found + 9, 2));
		t >> k;
		callback(_message.substr(found + 12, k), "", 0);
	}
	else {
		rem->_submit(_command, _message, pa, _filter, callback);
	}
}

void jingtumlib::Request::selectLedger(boost::property_tree::ptree pt) {
	int index;
	std::string st;
	int flag;
	bool tereg = false;
	try {
		st = pt.get<std::string>("ledger");
		flag = 1;
	}
	catch (std::exception ex) { flag = 0; }
	try {
		index = pt.get<int>("ledger");
		flag = 2;
	}
	catch (std::exception ex) { flag = 0; }

	if (flag == 1) {
		boost::regex reg("^[A-F0-9]+$");
		boost::cmatch what;
		tereg = regex_match(st.c_str(), what, reg);
	}

	int k;
	for(unsigned int i = 0; i < 3; ++i)
		if (st == LEDGER_STATES[i]) k = i;

	if (flag == 1 && ~k ) {
		_message = _message + ",\"ledger_index\":\"" + st + "\"";
	}
	else if (flag == 2) {
		_message = _message + ",\"ledger_index\":" + st;
	}
	else if (tereg) {
		_message = _message + ",\"ledger_hash\":\"" + st + "\"";
	}
	else {
		_message = _message + ",\"ledger_index\":\"validated\"" ;
	}
}




