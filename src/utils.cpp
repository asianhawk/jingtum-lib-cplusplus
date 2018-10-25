#include "pch.h"
#include "utils.h"

std::string HexToString(std::string h) {
	int i = 0;
	//int index = 0;
	int len = h.length();
	const char *st = h.c_str();
	char a;
	std::string s;

	if (len % 2) {
		a = hex2int(st[0]);
		//s.append(1,a);
		s += a;
		i = 1;
		//index = 1;
	}
	for (; i < len; i += 2) {
		a = hex2int(st[i]) * 16 + hex2int(st[i + 1]);
		//s.append(1,a);
		s += a;
		//index++;
	}

	return s;
}

std::string StringToHex(std::string h) {
	std::string s="";
	char b[8] = {0};
	for (unsigned int i=0; i < h.length(); ++i) {		
		sprintf_s(b, "%.2x",(unsigned char)h[i]);
		s += b;
	}

	return s;
}

std::string formatArgs(boost::property_tree::ptree args) {
	std::string newArgs = "[";
	unsigned int index = 1;
	if (args.size() != 0) {
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, args) {
			boost::property_tree::ptree s = v.second;
			newArgs.append(HexToString(v.second.get_child("Args").get<std::string>("Parameter")));
			if ( index < args.size() ) newArgs.append(1, ',');
			index++;
		}
	}
	newArgs.append(1, ']');

	return newArgs;
}

std::string UTF8_To_string(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t * pwBuf = new wchar_t[nwLen + 1]; 
	memset(pwBuf, 0, nwLen * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr = pBuf;
	delete[]pBuf;
	delete[]pwBuf;
	pBuf = NULL;
	pwBuf = NULL;
	return retStr;	
}

std::string string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);
	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr(pBuf);
	delete[]pwBuf;
	delete[]pBuf;
	pwBuf = NULL;
	pBuf = NULL;
	return retStr;
}

std::vector<unsigned char> convertHexToByteArray(std::string str) {
	if (str.length() % 2 != 0) str += "0";
	std::vector<unsigned char> dst;
	unsigned int i = 0;
	while (i < str.length())
	{
		dst.push_back(hex2int(str[i]) * 16 + hex2int(str[i + 1]));
		i += 2;
	}

	return dst;
}

std::string getPureStr(std::string str) {
	int index = 0;
	if (!str.empty())
	{
		while ((index = str.find(' ', index)) != std::string::npos)
		{
			str.erase(index, 1);
		}
	}
	index = 0;
	if (!str.empty())
	{
		while ((index = str.find('\n', index)) != std::string::npos)
		{
			str.erase(index, 1);
		}
	}
	return str;
}

boost::property_tree::ptree extendThree(boost::property_tree::ptree A, boost::property_tree::ptree B, boost::property_tree::ptree C) {
	int size = A.size() + B.size() + C.size();
	std::string *key = new std::string[size];
	std::string *val = new std::string[size];
	int *syl = new int[size];
	int index = 0;
	if (C.size() != 0) {
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, C) {
			key[index] = v.first;			
			try {
				std::stringstream t;
				boost::property_tree::write_json(t, v.second);
				val[index] = t.str();
				syl[index] = 1;
			}
			catch (boost::property_tree::ptree_error ex) {
				val[index] = v.second.data();
				syl[index] = 0;
			}			
			index++;
		}
	}
	if (B.size() != 0) {
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, B) {
			key[index] = v.first;
			try {
				std::stringstream t;
				boost::property_tree::write_json(t, v.second);
				val[index] = t.str();
				syl[index] = 1;
			}
			catch (boost::property_tree::ptree_error ex) {
				val[index] = v.second.data();
				syl[index] = 0;
			}
			index++;
		}
	}
	if (A.size() != 0) {
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, A) {
			key[index] = v.first; 
			try {
				std::stringstream t;
				boost::property_tree::write_json(t, v.second);
				val[index] = t.str();
				syl[index] = 1;
			}
			catch (boost::property_tree::ptree_error ex) {
				val[index] = v.second.data();
				syl[index] = 0;
			}
			index++;
		}
	}

	std::string tol = "{";
	unsigned int i, j;
	int f1, f2;
	for (i = 0; i < C.size(); ++i) {
		tol = tol + "\""+ key[i] +"\":\"" + val[i] + "\"," ;
		if (syl[i ] == 0) tol = tol + "\"" + key[i] + "\":\"" + val[i] + "\",";
		else if (syl[i] == 1) tol = tol + "\"" + key[i] + "\":" + val[i ] + ",";
	}
	for (i = 0; i < B.size(); ++i) {
		f1 = 0;
		for (j = 0; j < C.size(); ++j) {
			if (key[i + C.size()] == key[j]) {
				f1 = 1;
				continue;
			}
		}
		if (f1 == 0) {
			if (syl[i + C.size()] == 0) tol = tol + "\"" + key[i + C.size()] + "\":\"" + val[i + C.size()] + "\",";
			else if ( syl[i+C.size()] == 1) tol = tol + "\"" + key[i + C.size()] + "\":" + val[i + C.size()] + ",";
			
		}
	}
	for (i = 0; i < A.size(); ++i) {
		f1 = 0;
		for (j = 0; j < C.size(); ++j) {
			if (key[i + C.size()+B.size()] == key[j]) {
				f1 = 1;
				continue;
			}
		}
		f2 = 0;
		for (j = 0; j < B.size(); ++j) {
			if (key[i + C.size() + B.size()] == key[j+C.size()]) {
				f2 = 1;
				continue;
			}
		}
		if (f1 == 0 && f2 == 0) {
			if (syl[i + C.size()+B.size()] == 0) tol = tol + "\"" + key[i + C.size() + B.size()] + "\":\"" + val[i + C.size() + B.size()] + "\",";
			else if (syl[i + C.size() + B.size()] == 1) tol = tol + "\"" + key[i + C.size() + B.size()] + "\":" + val[i + C.size() + B.size()] + ",";
		}
	}
	tol = tol.substr(0, tol.length() - 1) + "}";
	std::stringstream str(tol);
	boost::property_tree::ptree pt;
	boost::property_tree::read_json(str, pt);

	delete[] key;
	delete[] val;
	delete[] syl;

	return pt;
}

