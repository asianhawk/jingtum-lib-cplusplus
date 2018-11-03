#include "pch.h"
#include "stdafx.h"
#include "Serializer.h"

int fe[21] = { 0, (int)STInt16serialize, (int)STInt32serialize , (int)STInt64serialize, (int)STHash128serialize , (int)STHash256serialize , (int)STAmountserialize, (int)STVLserialize, (int)STAccountserialize, 0, 0, 0, 0, 0, (int)STObjectserialize, (int)STArrayserialize, (int)STInt8serialize, (int)STHash160serialize, (int)STPathSetserialize, (int)STVector256serialize, (int)STMemoserialize };
std::string stre[21] = { "", "STInt16serialize", "STInt32serialize", "STInt64serialize", "STHash128serialize", "STHash256serialize", "STAmountserialize", "STVLserialize", "STAccountserialize", "", "", "", "", "", "STObjectserialize", "STArrayserialize", "STInt8serialize", "STHash160serialize", "STPathSetserialize", "STVector256serialize", "STMemoserialize" };
boost::property_tree::ptree ifm = INVERSE_FIELDS_MAP();

Serializer::Serializer() {
	pointer = 0;
}

Serializer::Serializer(std::string str) {
	int half = str.length() / 2;
	Convert(&buffer, str);
	pointer = 0;  // ?? pointer = buffer.size()
}

Serializer::Serializer(std::vector<unsigned char> buf) {
	buffer = buf;
	pointer = 0;  // ?? pointer = buffer.size()
}

Serializer::~Serializer() {
}

void Serializer::append(std::vector<unsigned char> buf){
	buffer.insert(buffer.end(), buf.begin(), buf.end());
	pointer += buf.size();	
}

void Serializer::append(Serializer ser) {
	buffer.insert(buffer.end(), ser.buffer.begin(), ser.buffer.end());
	pointer += ser.buffer.size();
}

void Serializer::from_json(boost::property_tree::ptree ob) {
	buffer.clear();
	boost::property_tree::ptree obj(ob);
	int TT, LT;
	int flag;
	try {
		TT = obj.get<int>("TransactionType");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		try {
			obj.put_child("TransactionType",lookup_type_tx(TT));
		}
		catch (boost::property_tree::ptree_error ex) {	
			std::cout << ex.what() << "TransactionType ID is invalid!" << std::endl;
			exit(0);
		}
	}
	try {
		LT = obj.get<int>("LedgerEntryType");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1) {
		try {
			obj.put_child("LedgerEntryType", lookup_type_tx(LT));
		}
		catch (boost::property_tree::ptree_error ex) {
			std::cout << ex.what() << "LedgerEntryType ID is invalid!" << std::endl;
			exit(0);
		}
	}

	boost::property_tree::ptree td, ld;
	std::string tt, lt;
	int flag2, flag3;
	try {
		tt = obj.get<std::string>("TransactionType");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error&) { flag = 0; }
	if (flag == 1) {
		td = TRANSACTION_TYPES(base()).get_child(tt);
		try {
			if (td.size() <= 0) throw std::string("TransactionType is invalid");
		}
		catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
		
		boost::property_tree::ptree::iterator it = td.begin();
		obj.put("TransactionType", it->second.data());
	}
	else {
		try {
			lt = obj.get<std::string>("LedgerEntryType");
			flag2 = 1;
		}
		catch (boost::property_tree::ptree_error&) { flag2 = 0; }
		if (flag2 == 1) {
			ld = LEDGER_ENTRY_TYPES(sleBase()).get_child(lt);
			try{
				if (td.size() <= 0) throw std::string("LedgerEntryType is invalid");
			}
			catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
			boost::property_tree::ptree::iterator it = ld.begin();
			obj.put("LedgerEntryType", it->second.data());
		}
		else {
			try {
				boost::property_tree::ptree an = obj.get_child("AffectedNodes");
				flag3 = 1;
			}
			catch (boost::property_tree::ptree_error&) { flag3 = 0; }
			try {
				if (flag3 == 1) td = METADATA();
				else throw std::string("Object to be serialized must contain either 'TransactionType', LedgerEntryType or AffectedNodes");
			}
			catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
		}
	}
	
	serialize(this, td, obj);
	
}

