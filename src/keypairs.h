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

std::string generateSeed();  // 生成seed
std::string base58encode(const std::string &);  // base58编码
std::string base58decode(std::string);  // base58解码    // 是否要处理第一个零字符，未确认
std::string __encode(int, unsigned char *);  // jingtum编码规则
unsigned char * __decode(int, std::string);  // jingtum解码规则
// 生成钥匙对, deriveKeyPair(secret, PublicKey, PrivateKey)
unsigned char * deriveKeyPair(std::string,  std::string *, std::string *);  
std::string deriveAddress(unsigned char *);  // 从公钥生成钱包地址
unsigned char * brand(int);  // 随机数函数
int hex2int(char);  // 16进制字符到int
void Convert(unsigned char *, std::string);  // 两位16进制字符串到int转化
bool checkAddress(std::string);

#endif KEYPAIRS_H