boost::property_tree::ptree processAffectNode(boost::property_tree::ptree an) {
	boost::property_tree::ptree result;
	result.put_value("");

	std::string TS[3] = { "CreatedNode", "ModifiedNode", "DeletedNode" };
	int flag = 0;
	for (auto it : TS) {
		try {
			boost::property_tree::ptree tmp;
			tmp = an.get_child(it);
			flag = 1;
		}
		catch (boost::property_tree::ptree_error ex) { flag = 0; }
		if (flag == 1) {
			result.put("diffType", it);
			break;
		}
	}

	if (flag == 0) return result;
	an = an.get_child(result.get<std::string>("diffType"));
	result.put("entryType", an.get<std::string>("LedgerEntryType"));
	result.put("ledgerIndex", an.get<std::string>("LedgerIndex"));
	
	boost::property_tree::ptree PreviousFields;
	boost::property_tree::ptree NewFields;
	boost::property_tree::ptree FinalFields;
	boost::property_tree::ptree blank;
	blank.put_value("");
	try {
		PreviousFields = an.get_child("PreviousFields");
	}
	catch (boost::property_tree::ptree_error ex) { PreviousFields = blank; }
	result.put_child("fieldPrev", PreviousFields);
	try {
		NewFields = an.get_child("NewFields");
	}
	catch (boost::property_tree::ptree_error ex) { NewFields = blank; }
	result.put_child("fieldsNew", NewFields);
	try {
		FinalFields = an.get_child("FinalFields");
	}
	catch (boost::property_tree::ptree_error ex) { FinalFields = blank; }
	result.put_child("fieldsFinal", FinalFields);

	result.put_child("fields", extendThree(PreviousFields, NewFields, FinalFields));

	try {
		result.put("PreviousTxnID", an.get<std::string>("PreviousTxnID"));
	}
	catch (boost::property_tree::ptree_error ex) {}	

	return result;
}

bool isNum(std::string str) {
	std::stringstream sin(str);
	double d;
	char c;
	if (!(sin >> d)) return false;
	if (sin >> c) return false;
	return true;
}

bool isInt(std::string str) {
	if (isNum(str)) {
		if (str.find(".") == std::string::npos) return true;
		else return false;
	}
	return false;
}

bool isJSON(std::string s) {
	if (s == "") return false;
	if (s.length() <= 2) return false;
	std::string::size_type le = s.find("{");
	std::string::size_type ri = s.find("}");
	if (le == std::string::npos || ri == std::string::npos) return false;

	std::stringstream str(s);
	boost::property_tree::ptree J;
	try {
		boost::property_tree::read_json(str, J);
		return true;
	}
	catch (boost::property_tree::ptree_error ex) { return false; }
}

bool isValidHash(std::string hash) {
	if (hash == "") {
		return false;
	}
	boost::regex HASH_RE("^[A-F0-9]{64}$");
	boost::cmatch what;	
	return regex_match(hash.c_str(), what, HASH_RE);	
}

bool isValidAddress(std::string addr) {
	return checkAddress(addr);
}

