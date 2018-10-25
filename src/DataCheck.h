#ifndef DATACHECK_H
#define DATACHECK_H

#define BOOST_SPIRIT_THREADSAFE

#include <boost/regex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <string>

#define CURRENCY_NAME_LEN 3
#define CURRENCY_NAME_LEN2 6
#define TUM_NAME_LEN 40

bool isLetterNumber(std::string);
bool isCurrency(std::string);
bool isCustomTum(std::string);
bool isTumCode(std::string);



#endif DATACHECK_H
