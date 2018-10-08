#include "pch.h"
#include "keypairs.h"

const char *alphabet = "jpshnaf39wBUDNEGHJKLM4PQRST7VWXYZ2bcdeCg65rkm8oFqi1tuvAxyz";

std::string generateSeed() {
	unsigned char *seed;
	seed = brand(16);
	return __encode(SEED_PREFIX, seed);
}

unsigned char * deriveKeyPair(std::string sec, std::string *PublicKey, std::string *PrivateKey) {
	
	EC_GROUP *ec_group;  // 椭圆函数
	BIGNUM *order;  // 椭圆函数的阶，secp256k1，常量
	order = BN_new();
	ec_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
	EC_GROUP_get_order(ec_group, order, NULL);

	unsigned char *seed;
	seed = __decode(SEED_PREFIX, sec);

	int seq = 0;
	int flag = 1;
	unsigned char expandseed[20];
	for (unsigned int i = 0; i < 16; ++i) {
		expandseed[i] = seed[i];
	}
	unsigned char md[SHA512_DIGEST_LENGTH];
	BIGNUM * pri_gen_bn = BN_new();
	unsigned char Private_Gen[32];  // 私钥生成
	while (flag == 1) {
		expandseed[16] = seq >> 24 & 0xFF;
		expandseed[17] = seq >> 16 & 0xFF;
		expandseed[18] = seq >> 8 & 0xFF;
		expandseed[19] = seq & 0xFF;
		std::string str((char *)expandseed);
		SHA512(expandseed, 20, md);
		for (unsigned int i = 0; i < 32; ++i) {
			Private_Gen[i] = md[i];
		}
		BN_hex2bn(&pri_gen_bn, (char *)Private_Gen);
		if (BN_is_zero(pri_gen_bn)) flag = 0;
		if (BN_cmp(pri_gen_bn, order) < 0) flag = 0;
		seq += 1;
	}

	unsigned char * Public_Gen;  // 33字节， 公钥生成
	EC_POINT * pubkey = EC_POINT_new(ec_group);
	BN_bin2bn(Private_Gen, 32, pri_gen_bn);
	EC_POINT_mul(ec_group, pubkey, pri_gen_bn, NULL, NULL, NULL);

	EC_POINT_point2buf(ec_group, pubkey, POINT_CONVERSION_COMPRESSED, &Public_Gen, NULL);
	/*std::cout << "pubgen:"<< std::endl;
	for (unsigned int i = 0; i < 33; i++)
	{
		if (((i % 16) == 0) && (i != 0)) printf("\n");
		printf("%02x ", (unsigned char)Public_Gen[i]);
	}*/
	unsigned char expandPubGen[41];
	for (unsigned int i = 0; i < 33; ++i) {
		expandPubGen[i] = Public_Gen[i];
	}
	flag = 1;
	seq = 0;
	unsigned char tmp[32];
	BIGNUM * bn = BN_new();
	while (flag == 1) {
		expandPubGen[33] = 0 >> 24 & 0xFF;
		expandPubGen[34] = 0 >> 16 & 0xFF;
		expandPubGen[35] = 0 >> 8 & 0xFF;
		expandPubGen[36] = 0 & 0xFF;
		expandPubGen[37] = seq >> 24 & 0xFF;
		expandPubGen[38] = seq >> 16 & 0xFF;
		expandPubGen[39] = seq >> 8 & 0xFF;
		expandPubGen[40] = seq & 0xFF;
		std::string str((char *)expandPubGen);
		SHA512(expandPubGen, 41, md);
		for (unsigned int i = 0; i < 32; ++i) {
			tmp[i] = md[i];
		}
		BN_hex2bn(&bn, (char *)tmp);
		if (BN_is_zero(bn)) flag = 0;
		if (BN_cmp(bn, order) < 0) flag = 0;
		seq += 1;
	}

	BN_bin2bn(tmp, 32, bn);
	BIGNUM * rem = BN_new();
	unsigned char to[32];  // store the privatekey
	BN_CTX * bnctx = BN_CTX_new();
	int s = BN_mod_add(rem, bn, pri_gen_bn, order, bnctx);
	s = BN_bn2bin(rem, to);

	char secret[65] = { 0 };
	char tt[3] = { 0 };
	int i;
	for (i = 0; i < 32; ++i) {
		sprintf_s(tt, "%02x", to[i]);
		strcat_s(secret, tt);
	}
	_strupr_s(secret);
	*PrivateKey += secret;

	BIGNUM * secbn = BN_new();
	BN_bin2bn(to, 32, secbn);
	EC_POINT_mul(ec_group, pubkey, secbn, NULL, NULL, NULL);
	unsigned char *publickeychar;
	EC_POINT_point2buf(ec_group, pubkey, POINT_CONVERSION_COMPRESSED, &publickeychar, NULL);
	/*std::cout << "publicKey:" << std::endl;
	for (unsigned int i = 0; i < 33; i++)
	{
		if (((i % 16) == 0) && (i != 0)) printf("\n");
		printf("%02x ", publickeychar[i]);
	}*/

	char pubchar[67] = { 0 };
	for (i = 0; i < 33; ++i) {
		sprintf_s(tt, "%02x", publickeychar[i]);
		strcat_s(pubchar, tt);
	}
	_strupr_s(pubchar);
	*PublicKey += pubchar;
	/*std::cout << "publickey:" << std::endl;
	std::cout << PublicKey << std::endl;*/

	BN_clear_free(bn);
	BN_clear_free(pri_gen_bn);
	BN_clear_free(rem);
	BN_CTX_free(bnctx);
	BN_clear_free(secbn);
	BN_clear_free(order);

	return publickeychar;
}