bool isValidSecret(std::string secret) {
	try {
		unsigned char *pubkeychar;
		unsigned char prikey[32];
		std::string PrivateKey;
		std::string PublicKey;
		pubkeychar = deriveKeyPair(secret, &PublicKey, &PrivateKey, prikey);
		return true;
	}
	catch (std::exception ex) {
		return false;
	}
}

bool isValidCurrency(std::string cur) {
	if (cur == "") {
		return false;
	}
	boost::regex CURRENCY_RE("^([a-zA-Z0-9]{3,6}|[A-F0-9]{40})$");
	boost::cmatch what;
	return regex_match(cur.c_str(), what, CURRENCY_RE);
}

bool isValidAmount(boost::property_tree::ptree Amount) {
	double val;
	int flag = 0;
	try {
		val = Amount.get<double>("value");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 0 || val == NAN) return false;

	std::string cur;
	try {
		cur = Amount.get<std::string>("currency");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 0 || !isValidCurrency(cur)) return false;
	if (Amount.get<std::string>("currency") == currency && Amount.get<std::string>("issuer") != "") return false;
	if (Amount.get<std::string>("currency") != currency && !isValidAddress(Amount.get<std::string>("issuer"))) return false;
	return true;
	
}

bool isValidAmount0(boost::property_tree::ptree amount) {
	std::string cur;
	int f1, f2;
	try {
		cur = amount.get<std::string>("currency");
		f1 = 1;
	}
	catch (boost::property_tree::ptree_error ex) { f1 = 0; }
	if (f1 == 1) {
		if (!isValidCurrency(cur)) return false;
	}
	std::string issuer;
	try {
		issuer = amount.get<std::string>("issuer");
		f2 = 1;
	}
	catch (boost::property_tree::ptree_error ex) { f2 = 0; }
	if (f1 == 1 && f2 == 1) {
		if (cur == currency && issuer != "") return false;
		if (cur != currency && !isValidAddress(issuer)) return false;
	}
	return true;
}

boost::property_tree::ptree LEDGER_FLAGS() {
	boost::property_tree::ptree obj;

	boost::property_tree::ptree account_root;
	account_root.put("PasswordSpent",  0x00010000);
	account_root.put("RequireDestTag", 0x00020000);
	account_root.put("RequireAuth",    0x00040000);
	account_root.put("DisallowSWT",    0x00080000);
	account_root.put("DisableMaster",  0x00100000);
	obj.put_child("account_root", account_root);

	boost::property_tree::ptree offer;
	offer.put("Passive", 0x00010000);
	offer.put("Sell",    0x00020000);
	obj.put_child("offer", offer);

	boost::property_tree::ptree state;
	state.put("LowReserve", 0x00010000);
	state.put("HighReserve", 0x00020000);
	state.put("LowAuth", 0x00040000);
	state.put("HighAuth", 0x00080000);
	state.put("LowNoSkywell", 0x00100000);
	state.put("HighNoSkywell", 0x00200000);
	obj.put_child("state", state);

	return obj;
}

boost::property_tree::ptree TransactionSetClearFlags() {
	boost::property_tree::ptree obj;
	boost::property_tree::ptree AccountSet;
	AccountSet.put("asfRequireDest", 1);
	AccountSet.put("asfRequireAuth", 2);
	AccountSet.put("asfDisallowSWT", 3);
	AccountSet.put("asfDisableMaster", 4);
	AccountSet.put("asfNoFreeze", 6);
	AccountSet.put("asfGlobalFreeze", 7);
	obj.put_child("AccountSet",AccountSet);

	return obj;
}

boost::property_tree::ptree TransactionFlags() {
	boost::property_tree::ptree obj;

	boost::property_tree::ptree Universal;
	Universal.put("FullyCanonicalSig", 0x80000000);
	obj.put_child("Universal", Universal);
	
	boost::property_tree::ptree AccountSet;
	AccountSet.put("RequireDestTag",  0x00010000);
	AccountSet.put("OptionalDestTag", 0x00020000);
	AccountSet.put("RequireAuth",     0x00040000);
	AccountSet.put("OptionalAuth",    0x00080000);
	AccountSet.put("DisallowSWT",     0x00100000);
	AccountSet.put("AllowSWT",        0x00200000);
	obj.put_child("AccountSet", AccountSet);
	
	boost::property_tree::ptree TrustSet;
	TrustSet.put("SetAuth",        0x00010000);
	TrustSet.put("NoSkywell",      0x00020000);
	TrustSet.put("SetNoSkywell",   0x00040000);
	TrustSet.put("ClearNoSkywell", 0x00080000);
	TrustSet.put("SetFreeze",      0x00100000);
	TrustSet.put("ClearFreeze",    0x00200000);
	obj.put_child("TrustSet", TrustSet);

	boost::property_tree::ptree OfferCreate;
	OfferCreate.put("Passive",           0x00010000);
	OfferCreate.put("ImmediateOrCancel", 0x00020000);
	OfferCreate.put("FillOrKill",        0x00040000);
	OfferCreate.put("Sell",              0x00080000);
	obj.put_child("OfferCreate", OfferCreate);

	boost::property_tree::ptree Payment;
	Payment.put("NoSkywellDirect", 0x00010000);
	Payment.put("PartialPayment",  0x00020000);
	Payment.put("LimitQuality",    0x00040000);
	obj.put_child("Payment", Payment);

	boost::property_tree::ptree RelationSet;
	RelationSet.put("Authorize", 0x00000001);
	RelationSet.put("Freeze",    0x00000011);
	obj.put_child("RelationSet", RelationSet);

	return obj;
}

