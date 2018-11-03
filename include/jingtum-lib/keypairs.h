#ifndef KEYPAIRS_H
#define KEYPAIRS_H

#include <boost/asio/ssl/stream.hpp>
#include <openssl/bn.h>
#include <openssl/sha.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <string>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <secp256k1/secp256k1.h>
#include <secp256k1/lax_der_parsing.h>
#include <secp256k1/lax_der_privatekey_parsing.h>

#define SEED_PREFIX 33
#define ACCOUNT_PREFIX 0
#define DOMAIN_CHECK(c) ('0'<=(c)&&(c)<='9'||'a'<=(c)&&(c)<='f'||'A'<=(c)&&(c)<='F')

std::string generateSeed();  
std::string base58encode(const std::string &);  
std::string base58decode(std::string);  
std::string __encode(int, unsigned char *); 
unsigned char * __decode(int, std::string);  

unsigned char * deriveKeyPair(std::string,  std::string *, std::string *, unsigned char *);
std::string deriveAddress(unsigned char *); 
unsigned char * convertAddressToBytes(std::string);
unsigned char * brand(int);  
int hex2int(char);  
void Convert(unsigned char *, std::string);  
void Convert(std::vector<unsigned char> *, std::string);
bool checkAddress(std::string);

#endif KEYPAIRS_H
