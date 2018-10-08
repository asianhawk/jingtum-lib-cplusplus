#ifndef KEYPAIRS_H
#define KEYPAIRS_H

#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif

#include <boost/asio/ssl/stream.hpp>
#include <openssl/sha.h>
#include <openssl/bn.h>
#include <openssl/sha.h>
#include <openssl/ecdsa.h>
#include <string>
#include <iostream>
#include <time.h>
#include <stdlib.h>

#define SEED_PREFIX 33
#define ACCOUNT_PREFIX 0
#define DOMAIN_CHECK(c) ('0'<=(c)&&(c)<='9'||'a'<=(c)&&(c)<='f'||'A'<=(c)&&(c)<='F')

std::string generateSeed();  // ����seed
std::string base58encode(const std::string &);  // base58����
std::string base58decode(std::string);  // base58����    // �Ƿ�Ҫ������һ�����ַ���δȷ��
std::string __encode(int, unsigned char *);  // jingtum�������
unsigned char * __decode(int, std::string);  // jingtum�������
// ����Կ�׶�, deriveKeyPair(secret, PublicKey, PrivateKey)
unsigned char * deriveKeyPair(std::string,  std::string *, std::string *);  
std::string deriveAddress(unsigned char *);  // �ӹ�Կ����Ǯ����ַ
unsigned char * brand(int);  // ���������
int hex2int(char);  // 16�����ַ���int
void Convert(unsigned char *, std::string);  // ��λ16�����ַ�����intת��
bool checkAddress(std::string);

#endif KEYPAIRS_H