boost::property_tree::ptree parseAmount(boost::property_tree::ptree tx, std::string str) {
	boost::property_tree::ptree Sym;
	float sym;
	int flag;
	Sym = tx.get_child(str);
	if (Sym.size() == 0) {
		sym = tx.get<float>(str);
		flag = 1;
	}
	else flag = 0;
	if ( flag == 0 ) {
		if (isValidAmount(Sym)) {
			return Sym;
		}
		else {
			boost::property_tree::ptree obj;
			std::stringstream str_stream("{}");
			boost::property_tree::read_json(str_stream, obj);
			return obj;
		}
	}
	else {
		boost::property_tree::ptree obj;
		if ((sym != NAN)) {
			BN_CTX * ctx = BN_CTX_new();
			BIGNUM * bn = BN_new();
			BIGNUM * di = BN_new();
			BIGNUM * dv = BN_new();
			BIGNUM * re = BN_new();
			char * DV;
			BN_dec2bn(&bn, std::to_string(sym).c_str());
			BN_dec2bn(&di, "1000000.0");
			BN_div(dv, re, bn, di, ctx);
			DV = BN_bn2dec(dv);
			std::string s(DV);

			BN_clear_free(bn);
			BN_clear_free(di);
			BN_clear_free(dv);
			BN_clear_free(re);
			BN_CTX_free(ctx);

			obj.put("value", s);
			obj.put("currency", currency);
			obj.put("issuer", "");

		}
		else {
			std::stringstream str_stream("{}");
			boost::property_tree::read_json(str_stream, obj);
		}

		return obj;
	}
	
}

bool isAmountZero(boost::property_tree::ptree amount) {
	if (amount.size() == 0) return false;
	return amount.get<int>("value") < 1e-12;
}

boost::property_tree::ptree AmountNegate(boost::property_tree::ptree amount) {
	if (amount.size() == 0) return amount;

	BN_CTX * ctx = BN_CTX_new();
	BIGNUM * bn = BN_new();
	BIGNUM * pl = BN_new();
	BIGNUM * mm = BN_new();
	char * DV;
	BN_dec2bn(&bn, std::to_string(amount.get<int>("value")).c_str());
	BN_dec2bn(&pl, "-1");
	BN_mul(mm, bn, pl, ctx);
	DV = BN_bn2dec(mm);
	std::string s(DV);
	BN_clear_free(bn);
	BN_clear_free(pl);
	BN_clear_free(mm);
	BN_CTX_free(ctx);

	boost::property_tree::ptree obj;
	obj.put("value", s);
	obj.put("currency", amount.get<std::string>("currency"));
	obj.put("issuer", amount.get<std::string>("issuer"));

	return obj;
}

boost::property_tree::ptree AmountAdd(boost::property_tree::ptree amount1, boost::property_tree::ptree amount2) {
	if (amount1.size() == 0) return amount2;
	if (amount2.size() == 0) return amount1;
	boost::property_tree::ptree obj;
	if (amount1.size() != 0 && amount2.size() != 0) {
		BN_CTX * ctx = BN_CTX_new();
		BIGNUM * bn = BN_new();
		BIGNUM * pl = BN_new();
		BIGNUM * mm = BN_new();
		char * DV;
		BN_dec2bn(&bn, std::to_string(amount1.get<int>("value")).c_str());
		BN_dec2bn(&pl, std::to_string(amount1.get<int>("value")).c_str());
		BN_mul(mm, bn, pl, ctx);
		DV = BN_bn2dec(mm);
		std::string s(DV);
		BN_clear_free(bn);
		BN_clear_free(pl);
		BN_clear_free(mm);
		BN_CTX_free(ctx);		
		obj.put("value", s);
		obj.put("currency", amount1.get<std::string>("currency"));
		obj.put("issuer", amount1.get<std::string>("issuer"));
	}
	else {
		std::stringstream str_stream("{}");
		boost::property_tree::read_json(str_stream, obj);
	}

	return obj;
}		

