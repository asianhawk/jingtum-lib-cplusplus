#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <string>
#include <typeinfo>

#define RE 0
#define OP 1
#define DE 2

boost::property_tree::ptree base();
boost::property_tree::ptree TRANSACTION_TYPES(boost::property_tree::ptree);
boost::property_tree::ptree sleBase();
boost::property_tree::ptree LEDGER_ENTRY_TYPES(boost::property_tree::ptree);
boost::property_tree::ptree METADATA();


#endif SERIALIZER_H
