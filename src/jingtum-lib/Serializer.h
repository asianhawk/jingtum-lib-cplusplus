#ifndef SERIALIZER_H
#define SERIALIZER_H

#define BOOST_SPIRIT_THREADSAFE

#include <boost/regex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <typeinfo>
#include <math.h>
#include "keypairs.h"
#include "TypesUtils.h"
#include "utils.h"
#include "TumAmount.h"

#define CURRENCY_NAME_LEN 3
#define CURRENCY_NAME_LEN2  6

class Serializer {
public:
	Serializer();
	Serializer(std::vector<unsigned char> buf);
	Serializer(std::string);
	~Serializer();

public:
	void from_json(boost::property_tree::ptree);
	void append(std::vector<unsigned char> buf);
	void append(Serializer);
	std::string to_hex();
	std::string hash(int);
	void serialize(Serializer *, boost::property_tree::ptree, boost::property_tree::ptree);
	boost::property_tree::ptree lookup_type_tx(int);
	boost::property_tree::ptree lookup_type_le(int);

	std::vector<unsigned char> buffer;
	int pointer;
	bool no_marker;
private:	

};

////////////////////////////////////////////////////////////////////////////////

bool sort_field_compare(std::string, std::string);
//void STObjectserialize(Serializer *, boost::property_tree::ptree, bool);
//void STObjectserialize(Serializer *, boost::property_tree::ptree);
void STObjectserialize(Serializer *,std::string, bool);
//void serialize(Serializer *, std::string, boost::property_tree::ptree);
void serialize(Serializer *, std::string, std::string);
void STInt8serialize(Serializer *, std::string, bool);
void STInt16serialize(Serializer *, std::string, bool);
void STInt32serialize(Serializer *, std::string, bool);
void STInt64serialize(Serializer *, std::string, bool);
void STHash128serialize(Serializer *, std::string, bool);
void STHash256serialize(Serializer *, std::string, bool);
void STHash160serialize(Serializer *, std::string, bool);
void STCurrencyserialize(Serializer *, std::string, bool);   // to_bytes()?
void STAmountserialize(Serializer *, std::string, bool);
void STVLserialize(Serializer *, std::string, bool);
void STAccountserialize(Serializer *, std::string, bool);
void STArrayserialize(Serializer *, std::string, bool);
void STPathSetserialize(Serializer *, std::string, bool);
void STVector256serialize(Serializer *, std::string, bool);
//void STMemoserialize(Serializer *, boost::property_tree::ptree);
void STMemoserialize(Serializer *, std::string, bool);
int get_transaction_type(std::string);
std::string get_transaction_type(int);
int get_ledger_entry_type(std::string);
std::string get_ledger_entry_type(int);

std::vector<unsigned char> convertIntegerToByteArray(std::string, int);
void serializeHex(Serializer *, std::string, bool);

std::string convertStringtoHex(std::string);
void serialize_varint(Serializer *, int);
void serialize_varint(Serializer *, std::string);
bool isHexInt64String(std::string);
bool isHexString(std::string);
bool isCurrencyString(std::string);
std::vector<unsigned char> from_json_to_bytes(std::string, std::string);

template<typename T>
void callfun(Serializer *, T, bool, int, int*);

#endif SERIALIZER_H
