#ifndef JINGTUMLIB_H
#define JINGTUMLIB_H

#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/regex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <openssl/sha.h>
#include <openssl/bn.h>
#include <openssl/sha.h>
#include <openssl/ecdsa.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <cstdlib>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>
#include "utils.h"
 

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace websocket = boost::beast::websocket;

struct para {
	bool fg;
	std::string str;
};

namespace jingtumlib {

	class Server;
	class Request;
	class ConnectNoSSL;
	class ConnectSSL;
		
	// --------------------------Wallet------------------------------
	class Wallet {
		
	public:
		Wallet();   
		~Wallet();  
		std::string generate();  
		std::string fromSecret(std::string);   
		
	public:
		std::string secret;  // 对外的私钥		
		std::string PrivateKey;  // 私钥字符串形式
		std::string PublicKey;  // 公钥字符串形式
		unsigned char *publickeychar;  // 公钥
		std::string Address;  // 钱包地址

	};

	//---------------------------Remote------------------------------ 
	class Remote {

		friend class Request;  
		friend class Transaction; 

	public:
		std::string _url;  
		bool _localsign;   
		
	public:
		Remote(std::string);  
		~Remote();  
		void connect(void(*)(std::string, std::string, Remote *));  
		void disConnect();  
		void _submit(std::string, std::string, para, std::string (*)(std::string, para), void(*)(std::string, std::string, Remote *));  

		Request subscribe(std::string);  
		Request requestServerInfo();
		Request requestLedgerClosed();
		Request requestLedger(std::string);
		Request requestTx(std::string);
		Request requestAccountInfo(std::string);
		Request requestAccountTums(std::string);
		Request requestAccountRelations(std::string);
		Request requestAccountOffers(std::string);
		Request requestAccountTx(std::string);
		Request requestOrderBook(std::string);

		Transaction buildPaymentTx(std::string);

	private:
		void parseUrl(std::string);  
		void _handleMessage(int, std::string);
		void _handleLedgerClosed();
		void _handleServerStatus();
		void _handleResponse(int);
		void _handleTransaction();
		void _handlePathFind();
		void _updateServerStatus(int);
		void _requestAccount(std::string, Request *);

	private:
		std::string protocol;  
		std::string domain;  
		std::string port;  
		Server *server;  // server对象指针，用于连接服务器
		boost::property_tree::ptree _status;  // 储存返回数据的状态
		
	};	

	//---------------------------Request-------------------------------- 
	class Request {

	public:
		Request(Remote *);
		Request(Remote *, std::string, std::string (*)(std::string, para));	

		void submit(void(*callback)(std::string, std::string, Remote *)); 
		void selectLedger(boost::property_tree::ptree);  

	public:
		Remote *rem;
		std::string  _message;
		std::string _command;
		para pa;

	private:
		std::string (*_filter)(std::string, para);
		
	};		

	//---------------------------Transaction-------------------------------- 
	class Transaction {

	public:
		Transaction(Remote *);

		void submit(void(*callback)(std::string, std::string, Remote *));  
		void setSecret(std::string);
		void addMemo(std::string);
		std::string signing();

	public:
		Remote *rem;
		boost::property_tree::ptree tx_json;
		std::string _secret;
		para pa;

	};

	//---------------------------BasicConnect--------------------------- 
	class BasicConnect {

	public:
		BasicConnect(std::string, std::string, std::string);  
		std::string Message(std::string);  
		bool CloseConnect();  

	public:
		std::string _text;  
		std::string _protocol;  
		std::string _domain;  
		std::string _port;  

	public: // boost::beast库中websocket所用的参数
		boost::asio::io_context ioc;
		tcp::resolver resolver{ ioc };
		websocket::stream<tcp::socket> ws{ ioc };
		ssl::context ctx{ ssl::context::sslv23_client };
		websocket::stream<ssl::stream<tcp::socket>> wss{ ioc, ctx };

	};
	// -------------------------Websocket----------------------------------
	class ConnectNoSSL : public BasicConnect {  
	public:
		ConnectNoSSL(std::string, std::string, std::string);  
		void Connect();  
	};

	class ConnectSSL : public BasicConnect {  
	public:
		ConnectSSL(std::string, std::string, std::string); 
		void Connect();  
	};

	// ------------------------Server---------------------------------------
	struct bm {  // 返回数据部分结果解析
		std::string backmsg;  // 储存返回数据
		std::string datatype;  // 储存返回数据的type字段
		int dataid;  // 储存返回数据的id字段
		std::string dataresult;  // 储存返回数据的result字段
		std::string datastatus;  // 储存返回数据的status字段
		std::string dataresultServerStatus;  //储存返回数据的result字段中的server_status字段
	};

	class Server {
	public:
		Server(std::string, std::string, std::string); 
		~Server();  
		bool connect();  
		int sendMessage(std::string,std::string);  
		bool disconnect();  
		void setState(std::string);

	public:
		ConnectNoSSL *cns;  
		ConnectSSL *cs;  
		int flag;  
		bool _connected;  
		bool _opened;
		std::string _state;
		int id;
		std::vector<bm> bmsg;  //储存每次返回的数据

		bm cbm;  //当前的返回数据

		std::string onlineStates[6] = { "syncing", "tracking", "proposing", "validating", "full", "connected" };

	private:
		std::string getResult(std::string);  // 提取返回数据的result字段		
	};
}

std::string callback_requestServerInfo(std::string, para);
std::string callback_requestLedgerClosed(std::string, para);
std::string callback_requestLedger(std::string, para);
std::string callback_requestAccountTx(std::string, para);
int getRelationType(std::string);


#endif JINGTUMLIB_H