boost::property_tree::ptree AmountSubtract(boost::property_tree::ptree amount1, boost::property_tree::ptree amount2) {
	return AmountAdd(amount1, AmountNegate(amount2));
}

std::string AmountRatio(boost::property_tree::ptree amount1, boost::property_tree::ptree amount2) {
	BN_CTX * ctx = BN_CTX_new();
	BIGNUM * bn = BN_new();
	BIGNUM * di = BN_new();
	BIGNUM * dv = BN_new();
	BIGNUM * re = BN_new();
	char * DV;
	BN_dec2bn(&bn, std::to_string(amount1.get<int>("value")).c_str());
	BN_dec2bn(&di, std::to_string(amount2.get<int>("value")).c_str());
	BN_div(dv, re, bn, di, ctx);
	DV = BN_bn2dec(dv);
	std::string s(DV);

	BN_clear_free(bn);
	BN_clear_free(di);
	BN_clear_free(dv);
	BN_clear_free(re);
	BN_CTX_free(ctx);

	return s;
}

std::string getPrice(boost::property_tree::ptree effect, bool funded) {
	boost::property_tree::ptree g, p;
	try {
		g = effect.get_child("got");
	}
	catch (boost::property_tree::ptree_error ex) {
		g = effect.get_child("pays");
	}
	try {
		p = effect.get_child("paid");
	}
	catch (boost::property_tree::ptree_error ex) {
		p = effect.get_child("gets");
	}
	if (!funded) {
		return AmountRatio(g, p);
	}
	else {
		return AmountRatio(p, g);
	}
}

boost::property_tree::ptree reverseAmount(boost::property_tree::ptree amount, std::string account) {
	boost::property_tree::ptree obj;
	obj.put("value", std::to_string(-amount.get<double>("value")));
	obj.put("currency", amount.get<std::string>("currency"));
	obj.put("issuer", account);
	return obj;
}

std::string txnType(boost::property_tree::ptree tx, std::string account) {
	int f1 = 0, f2 = 0, f3 = 0, f4 = 0;
	try {
		if ( tx.get<std::string>("Account") == account ) f1 = 1;
	}
	catch (boost::property_tree::ptree_error ex) { f1 = 0; }
	try {
		if (tx.get<std::string>("Target") == account) f2 = 1;
	}
	catch (boost::property_tree::ptree_error ex) { f2 = 0; }
	try {
		if (tx.get<std::string>("Destination") == account) f3 = 1;
	}
	catch (boost::property_tree::ptree_error ex) { f3 = 0; }
	boost::property_tree::ptree t;
	try {		
		t = tx.get_child("LimitAmount");
		f4 = 1;
	}catch (boost::property_tree::ptree_error ex) { f4 = 0; }
	if ( f4 == 1 ) {
		try {
			if (t.get<std::string>("issuer") == account) { f4 = 1; }
		}
		catch (boost::property_tree::ptree_error ex) { f4 = 0; }
	}
	if (f1 || f2 || f3 || f4) {
		std::string type = tx.get<std::string>("TransactionType");
		if (type == "Payment") {
			return f1 ? f3 ? "convert" : "sent" : "received";
		}
		else if (type == "OfferCreate"){
			return "offernew";
		}
		else if (type == "TrustSet") {
			return f1 ? "trusting" : "trusted";
		}
		else if (type == "RelationDel" || type == "AccountSet" || type == "SetRegularKey" || type == "RelationSet" || type == "SingSet" || type == "Operation" || type == "ConfigContract") {			
			std::transform(type.begin(), type.end(), type.begin(), ::tolower);
			return type;
		}
		else return "unknown";
	}
	else
	{
		return "offereffect";
	}
	
}