void Serializer::serialize(Serializer *so, boost::property_tree::ptree td, boost::property_tree::ptree obj) {

	std::stringstream str;
	boost::property_tree::write_json(str, obj);
	STObjectserialize(so, str.str(), true);
}

std::string Serializer::hash(int prefix) {
	Serializer *sign_buffer = new Serializer;

	if (prefix != NAN) {
		STInt32serialize(sign_buffer, std::to_string(prefix), false);
	}

	sign_buffer->append(this->buffer);

	int len = sign_buffer->buffer.size();
	unsigned char *bu = new unsigned char[len];
	for (int i = 0; i < len; ++i) {
		bu[i] = sign_buffer->buffer[i];
	}
	unsigned char md[SHA512_DIGEST_LENGTH];
	SHA512(bu, len, md);
	std::string s="";
	char b[3] = { 0 };
	for (unsigned int i = 0; i < 32; ++i) {
		sprintf_s(b, "%02x", md[i]);
		s += b;
	}

	delete sign_buffer;

	return s;
}

std::string Serializer::to_hex() {
	std::string s = "";
	char b[3] = { 0 };
	for (unsigned int i = 0; i < buffer.size(); ++i) {
		sprintf_s(b, "%02x", buffer[i]);
		s += b;
	}

	return s;
}

boost::property_tree::ptree Serializer::lookup_type_tx(int id) {
	boost::property_tree::ptree tx;
	tx = TRANSACTION_TYPES(base()).get_child("Payment");
	return tx;
}

boost::property_tree::ptree Serializer::lookup_type_le(int id) {
	boost::property_tree::ptree tx;
	tx = TRANSACTION_TYPES(base()).get_child("Payment");
	return tx;
}

bool sort_field_compare(std::string a, std::string b) {
	boost::property_tree::ptree a_field_coordinates, b_field_coordinates;
	
	int a_type_bits, a_field_bits, b_type_bits, b_field_bits;	
	a_field_coordinates = ifm.get_child(a);
	boost::property_tree::ptree::iterator it = a_field_coordinates.begin();
	std::stringstream s1(it->second.data()); s1 >> a_type_bits; it++;
	std::stringstream s2(it->second.data()); s2 >> a_field_bits;
	b_field_coordinates = ifm.get_child(b);
	boost::property_tree::ptree::iterator it2 = b_field_coordinates.begin();
	std::stringstream s3(it2->second.data()); s3 >> b_type_bits; it2++;
	std::stringstream s4(it2->second.data()); s4 >> b_field_bits;

	int com = a_type_bits != b_type_bits ? a_type_bits - b_type_bits : a_field_bits - b_field_bits;

	if (com > 0) return false;
	else return true;

}

//void STObjectserialize(Serializer *so, boost::property_tree::ptree val, bool no_marker) {
void STObjectserialize(Serializer *so, std::string vals, bool no_marker) {

	//bool no_marker = true;

	std::vector<std::string> keys;

	std::stringstream s(vals);
	boost::property_tree::ptree val;
	try {
		boost::property_tree::read_json(s, val);
	}
	catch (boost::property_tree::ptree_error ex) { std::cout << ex.what() << std::endl; }

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, val) {
		char lo = v.first[0] + 32;
		if (v.first[0] == lo) {
			return;
		}
		try {
			int f;
			try {
				boost::property_tree::ptree tmp = INVERSE_FIELDS_MAP().get_child(v.first);
				f = 1;
			}
			catch (boost::property_tree::ptree_error ex) { f = 0; }
			if ( f == 0 ) throw std::string("JSON contains unknown firled:" + v.first);
		}
		catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
		keys.push_back(v.first);
	}

	std::sort(keys.begin(), keys.end(), sort_field_compare);

	for (unsigned int i = 0; i < keys.size(); ++i) {
		
		boost::property_tree::ptree t = val.get_child(keys[i]);
		std::stringstream str;
		int flag;
		try {
			boost::property_tree::write_json(str, t);
			flag = 1;
		}
		catch (boost::property_tree::ptree_error) { flag = 0; }
		
		if ( flag == 1)	serialize(so, keys[i], str.str());
		else serialize(so, keys[i], t.data());		
	}

	if (!no_marker) STInt8serialize(so, std::to_string(0xe1), no_marker);
}