std::string deriveAddress(unsigned char *pubkeychar /*33 bytes*/) {
	unsigned char md1[SHA256_DIGEST_LENGTH];
	unsigned char md2[21];
	SHA256(pubkeychar, 33, md1);

	// ripemd160
	EVP_MD_CTX *ctx = EVP_MD_CTX_new();
	EVP_MD_CTX_init(ctx);
	EVP_DigestInit_ex(ctx, EVP_ripemd160(), NULL);
	EVP_DigestUpdate(ctx, md1, 32);
	unsigned int t = 20;
	EVP_DigestFinal_ex(ctx, md2, &t);
	EVP_MD_CTX_free(ctx);

	md2[20] = '\0';
	std::string ss = __encode(0, md2);
	//std::cout << "address:" << ss << std::endl;

	return ss;
};

std::string base58encode(const std::string & hexstring)
{
	std::string result = "";
	BN_CTX * bnctx = BN_CTX_new();
	BIGNUM * bn = BN_new();
	BIGNUM * bn0 = BN_new();
	BIGNUM * bn58 = BN_new();
	BIGNUM * dv = BN_new();
	BIGNUM * rem = BN_new();
	BN_hex2bn(&bn, hexstring.c_str());
	BN_hex2bn(&bn58, "3a");
	BN_hex2bn(&bn0, "0");
	while (BN_cmp(bn, bn0) > 0) {
		BN_div(dv, rem, bn, bn58, bnctx);
		BN_copy(bn, dv);
		char base58char = alphabet[BN_get_word(rem)];
		result += base58char;
	}
	// deal with leading zeros
	for (unsigned int i = 0; i < hexstring.length() - 1 && hexstring[i] == '0' && hexstring[i + 1] == '0'; ++i) {
		result += alphabet[0];
	}
	std::reverse(result.begin(), result.end());

	BN_clear_free(bn);
	BN_clear_free(bn0);
	BN_clear_free(bn58);
	BN_clear_free(dv);
	BN_clear_free(rem);
	BN_CTX_free(bnctx);

	return result;
}