boost::property_tree::ptree processTx(boost::property_tree::ptree txn, std::string account) {
	boost::property_tree::ptree tx;
	int flag = 0;
	try {
		tx = txn.get_child("tx");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 0) {
		try {
			tx = txn.get_child("transaction");
			flag = 1;
		}
		catch(boost::property_tree::ptree_error ex) {  }
		if (flag == 0) {
			tx = txn;
		}
	}
	boost::property_tree::ptree result;

	boost::property_tree::ptree meta;
	bool metaexist;
	try {
		meta = txn.get_child("meta");
		metaexist = true;
	}
	catch (boost::property_tree::ptree_error ex) { metaexist = false; };
	
	int date;
	flag = 0;
	try {
		date = tx.get<int>("date") + 0x386D4380;
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 0) { 
		try {
			date = tx.get<int>("Timestamp") + 0x386D4380;
		}
		catch(boost::property_tree::ptree_error ex) { flag = 0; }
	}
	result.put("date", date);
	result.put("hash", tx.get<std::string>("hash"));
	std::string type = txnType(tx, account);
	result.put("type", type );
	result.put("fee", std::to_string(tx.get<float>("Fee")/1000000.0));
	result.put("result", metaexist ? meta.get<std::string>("TransactionResult") : "failed");
	
	if (type == "sent") {
		result.put("counterparty", tx.get<std::string>("Destination"));
		result.put_child("amount", parseAmount(tx,"Amount"));
	}
	else if (type == "received") {
		result.put("counterparty", tx.get<std::string>("Account"));
		result.put_child("amount", parseAmount(tx, "Amount"));
	}
	else if (type == "trusted") {
		result.put("counterparty", tx.get<std::string>("Account"));
		result.put_child("amount", reverseAmount(tx.get_child("LimitAmount"), tx.get<std::string>("Account")));
	}
	else if (type == "trusting") {
		result.put("counterparty", tx.get_child("LimitAmount").get<std::string>("issuer"));
		result.put_child("amount", tx.get_child("LimitAmount"));
	}
	else if (type == "convert") {
		result.put_child("spent", parseAmount(tx, "SendMax"));
		result.put_child("amount", parseAmount(tx, "Amount"));
	}
	else if (type == "offernew") {
		result.put("offertype", tx.get<int>("Flags") & TransactionFlags().get_child("OfferCreate").get<int>("Sell") ? "sell" :"buy");
		result.put_child("gets", parseAmount(tx, "TakerGets"));
		result.put_child("pays", parseAmount(tx, "TakerPays"));
		result.put("seq", tx.get<std::string>("Sequence"));
	}
	else if (type == "offercancel") {
		result.put("offerseq", tx.get<std::string>("Sequence"));
	}
	else if (type == "relationset") {
		result.put("counterparty", account == tx.get<std::string>("Target") ? tx.get<std::string>("Account") : tx.get<std::string>("Target"));
		result.put("relationtype", tx.get<int>("RelationType") == 3 ? "freeze" : "authorize");
		result.put("isactive", account == tx.get<std::string>("Target") ? false : true);
		result.put_child("amount", parseAmount(tx, "LimitAmount"));
	}
	else if (type == "relationdel") {
		result.put("counterparty", account == tx.get<std::string>("Target") ? tx.get<std::string>("Account") : tx.get<std::string>("Target"));
		result.put("relationtype", tx.get<int>("RelationType") == 3 ? "unfreeze" : "unknown");
		result.put("isactive", account == tx.get<std::string>("Target") ? false : true);
		result.put_child("amount", parseAmount(tx, "LimitAmount"));
	}
	else if (type == "configcontract") {
		result.put("params", formatArgs(tx.get_child("Args")));
		int m = tx.get<int>("Method");
		if (m == 0) {
			result.put("method", "deploy");
			result.put("payload", tx.get<std::string>("Payload"));
		}
		else if (m == 1) {
			result.put("method", "call");
			result.put("destination", tx.get<std::string>("Destination"));
		}
	}

	boost::property_tree::ptree Memo;
	boost::property_tree::ptree me;
	flag = 0;
	try {
		Memo=tx.get_child("Memos");
		flag = 1;
	}
	catch (boost::property_tree::ptree_error ex) { flag = 0; }
	if (flag == 1 && Memo.size() > 0) {
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, Memo) {
			boost::property_tree::ptree memo = v.second.get_child("Memo");
			for (auto item : memo) {
				try {
					memo.put(item.first, UTF8_To_string(HexToString(item.second.data())));
				}
				catch (boost::property_tree::ptree_error ex) {}
			}
			me.push_back(std::make_pair("", memo));
		}
		result.put_child("memos", me);
	}
	
	boost::property_tree::ptree blank;
	boost::property_tree::ptree bc;
	bc.put_value("");
	blank.push_back(std::make_pair("", bc));
	result.put_child("effects", blank);
	if (metaexist = false || meta.get<std::string>("TransactionResult") != "tesSUCCESS") {
		return result;
	}

	boost::property_tree::ptree AffectedNodes = meta.get_child("AffectedNodes");
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, AffectedNodes) {
		boost::property_tree::ptree node = processAffectNode(v.second);
		boost::property_tree::ptree effect;
		std::stringstream str_stream("{}");
		boost::property_tree::read_json(str_stream, effect);
		if (node.get<std::string>("entryType") == "Offer") {
			boost::property_tree::ptree fieldSet = node.get_child("fields");
			int sell; 
			try {
				sell = fieldSet.get<int>("Flags") & LEDGER_FLAGS().get_child("offer").get<int>("Sell");
			}
			catch(boost::property_tree::ptree_error ex) { sell = 0; }

			int ff = 0;
			std::string nfAc;
			try {
				nfAc = fieldSet.get<std::string>("Account");
				ff = 1;
			}
			catch (boost::property_tree::ptree_error ex) { ff = 0; }
			if (ff == 1) {
				if (nfAc == account) {
					int f1 = 0;
					try {
						std::string t = node.get_child("fieldPrev").get<std::string>("TakerGets");
						f1 = 1;
					}
					catch (boost::property_tree::ptree_error ex) { f1 = 0; }
					if (node.get<std::string>("diffType") == "ModifiedNode" || (node.get<std::string>("diffType") == "DeletedNode" && f1 && isAmountZero(parseAmount(node.get_child("fieldsFinal"), "TakerGets")))) {
						effect.put("effect", "offer_partially_funded");
						boost::property_tree::ptree counterparty;
						counterparty.put("account", tx.get<std::string>("Account"));
						counterparty.put("seq", tx.get<std::string>("Sequence"));
						counterparty.put("hash", tx.get<std::string>("hash"));
						effect.put_child("counterparty",counterparty);
						if (node.get<std::string>("diffType") != "DeletedNode") {
							effect.put("remaining", !isAmountZero(parseAmount(node.get_child("fields"), "TakerGets")));
						}
						else {
							effect.put("cancelled", true);
						}
						
						effect.put_child("gets", parseAmount(fieldSet, "TakerGets"));
						effect.put_child("pays", parseAmount(fieldSet, "TakerPays"));
						boost::property_tree::ptree TakerGets = parseAmount(node.get_child("fieldsPrev"), "TakerGets");
						boost::property_tree::ptree TakerPays = parseAmount(node.get_child("fieldsPrev"), "TakerPays");
						effect.put_child("got", AmountSubtract(parseAmount(node.get_child("fieldsPrev"), "TakerPays"), parseAmount(node.get_child("fields"), "TakerPays")));
						effect.put_child("paid", AmountSubtract(parseAmount(node.get_child("fieldsPrev"), "TakerGets"), parseAmount(node.get_child("fields"), "TakerGets")));
						effect.put("type", sell ? "sold" : "bought");
					}  // 1. offer_partially_funded if
					else {
						std::string tp;
						int f2;
						try {
							tp = node.get_child("fieldsPrev").get<int>("TakerPays");
							f2 = 1;
						}
						catch (boost::property_tree::ptree_error ex) { f2 = 0; }
						effect.put("effect", node.get<std::string>("node") == "CreatedNode" ? "offer_created" : f2 ? "offer_funded" : "offer_cancelled");
						if (effect.get<std::string>("effect") == "offer_funded") {
							boost::property_tree::ptree fieldSet = node.get_child("fieldsPrev");
							boost::property_tree::ptree counterparty;
							counterparty.put("account", tx.get<std::string>("Account"));
							counterparty.put("seq", tx.get<std::string>("Sequence"));
							counterparty.put("hash", tx.get<std::string>("hash"));
							effect.put_child("counterparty", counterparty);
							effect.put_child("got", AmountSubtract(parseAmount(node.get_child("fieldsPrev"), "TakerPays"), parseAmount(node.get_child("fields"), "TakerPays")));
							effect.put_child("paid", AmountSubtract(parseAmount(node.get_child("fieldsPrev"), "TakerGets"), parseAmount(node.get_child("fields"), "TakerGets")));
							effect.put("type", sell ? "sold" : "bought");
						} // 2. offer_funded if
						if (effect.get<std::string>("effect") == "offer_created") {
							effect.put_child("gets", parseAmount(fieldSet, "TakerGets"));
							effect.put_child("pays", parseAmount(fieldSet, "TakerPays"));
							effect.put("type", sell ? "sell" : "buy");
						} // 3. offer_created
						if (effect.get<std::string>("effect") == "offer_cancelled") {
							effect.put("hash", node.get_child("fields").get<std::string>("PreviousTxnID"));
							if (result.get<std::string>("type") == "offercancel") {
								result.get_child("gets", parseAmount(fieldSet, "TakerGets"));
								result.get_child("pays", parseAmount(fieldSet, "TakerPays"));
							}
							effect.get_child("gets", parseAmount(fieldSet, "TakerGets"));
							effect.get_child("pays", parseAmount(fieldSet, "TakerPays"));
							effect.put("type", sell ? "sell" : "buy");
						}
					}
					effect.put("seq", node.get_child("fields").get<std::string>("Sequence"));
				} // fields.Account == account
				else if (tx.get<std::string>("Account") == account && node.get_child("fieldsPrev").size() != 0) {
					effect.put("effect", "offer_bought");
					boost::property_tree::ptree counterparty;
					counterparty.put("account", node.get_child("fields").get<std::string>("Account"));
					counterparty.put("seq", node.get_child("fields").get<std::string>("Sequence"));
					std::string hs;
					try {
						hs = node.get<std::string>("PreviousTxnID");
					}
					catch (boost::property_tree::ptree_error ex) {
						hs = node.get_child("fields").get<std::string>("PreviousTxnID");
					}
					counterparty.put("hash", hs);
					effect.put_child("paid", AmountSubtract(parseAmount(node.get_child("fieldsPrev"), "TakerPays"), parseAmount(node.get_child("fields"), "TakerPays")));
					effect.put_child("got", AmountSubtract(parseAmount(node.get_child("fieldsPrev"), "TakerGets"), parseAmount(node.get_child("fields"), "TakerGets")));
					effect.put("type", sell ? "bought" : "sold");
				} // 5. offer_bought if
				boost::property_tree::ptree gets, pays, got, paid;
				int g1, g2, p1, p2;
				try {
					gets = effect.get_child("gets");
					g1 = 1;
				}
				catch (boost::property_tree::ptree_error ex) { g1 = 0; }
				try {
					gets = effect.get_child("pays");
					p1 = 1;
				}
				catch (boost::property_tree::ptree_error ex) { p1 = 0; }
				try {
					gets = effect.get_child("got");
					g2 = 1;
				}
				catch (boost::property_tree::ptree_error ex) { g2 = 0; }
				try {
					gets = effect.get_child("paid");
					p2 = 1;
				}
				catch (boost::property_tree::ptree_error ex) { p2 = 0; }
				if ((g1 && p1) || (g2 && p2)) {
					bool created = effect.get<std::string>("effect") == "offer_created" && effect.get<std::string>("type") == "buy";
					bool funded = effect.get<std::string>("effect") == "offer_funded" && effect.get<std::string>("type") == "bought";
					bool cancelled = effect.get<std::string>("effect") == "offer_cancelled" && effect.get<std::string>("type") == "buy";
					bool bought = effect.get<std::string>("effect") == "offer_bought" && effect.get<std::string>("type") == "bought";
					bool partially_funded = effect.get<std::string>("effect") == "offer_partially_funded" && effect.get<std::string>("type") == "bought";
					effect.put("price", getPrice(effect, (created || funded || cancelled || bought || partially_funded)));
				} // add price if
			} // ff == 1 if
			if (result.get<std::string>("type") == "offereffect" && node.get<std::string>("entryType") == "AccountRoot") {
				if (node.get_child("fields").get<std::string>("RegularKey") == account) {
					effect.put("effect", "set_regular_key");
					effect.put("type", "null");
					effect.put("account", node.get_child("fields").get<std::string>("Account"));
					effect.put("regularkey", account);
				}
			}
			if (effect.size() != 0) {
				if (node.get<std::string>("diffType") == "DeletedNode" && effect.get<std::string>("effect") != "offer_bought") {
					effect.put("deleted", true);
				}
				blank.pop_back();
				blank.push_back(std::make_pair("", effect));
				result.put_child("effects", blank);				
			}
		}  // entrytype == offer if 
	}


	return result;
}

boost::property_tree::ptree ToAmount(boost::property_tree::ptree amount) {
	double value;
	int f1, f2;
	try {
		value = amount.get<double>("value");
		f1 = 1;
	}
	catch (boost::property_tree::ptree_error ex) { f1 = 0; }
	if (f1 == 1 && value > 100000000000) {
		boost::property_tree::ptree ex;
		ex.put("invalid amount", "amount's maximum value is 100000000000");
		return ex;
	}

	std::string cur;
	try {
		cur = amount.get<std::string>("currency");
		f2 = 1;
	}
	catch (boost::property_tree::ptree_error ex) { f2 = 0; }
	if (f1 && f2 && cur == currency) {
		boost::property_tree::ptree str;
		str.put("String", std::to_string(value*1000000.00));
		return str;
	}

	return amount;
}

std::string StringToJsonFormat(std::string s) {
	std::stringstream str(s);
	boost::property_tree::ptree t;
	boost::property_tree::read_json(str, t);
	std::stringstream str2;
	boost::property_tree::write_json(str2, t);

	return str2.str();
}