void serialize(Serializer *so, std::string field_name, std::string valueS) {
	boost::property_tree::ptree field_coordinates = ifm.get_child(field_name);
	boost::property_tree::ptree::iterator it = field_coordinates.begin();
	int type_bits, field_bits;
	std::stringstream s1(it->second.data()); s1 >> type_bits; it++;
	std::stringstream s2(it->second.data()); s2 >> field_bits;	
	int tag_byte = (type_bits < 16 ? type_bits << 4 : 0) | (field_bits < 16 ? field_bits : 0);

	bool JSON = isJSON(valueS);	

	if (!JSON) {
		if (field_name == "LedgerEntryType") {
			valueS = std::to_string(get_ledger_entry_type(valueS));
		}
		else if (field_name == "TransactionResult") {
			valueS = std::to_string(get_transaction_type(valueS));
		}
	}

	STInt8serialize(so, std::to_string(tag_byte), false);

	if (type_bits >= 16) STInt8serialize(so, std::to_string(type_bits), false);
	if (field_bits >= 16) STInt8serialize(so, std::to_string(field_bits), false);

	std::string serialized_object_type;
	if (field_name == "Memo" && JSON == true) {
		serialized_object_type = "STMemoserialize";
	}
	else serialized_object_type = "ST"+TYPES_MAP[type_bits]+"serialize";
	
	int len = sizeof(TYPES_MAP) / sizeof(TYPES_MAP[0]);
	int index;
	for (int i = 0; i < len; ++i) {		
		if (serialized_object_type == stre[i]) {
			index = i;
			break;
		}
	}

	try {
		callfun(so, valueS, false, index, fe);
	}
	catch (std::exception ex) {
		std::cout << ex.what()+ std::string("(" + field_name + ")") << std::endl;
	}
}

int get_transaction_type(std::string value) {
	int output;
	try {
		if (value == "Payment") {
			output = 0;
		}
		else if (value == "AccountSet") {
			output = 3;
		}
		else if (value == "SetRegularKey") {
			output = 5;
		}
		else if (value == "OfferCreate") {
			output = 7;
		}
		else if (value == "OfferCancel") {
			output = 8;
		}
		else if (value == "Contract") {
			output = 9;
		}
		else if (value == "RemoveContract") {
			output = 10;
		}
		else if (value == "TrustSet") {
			output = 20;
		}
		else if (value == "RelationSet") {
			output = 21;
		}
		else if (value == "RelationDel") {
			output = 22;
		}
		else if (value == "ConfigContract") {
			output = 30;
		}
		else if (value == "EnableFeature") {
			output = 100;
		}
		else if (value == "SetFee") {
			output = 101;
		}
		else throw std::string("Invalid transaction type!");
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }

	std::cout << "Get tx types:" << output << std::endl;
	return output;
}

