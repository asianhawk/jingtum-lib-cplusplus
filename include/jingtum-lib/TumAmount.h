#ifndef TUMAMOUNT_H
#define TUMAMOUNT_H

#define BOOST_SPIRIT_THREADSAFE

#include <boost/regex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include "DataCheck.h"
#include "utils.h"

#define bi_xns_max 9e18

class Amount {
public:
	Amount();
	~Amount();

	void parse_json(std::string);
	void parse_tum_value(boost::property_tree::ptree);
	void to_json();
	std::vector<unsigned char> tum_to_bytes();
	bool is_valid();
	bool is_native();
	bool is_negative();
	bool is_zero();
	bool is_positive();
	std::string issuer();

private:
	void parse_swt_value(std::string);		
	void parse_issuer(std::string);	 

public:
	unsigned long long _value;
	int _offset;
	bool _is_native;
	bool _is_negative;
	std::string _currency;
	std::string _issuer;
};

#endif TUMAMOUNT_H
