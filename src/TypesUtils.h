#ifndef TYPESUTILS_H
#define TYPESUTILS_H

#define BOOST_SPIRIT_THREADSAFE

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <string>

#define RE 0
#define OP 1
#define DE 2

const std::string TYPES_MAP[21] = {
	"none",
	"Int16",
	"Int32",
	"Int64",
	"Hash128",
	"Hash256",
	"Amount",
	"VL",
	"Account",
	"none",
	"none",
	"none",
	"none",
	"none",
	"Object",
	"Array",
	"Int8",
	"Hash160",
	"PathSet",
	"Vector256",
	"Memo"
};

boost::property_tree::ptree FIELDS_MAP();
boost::property_tree::ptree INVERSE_FIELDS_MAP();

boost::property_tree::ptree base();
boost::property_tree::ptree TRANSACTION_TYPES(boost::property_tree::ptree);
boost::property_tree::ptree sleBase();
boost::property_tree::ptree LEDGER_ENTRY_TYPES(boost::property_tree::ptree);
boost::property_tree::ptree METADATA();


#endif TYPESUTILS_H
