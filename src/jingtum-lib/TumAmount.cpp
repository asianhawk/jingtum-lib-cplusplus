#include "pch.h"
#include "stdafx.h"
#include "TumAmount.h"

Amount::Amount() {
	_offset = 0;
	_is_native = true;
	_is_negative = false;
	_currency = "";
	_issuer = "";
}

Amount::~Amount() {

}

bool Amount::is_valid() {
	return true;
}

bool Amount::is_native() {
	return _is_native;
}

bool Amount::is_negative() {
	return _is_negative;
}

bool Amount::is_zero() {
	return _value == 0;
}

bool Amount::is_positive() {
	return !is_zero() && !is_negative();
}

std::string Amount::issuer() {
	return _issuer;
}

void Amount::parse_issuer(std::string issuer) {
	if (isValidAddress(issuer)) _issuer = issuer;
}
void Amount::parse_json(std::string in_json) {
	
	bool JSON = isJSON(in_json);		
	boost::property_tree::ptree in_jsonJ;
	if (JSON == true) {
		std::stringstream str(in_json);
		boost::property_tree::read_json(str, in_jsonJ);
	}
	
	try {
		if (JSON == false && in_json != "") {
			parse_swt_value(in_json);
		}	
		else if (JSON == true) {
			std::string cur = in_jsonJ.get<std::string>("currency");
			if (!isTumCode(cur)) throw std::string("Amount.parse_json: Input JSON has invalid Tum info!");
			else {
				if (cur != "SWT") {
					_currency = cur;
					_is_native = false;
					std::string issuer;
					int flag;
					try {
						issuer = in_jsonJ.get<std::string>("issuer");
						flag = 1;
					}
					catch (boost::property_tree::ptree_error ex) { flag = 0; }
					if (flag == 1 && issuer != "undefined" && issuer != "")
						if (isValidAddress(issuer)) {
							_issuer = issuer;
							double value = in_jsonJ.get<double>("value");
							std::stringstream str;
							str << std::setiosflags(std::ios::scientific) << value;
							std::string vpow = str.str();
							//std::string vpow = vpow.substr(vpow.find_last_of("e") + 1);
							vpow = vpow.substr(vpow.find_last_of("e") + 1);
							std::stringstream str2(vpow);
							int t;
							str2 >> t;
							int offset = 15 - t;
							double factor = std::pow(10, offset);
							_value = (unsigned long long)(value * factor);
							_offset = -1 * offset;
						}
						else throw std::string("Amount.parse_json: input JSON has invalid issuer info!");
					else throw std::string("Amount.parse_json: input JSON has invalid issuer info!");
				}
				else {
					std::string value = in_jsonJ.get<std::string>("value");
					parse_swt_value(value);
				}
			}
		}
		else throw std::string("Amount.parse_json: Unsupported JSON type!");
	}
	catch (std::string ex) {
		std::cout << ex << std::endl;
		exit(0);
	}
}

void Amount::parse_swt_value(std::string j) {

	boost::regex swt_RE("^(-?)(\\d*)(\\.\\d{0,6})?$");
	boost::smatch m;
	//bool flag = regex_match(j.c_str(), m, swt_RE);	
	bool flag = regex_match(j, m, swt_RE);	

	if (flag) {
		if (m[3] == "") {
			std::stringstream str(m[2]);
			float c;
			str >> c;
			_value = (unsigned long long)(c * 1000000);
		}
		else {
			std::stringstream str(m[2]), str2(m[3]);
			float c, d;
			str >> c; str2 >> d;
			_value = (unsigned long long)((c + d)*1000000);
		}
				
		_is_native = true;
		_offset = 0;
		bool c;
		if (m[1] == "") c = false;
		else if (!isNum(m[1])) {
			std::stringstream str(m[1]);
			double c;
			str >> c;
			if (abs(c) > 1e7) c = false;
			else c = true;
		}
		else c = true;
		_is_negative = c && _value != 0;

		if (_value > bi_xns_max) {
			_value = std::numeric_limits<unsigned long long>::quiet_NaN();
		}
			
	}
	else _value = std::numeric_limits<unsigned long long>::quiet_NaN();
}

void Amount::parse_tum_value(boost::property_tree::ptree j) {
}

void Amount::to_json() {
}

std::vector<unsigned char> Amount::tum_to_bytes() {
	std::vector<unsigned char> currencyData;
	for (int i = 0; i < 20; ++i) currencyData.push_back(0);

	try {
		if (_currency.length() >= CURRENCY_NAME_LEN && _currency.length() <= CURRENCY_NAME_LEN2) {
			std::string currencyCode = _currency;
			int end = 14;
			int len = currencyCode.length() - 1;
			for (int j = len; j >= 0; j--) {
				char c = currencyCode[len - j];
				currencyData[end - j] = (int)c & 0xff;
			}
		}
		else if (_currency.length() == 40) {
			boost::regex RE("^[0-9A-F]", boost::regex::icase);
			boost::cmatch m;
			if (regex_match(_currency.c_str(), m, RE)) {
				currencyData = convertHexToByteArray(_currency);
			}
			else throw std::string("Invalid currency code");
		}
		else throw std::string("Incorrect currency code length");
	}
	catch (std::string ex) { std::cout << ex << std::endl; }

	return currencyData;
}