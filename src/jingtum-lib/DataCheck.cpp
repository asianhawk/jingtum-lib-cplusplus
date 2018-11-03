#include "pch.h"
#include "stdafx.h"
#include "DataCheck.h"

bool isCurrency(std::string in_code) {
	return (in_code != "" ? (in_code.length() >= CURRENCY_NAME_LEN && in_code.length() <= CURRENCY_NAME_LEN2 ? true : false):false );
}

bool isLetterNumber(std::string in_str) {
	boost::regex number_RE("^[0-9A-Z]+$");
	boost::cmatch m;
	return regex_match(in_str.c_str(), m, number_RE);
}

bool isCustomTum(std::string in_code) {
	if (isLetterNumber(in_code) && in_code.length() == TUM_NAME_LEN) return true;
	else return false;
}

bool isTumCode(std::string in_code) {
	return (in_code != "" && ( in_code == "SWT" || isCurrency(in_code) || isCustomTum(in_code) ));
}