std::string base58decode(std::string hexstring) {
	std::string result;
	result = alphabet;

	BN_CTX * bnctx = BN_CTX_new();
	BIGNUM * bn = BN_new();
	BIGNUM * bn0 = BN_new();
	BIGNUM * bn58 = BN_new();
	BIGNUM * rem = BN_new();
	BN_hex2bn(&bn0, "0");
	BN_hex2bn(&bn58, "3a");

	for (unsigned int i = 0; i < hexstring.length(); ++i) {
		unsigned long pos = result.find(hexstring[i]);
		BN_set_word(rem, pos);
		BN_mul(bn, bn0, bn58, bnctx);
		BN_add(bn0, bn, rem);
	}

	result = BN_bn2hex(bn0);
	// deal with leading zeros
	std::reverse(result.begin(), result.end());
	for (unsigned int i = 0; i < hexstring.length() - 1 && hexstring[i] == alphabet[0]; ++i) {
		result += "00";
	}
	std::reverse(result.begin(), result.end());

	BN_clear_free(bn);
	BN_clear_free(bn0);
	BN_clear_free(bn58);
	BN_clear_free(rem);
	BN_CTX_free(bnctx);

	return result;
}

std::string __encode(int version, unsigned char *randBytes) {
	int size = strlen((const char *)randBytes);
	unsigned char *s = new unsigned char[1 + size];
	s[0] = version;
	int i;
	for (i = 1; i <= size; ++i) {
		s[i] = randBytes[i - 1];
	}
	unsigned char md1[SHA256_DIGEST_LENGTH];
	unsigned char md2[SHA256_DIGEST_LENGTH];
	SHA256(s, size + 1, md1);
	SHA256(md1, SHA256_DIGEST_LENGTH, md2);
	char buf[100] = { 0 };
	char tmp[3] = { 0 };
	for (i = 0; i <= size; ++i) {
		sprintf_s(tmp, "%02x", s[i]);
		strcat_s(buf, tmp);
	}
	for (i = 0; i < 4; ++i) {
		sprintf_s(tmp, "%02x", md2[i]);
		strcat_s(buf, tmp);
	}
	buf[(size + 5) * 2 + 1] = '\0';
	std::string op = std::string((const char*)buf);

	return base58encode(op);
}

unsigned char * __decode(int version, std::string input) {
	std::string ss = base58decode(input);
	int i;
	int half = ss.length() / 2;
	unsigned char *pt = new unsigned char[half];

	Convert(pt, ss);

	if (pt[0] != version || half < 5) {
		std::cout << "invalid input size" << std::endl;
		throw 1;
	}
	unsigned char md1[SHA256_DIGEST_LENGTH];
	unsigned char md2[SHA256_DIGEST_LENGTH];
	SHA256((unsigned char *)pt, half - 4, md1);
	SHA256(md1, SHA256_DIGEST_LENGTH, md2);
	
	for (unsigned int i = 0; i < 4; ++i) {
		if (md2[i] != pt[half - 4 + i]) {
			unsigned char *ex = NULL;
			/*std::string st= "Invalid input";
			ex = (unsigned char*)st.c_str();*/
			return ex;
		}
	}

	unsigned char *res = new unsigned char[half];
	for (i = 0; i < half - 1; ++i) {
		res[i] = pt[i + 1];
	}
	res[half - 1] = '\0';
	return res;
}

unsigned char * brand(int num) {
	unsigned char *randNum = new unsigned char[num + 1];
	int i;
	srand((unsigned)time(NULL));
	for (i = 0; i < num; ++i) {
		int t = rand() % 256;
		randNum[i] = t;
	}
	randNum[num] = '\0';
	return randNum;
}

int hex2int(char c)
{
	if ((c >= 'A') && (c <= 'Z'))
	{
		return c - 'A' + 10;
	}
	else if ((c >= 'a') && (c <= 'z'))
	{
		return c - 'a' + 10;
	}
	else if ((c >= '0') && (c <= '9'))
	{
		return c - '0';
	}
	return -1;
}

void Convert(unsigned char *pt, std::string ss) {
	const char *st = ss.c_str();
	int len = ss.length() / 2;
	int i;
	for (i = 0; i < len; ++i) {
		int high = hex2int(st[2 * i]);
		int low = hex2int(st[2 * i + 1]);
		pt[i] = high * 16 + low;
	}
}

bool checkAddress(std::string addr) {
	unsigned char *s;		
	s = __decode(ACCOUNT_PREFIX, addr);	
	if (s == NULL) {
		return false;
	}
	else {
		return true;
	}	
}