std::string get_transaction_type(int value) {
	std::string output;
	try {
		switch (value) {
		case 0: {
			output = "Payment";
			break;
		}
		case 3: {
			output = "AccountSet";
			break;
		}
		case 5: {
			output = "SetRegularKey";
			break;
		}
		case 7: {
			output = "OfferCreate";
			break;
		}
		case 8: {
			output = "OfferCancel";
			break;
		}
		case 9: {
			output = "Contract";
			break;
		}
		case 10: {
			output = "RemoveContract";
			break;
		}
		case 20: {
			output = "TrustSet";
			break;
		}
		case 21: {
			output = "RelationSet";
			break;
		}
		case 22: {
			output = "RelationDel";
			break;
		}
		case 30: {
			output = "ConfigContract";
			break;
		}
		case 100: {
			output = "EnableFeature";
			break;
		}
		case 101: {
			output = "SetFee";
			break;
		}
		default:
			throw std::string("Invalid transaction type!");
		}
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
	std::cout << "Get tx types:" << output << std::endl;
	return output;
}

int get_ledger_entry_type(std::string value) {
	int output;
	if (value == "Payment") {
		output = 0;
	}
	else if (value == "AccountRoot") {
		output = 97;
	}
	else if (value == "Contract") {
		output = 99;
	}
	else if (value == "DirectoryNode") {
		output = 100;
	}
	else if (value == "EnabledFeatures") {
		output = 102;
	}
	else if (value == "FeeSettings") {
		output = 115;
	}
	else if (value == "GeneratorMap") {
		output = 103;
	}
	else if (value == "LedgerHashes") {
		output = 104;
	}
	else if (value == "Nickname") {
		output = 110;
	}
	else if (value == "Offer") {
		output = 111;
	}
	else if (value == "SkywellState") {
		output = 114;
	}
	else output = 0;
	std::cout << "Get ledger entry type::" << output << std::endl;
	return output;
}

std::string get_ledger_entry_type(int value) {
	std::string output;

	try {
		switch (value) {
		case 97: {
			output = "AccountRoot";
			break;
		}
		case 99: {
			output = "Contract";
			break;
		}
		case 100: {
			output = "DirectoryNode";
			break;
		}
		case 102: {
			output = "EnabledFeatures";
			break;
		}
		case 115: {
			output = "FeeSettings";
			break;
		}
		case 103: {
			output = "GeneratorMap";
			break;
		}
		case 104: {
			output = "LedgerHashes";
			break;
		}
		case 110: {
			output = "Nickname";
			break;
		}
		case 111: {
			output = "Offer";
			break;
		}
		case 114: {
			output = "SkywellState";
			break;
		}
		default:
			throw std::string("Invalid input type for transaction result!");
		}
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
	std::cout << "Get ledger entry type::" << output << std::endl;
	return output;
}


std::vector<unsigned char> convertIntegerToByteArray(std::string val, int bytes) {
	
	int vald;
	try {
		if (!isNum(val)) throw std::string("Value is not a number") + val;		
		std::stringstream sin(val);
		sin >> vald;
		if (vald > INT_MAX) {
			throw std::string("Value is too large!");
		}
		if (vald < 0 || vald >= pow(256, bytes)) {
			throw std::string("Value out of bounds!");
		}
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }

	std::vector<unsigned char> newBytes;
	int i;
	for (i = 0; i < bytes; ++i) {
		newBytes.insert(newBytes.begin(), (vald >> (i * 8) & 0xff));
	}

	return newBytes;
}

void STInt8serialize(Serializer *so, std::string val, bool no_marker) {
	so->append(convertIntegerToByteArray(val, 1));
}

void STInt16serialize(Serializer *so, std::string val, bool no_marker) {
	so->append(convertIntegerToByteArray(val, 2));
}

void STInt32serialize(Serializer *so, std::string val, bool no_marker) {
	so->append(convertIntegerToByteArray(val, 4));
}

void serializeHex(Serializer *so, std::string hexData, bool noLength) {
	std::vector<unsigned char> byteData = convertHexToByteArray(hexData);
	if (!noLength) {
		serialize_varint(so, byteData.size());
	}
	so->append(byteData);
}

std::string convertStringtoHex(std::string s) {
	return StringToHex(string_To_UTF8(s));
}

void serialize_varint(Serializer *so, int val) {
	try {
		if (val < 0) throw std::string("Variable integers are unsigned.");
		if (val <= 192) {
			std::vector<unsigned char> t;
			t.push_back(val);
			so->append(t);
		}
		else if (val <= 12480) {
			std::vector<unsigned char> t;
			val -= 193;
			t.push_back(193 + (val >> 8));
			t.push_back(val & 0xff);
			so->append(t);
		}
		else if (val <= 918744) {
			std::vector<unsigned char> t;
			val -= 12481;
			t.push_back(241 + (val >> 16));
			t.push_back(val >> 8 & 0xff);
			t.push_back(val & 0xff);
			so->append(t);
		}
		else throw std::string("Variable integer overflow.");
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
}

void STInt64serialize(Serializer *so, std::string val, bool no_marker) {
	std::string bnihs;
	try {
		if (!isHexInt64String(val)) throw std::string("Not a valid hex Int64");
		else bnihs = val;
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }	

	while (bnihs.length() < 16) {
		bnihs = "0" + bnihs;
	}

	serializeHex(so, bnihs, true);
}

bool isHexInt64String(std::string val) {
	boost::regex hex_RE("^[0-9A-F]{0,16}$", boost::regex::icase);
	boost::cmatch what;
	return regex_match(val.c_str(), what, hex_RE );
}

void STHash128serialize(Serializer *so, std::string val, bool no_marker) {
	boost::regex hex_RE("^[0-9A-F]{0,16}$", boost::regex::icase);
	boost::cmatch what;
	bool flag = regex_match(val.c_str(), what, hex_RE);
	try {
		if (flag == true && val.length() <= 32) {
			serializeHex(so, val, true);
		}
		else throw std::string("Invalid Hash128");
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
}

void STHash256serialize(Serializer *so, std::string val, bool no_marker) {
	boost::regex hex_RE("^[0-9A-F]{0,16}$", boost::regex::icase);
	boost::cmatch what;
	bool flag = regex_match(val.c_str(), what, hex_RE);
	try {
		if (flag == true && val.length() <= 64) {
			serializeHex(so, val, true);
		}
		else throw std::string("Invalid Hash256");
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
}

void STHash160serialize(Serializer *so, std::string val, bool no_marker) {
	serializeHex(so, val, true);  
}

void STCurrencyserialize(Serializer *, std::string, bool no_marker) {
}

void STAmountserialize(Serializer *so, std::string val, bool no_marker) {
	Amount amount;
	amount.parse_json(val);
	try {
		if (!amount.is_valid()) throw std::string("Not a valid object!");
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }

	std::vector<unsigned char> valueBytes;
	for (int i = 0; i < 8; ++i) valueBytes.push_back('0');

	if (amount.is_native()) {
		unsigned long long bn = amount._value;
		std::stringstream str;
		str << std::hex << bn;
		std::string sbn = str.str();
		try {
			if (sbn.length() > 16) throw std::string("Amount value out of bounds.");
		}
		catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
		while (sbn.length() < 16) sbn = "0" + sbn;
		valueBytes = convertHexToByteArray(sbn);
		valueBytes[0] &= 0x3f;
		if (!amount.is_negative()) valueBytes[0] |= 0x40;

		so->append(valueBytes);
	}
	else {
		int hi = 0, lo = 0;
		hi |= 1 << 31;
		if (!amount.is_zero()) {
			if (!amount.is_negative()) hi |= 1 << 30;
			hi |= ((97 + amount._offset) & 0xff) << 22;
			hi |= amount._value >> 32 & 0x3fffff;
			lo = amount._value & 0xffffffff;

			int arr[2] = { hi, lo };
			int l = 2; // arr.length
			int x, bl, tmp;
			if (l == 0) bl = 0;
			else {
				x = arr[l - 1];
				int c = (int)round(x / 0x10000000000);
				bl = (l - 1) * 32 + (c == 0 ? 32 : c);
			}

			std::vector<unsigned char> tmparray;
			for (int i = 0; i < bl / 8; ++i)
			{
				int t = (int)round(i / 4);
				if ((i & 3) == 0) tmp = arr[t];
				tmparray.push_back(tmp >> 24 % 256);
				//tmparray.push_back(tmp >> 24 % 256);
				tmp <<= 8;
			}
			try {
				if (tmparray.size() > 8) throw std::string("invalid byte array length in AMOUNT value representation");
			}
			catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
			valueBytes = tmparray;

			so->append(valueBytes);
			std::vector<unsigned char> tum_bytes = amount.tum_to_bytes();

			so->append(tum_bytes);
			unsigned char *s = convertAddressToBytes(amount.issuer());
			std::vector<unsigned char> iss;
			int lenofs = 20;
			for (int i = 0; i < lenofs; ++i)
				iss.push_back(s[i]);
			so->append(iss);
		}
	}
}

void STVLserialize(Serializer *so, std::string val, bool no_marker) {
	serializeHex(so, val, false);
}

void STAccountserialize(Serializer *so, std::string val, bool no_marker) {
	unsigned char *s = convertAddressToBytes(val);
	int lenofs = 20;  
	serialize_varint(so, lenofs);  
	std::vector<unsigned char> buf;
	for (int i = 0; i < lenofs; ++i)
		buf.push_back(s[i]);
	so->append(buf);
}

void STArrayserialize(Serializer *so, std::string value, bool no_marker) {
	std::stringstream str(value);
	boost::property_tree::ptree val;
	boost::property_tree::read_json(str, val);

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, val) {
		std::vector<std::string> keys;
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v1, v.second) {
			keys.push_back(v1.first);
		}
		try {
			if (keys.size() != 1) {
				throw std::string("Cannot serialize an array containing non-single-key objects");
			}
		}
		catch (std::string ex) { std::cout << ex << std::endl; exit(0); }

		std::string field_name = keys[0];
		boost::property_tree::ptree value = v.second.get_child(field_name);
		std::stringstream str;
		int flag;
		try {
			boost::property_tree::write_json(str, value);
			flag = 1;
		}
		catch (boost::property_tree::ptree ex) { flag = 0; }
		if ( flag == 1)	serialize(so, field_name, str.str());
		else serialize(so, field_name, value.data());		
	}	

	STInt8serialize(so, std::to_string(0xF1), no_marker);
}

void STPathSetserialize(Serializer *so, std::string value, bool no_marker) {
	int typeBoundary = 0xff;
	int typeEnd = 0x00;
	int typeAccount = 0x01;
	int typeCurrency = 0x10;
	int typeIssuer = 0x20;

	std::stringstream str(value);
	boost::property_tree::ptree val;
	boost::property_tree::read_json(str, val);

	int i = 0;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, val) {
		if (i) STInt8serialize(so, std::to_string(typeBoundary), no_marker);

		BOOST_FOREACH(boost::property_tree::ptree::value_type &v1, v.second) {
			boost::property_tree::ptree entry = v1.second;
			int type = 0;

			std::string account, cur, issuer;
			int f1, f2, f3;
			try {
				account = entry.get<std::string>("account");
				f1 = 1;
			}
			catch (boost::property_tree::ptree_error ex) { f1 = 0; }
			if (f1 == 1) type |= typeAccount;
			try {
				cur = entry.get<std::string>("currency");
				f2 = 1;
			}
			catch (boost::property_tree::ptree_error ex) { f2 = 0; }
			if (f2 == 1) type |= typeCurrency;
			try {
				issuer = entry.get<std::string>("issuer");
				f3 = 1;
			}
			catch (boost::property_tree::ptree_error ex) { f3 = 0; }
			if (f3 == 1) type |= typeIssuer;

			STInt8serialize(so, std::to_string(type), no_marker);
			if (f1 == 1) {
				unsigned char *s = convertAddressToBytes(account);
				int lenofs = 20;
				std::vector<unsigned char> buf;
				for (int k = 0; k < lenofs; ++k)
					buf[k] = s[k];
				so->append(buf);
			}
			if (f2 == 1) {
				std::vector<unsigned char> currencyBytes = from_json_to_bytes(cur, entry.get<std::string>("non_native"));
			}
			if (f3 == 1) {
				unsigned char *s = convertAddressToBytes(issuer);
				int lenofs = 20;
				std::vector<unsigned char> buf;
				for (int k = 0; k < lenofs; ++k)
					buf[k] = s[k];
				so->append(buf);
			}
		}
		i++;
	}	

	STInt8serialize(so, std::to_string(typeEnd),no_marker);
}

void STVector256serialize(Serializer *so, std::string value, bool no_marker) {
	std::stringstream str(value);
	boost::property_tree::ptree val;
	boost::property_tree::read_json(str, val);
	int len = 0;
	for (auto item : val) {
		len++;
	}
	serialize_varint(so, len * 32);
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, val) {
		STHash256serialize(so, v.second.data(), no_marker);
	}	
}

void STMemoserialize(Serializer *so, std::string vals, bool no_marker) {
	//bool no_marker = false;

	std::stringstream str(vals);
	boost::property_tree::ptree val;
	boost::property_tree::read_json(str, val);

	std::vector<std::string> keys;

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, val) {
		char lo = v.first[0] + 32;
		if (v.first[0] == lo) {
			return;
		}
		try {
			boost::property_tree::ptree tmp = INVERSE_FIELDS_MAP().get_child(v.first);
		}
		catch (std::exception ex) { std::cout << ex.what()+ std::string("JSON contains unknown field" + v.first) << std::endl; exit(0); }
		keys.push_back(v.first);
	}
		
	std::sort(keys.begin(), keys.end(), sort_field_compare);

	bool isJson;
	int flag;
	std::string mf;
	try {
		mf = val.get<std::string>("MemoFormat");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; };
	if (flag == 1 && mf == "json") isJson = true;

	for (unsigned int i = 0; i < keys.size(); ++i) {
		std::string key = keys[i];
		std::string value;
		boost::property_tree::ptree Value = val.get_child(key);
		if (Value.size() == 0) {
			value = val.get<std::string>(key);
			if (key == "MemoType" || key == "MemoFormat" ||  key == "MemoData") {
				//value = convertStringtoHex(value);
				value = value; 
			}
		}
		else if ( Value.size() > 0 && key == "MemoData"){
			try {
				if (isJson) {
					try {
						std::stringstream ss;
						boost::property_tree::write_json(ss, Value);
						//value = convertStringtoHex(getPureStr(ss.str()));
						value = ss.str();
					}
					catch (std::exception ex) { std::cout << ex.what() + std::string("MemoFormat json with invalid JSON in MemoData field") << std::endl; exit(0); }
				}
				else throw "MemoFormat can only be a JSON object with a valid json MemoFormat.";
			}
			catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
		}
		serialize(so, key, value);
	}

	if (!no_marker) {
		STInt8serialize(so, std::to_string(0xe1), no_marker);
	}

}

