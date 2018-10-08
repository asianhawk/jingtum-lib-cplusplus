#ifndef UTILS_H
#define UTILS_H

#include <boost/regex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <string>
#include <typeinfo>
#include "keypairs.h"

#define currency "SWT"
#define ACCOUNT_ZERO "jjjjjjjjjjjjjjjjjjjjjhoLvTp"
#define ACCOUNT_ONE "jjjjjjjjjjjjjjjjjjjjBZbvri"
#define fee 10000

std::string HexToString(std::string);
std::string StringToHex(std::string);

bool isValidHash(std::string);
bool isValidAddress(std::string);
bool isValidSecret(std::string);
bool isValidAmount(boost::property_tree::ptree);
bool isValidCurrency(std::string);
bool isValidAmount0(boost::property_tree::ptree);

boost::property_tree::ptree LEDGER_FLAGS();
boost::property_tree::ptree TransactionFlags();
boost::property_tree::ptree TransactionSetClearFlags();

std::string getPureStr(std::string);
boost::property_tree::ptree extendThree(boost::property_tree::ptree, boost::property_tree::ptree, boost::property_tree::ptree);
boost::property_tree::ptree processAffectNode(boost::property_tree::ptree);

std::string formatArgs(boost::property_tree::ptree);
std::string UTF8_To_string(const std::string & str);
std::string string_To_UTF8(const std::string & str);

bool isAmountZero(boost::property_tree::ptree);
boost::property_tree::ptree AmountNegate(boost::property_tree::ptree);
boost::property_tree::ptree AmountAdd(boost::property_tree::ptree, boost::property_tree::ptree);
boost::property_tree::ptree AmountSubtract(boost::property_tree::ptree, boost::property_tree::ptree);
std::string AmountRatio(boost::property_tree::ptree, boost::property_tree::ptree);
std::string getPrice(boost::property_tree::ptree, bool);

boost::property_tree::ptree parseAmount(boost::property_tree::ptree, std::string);
boost::property_tree::ptree reverseAmount(boost::property_tree::ptree, std::string);

boost::property_tree::ptree processTx(boost::property_tree::ptree, std::string);
std::string txnType(boost::property_tree::ptree, std::string);

#endif UTILS_H
