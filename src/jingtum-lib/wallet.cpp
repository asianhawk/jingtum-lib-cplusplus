#include "pch.h"
#include "stdafx.h"
#include "jingtumlib.h"
#include "keypairs.h"

jingtumlib::Wallet::Wallet() {	
	PrivateKey = "00";
}

jingtumlib::Wallet::Wallet(std::string input) {
	PrivateKey = "00";
	publickeychar = deriveKeyPair(input, &PublicKey, &PrivateKey, prikey);
	secret = input;
}

jingtumlib::Wallet::~Wallet() {
	
}

std::string jingtumlib::Wallet::getPublicKey() {
	return PublicKey;
}

std::string jingtumlib::Wallet::generate() {	
	secret = generateSeed();
	publickeychar = deriveKeyPair(secret, &PublicKey, &PrivateKey, prikey);
	Address = deriveAddress(publickeychar);
	std::string ss;
	ss = "{\"secret\":" + secret + ",\"address\":" + Address + "}";
	return ss;
}

std::string jingtumlib::Wallet::fromSecret(std::string input) {
	publickeychar = deriveKeyPair(input, &PublicKey, &PrivateKey, prikey);
	Address = deriveAddress(publickeychar);
	std::string ss;
	ss = "{\"secret\":" + input + ",\"address\":" + Address + "}";
	return ss;
}

std::string jingtumlib::Wallet::signTx(std::string message) {
	if (message == "") return NULL;

	int half = message.length() / 2;
	unsigned char *pt = new unsigned char[half];
	Convert(pt, message);

	secp256k1_ecdsa_signature signature;
	unsigned char sig[72];
	size_t siglen = 72;
	secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
	secp256k1_ecdsa_sign(ctx, &signature, pt, prikey, NULL, NULL);
	secp256k1_ecdsa_signature_serialize_der(ctx, sig, &siglen, &signature);

	std::string ss;	
	char secret[145] = { 0 };
	char tt[3] = { 0 };
	for (unsigned int i = 0; i < siglen; ++i) {
		sprintf_s(tt, "%02x", sig[i]);
		strcat_s(secret, tt);
	}
	_strupr_s(secret);
	ss = secret;

	delete[] pt;

	return ss;
}