bool isHexString(std::string s) {
	boost::regex hex_RE("^[0-9A-F]+$", boost::regex::icase);
	boost::cmatch what;
	return regex_match(s.c_str(), what, hex_RE);
}

bool isCurrencyString(std::string s) {
	boost::regex hex_RE("^[A-Z0-9]{3}$");
	boost::cmatch what;
	return regex_match(s.c_str(), what, hex_RE);
}

std::vector<unsigned char> from_json_to_bytes(std::string j, std::string shouldInterpretSWT) {

	std::vector<unsigned char> val;
	for (int i = 0; i < 20; ++i) val.push_back('0');
	if (isHexString(j) && j.length() == 40) val = convertHexToByteArray(j);
	else if (isCurrencyString(j)){
		std::string currencyCode = j;
		if (currencyCode.length() >= CURRENCY_NAME_LEN && currencyCode.length() <= CURRENCY_NAME_LEN2) {
			int end = 14;
			int len = currencyCode.length() - 1;
			for (int x = len; x >= 0; x--) {
				char c = currencyCode[len - x];
				val[end - x] = (int)c & 0xff;
			}
		}
	}

	return val;
}

template<typename T>
void callfun(Serializer *so, T val, bool no_marker,int index, int *t) {
	((void(*)(Serializer *, T, bool))t[index])(so, val, no_marker);
}