
#include "pch.h"
#include "jingtumlib.h"
#include "keypairs.h"

jingtumlib::Wallet::Wallet() {	
	PrivateKey = "00";
}

jingtumlib::Wallet::~Wallet() {
	
}

std::string jingtumlib::Wallet::generate() {	
	unsigned char *pubkeychar;
	secret = generateSeed();
	pubkeychar = deriveKeyPair(secret,  &PublicKey, &PrivateKey);
	Address = deriveAddress(pubkeychar);
	std::string ss;
	ss = "{\"secret\":" + secret + ",\"address\":" + Address + "}";
	return ss;
}

std::string jingtumlib::Wallet::fromSecret(std::string input) {
	unsigned char *pubkeychar;	
	pubkeychar = deriveKeyPair(secret,  &PublicKey, &PrivateKey);
	Address = deriveAddress(pubkeychar);
	std::string ss;
	ss = "{\"secret\":" + input + ",\"address\":" + Address + "}";
	return ss;
}


