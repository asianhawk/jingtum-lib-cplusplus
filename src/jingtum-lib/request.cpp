#include "pch.h"
#include "stdafx.h"
#include "jingtumlib.h"

std::string LEDGER_STATES[3] = { "current", "closed", "validated" };

jingtumlib::Request::Request(jingtumlib::Remote *r) {
	rem = r;
	_command = "";
	_cbfunc = NULL;
	_message = "";
	pa.fg = true;
	pa.str = "";
}

jingtumlib::Request::Request(jingtumlib::Remote *r, std::string command, std::string (* cbfunc)(std::string, para) ) {
	rem = r;
	_command = command;	
	_cbfunc = cbfunc;
	_message = "";	
	pa.fg = true;
	pa.str = "";
};

void jingtumlib::Request::submit() {
	rem->_submit(_command, _message, pa, _cbfunc, NULL, NULL, NULL, NULL);
}

void jingtumlib::Request::submit(void(*callback)(std::string)) {
	rem->_submit(_command, _message, pa, _cbfunc, NULL, NULL, NULL, callback);
}

void jingtumlib::Request::submit(void(*callback)(std::string, std::string, jingtumlib::Remote *)) {
	rem->_submit(_command, _message, pa, _cbfunc, callback, NULL, NULL, NULL);
}

void jingtumlib::Request::submit(void(* callback)(std::string, std::string)) {	
	std::size_t found = _message.find("Exception");
	int k;
	if (found != std::string::npos) {
		std::stringstream t(_message.substr(found + 9, 2));
		t >> k;
		callback(_message.substr(found + 12,k), "");
	}
	else {
		rem->_submit(_command, _message, pa, _cbfunc, NULL, callback, NULL, NULL);
	}	
}

void jingtumlib::Request::submit(void(*callback)(std::string, std::string, int)) {
	std::size_t found = _message.find("Exception");
	int k;
	if (found != std::string::npos) {
		std::stringstream t(_message.substr(found + 9, 2));
		t >> k;
		callback(_message.substr(found + 12, k), "", 0);
	}
	else {
		rem->_submit(_command, _message, pa, _cbfunc, NULL, NULL, callback, NULL);
	}
}

void jingtumlib::Request::selectLedger(std::string ledger) {
	bool tereg = false;	
	boost::regex reg("^[A-F0-9]+$");
	boost::cmatch what;
	tereg = regex_match(ledger.c_str(), what, reg);

	int k = -1;
	for (unsigned int i = 0; i < 3; ++i)
		if (ledger == LEDGER_STATES[i]) k = i;
	if (!isNum(ledger) && ~k) {
		_message = _message + ",\"ledger_index\":" + ledger;
	}
	else if (isNum(ledger)) {
		_message = _message + ",\"ledger_index\":" + ledger;
	} 
	else if (tereg) {
		_message = _message + ",\"ledger_hash\":" + ledger;
	}
	else 
		_message = _message + ",\"ledger_index\":\"validated\"";	

}




