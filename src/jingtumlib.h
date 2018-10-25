#ifndef JINGTUMLIB_H
#define JINGTUMLIB_H

#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif

#define BOOST_SPIRIT_THREADSAFE

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
#include "lru-cache.h"
#include "utils.h" 

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace websocket = boost::beast::websocket;

struct para {
	int addr;
	bool fg;
	std::string str;
	void(*_remcs)(std::string, std::string);
	void(*_reqcs)(std::string, std::string, int);
};

namespace jingtumlib {

	class Server;
	class Request;
	class Transaction;
	class ConnectNoSSL;
	class ConnectSSL;
	class Remote;
	
	struct cbk {
		void (*_reqcs)(std::string, std::string);		
		void req_callback_submit(std::string, std::string);
		void (*_remcs)(std::string, std::string);
		void remote_callback_submit(std::string, std::string);
		Remote *r;
		Transaction *tx;

	};

	// --------------------------Wallet------------------------------
	class Wallet {
		
	public:
		Wallet();   
		Wallet(std::string);
		~Wallet();  
		std::string generate();  
		std::string fromSecret(std::string);   
		std::string getPublicKey();
		std::string signTx(std::string);
		
	public:
		std::string secret;  // 对外的私钥		
		std::string PrivateKey;  // 私钥字符串形式
		unsigned char prikey[32];  // 私钥
		std::string PublicKey;  // 公钥字符串形式
		unsigned char *publickeychar;  // 公钥
		std::string Address;  // 钱包地址
	};

	//---------------------------Remote------------------------------ 
	class Remote {

		/*friend class Request;  
		friend class Transaction; */

	public:
		std::string _url;  
		bool _localsign;   
		
	public:
		Remote(std::string);  
		~Remote();  
		void connect(void(*)(std::string, std::string, Remote *));  
		void disConnect();  
		void _submit(std::string, std::string, para, std::string(*)(std::string, para));
		void _submit(std::string, std::string, para, std::string (*)(std::string, para), void(*)(std::string, std::string));  
		void _submit(std::string, std::string, para, std::string(*)(std::string, para), void(*)(std::string, std::string, int));

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
		Transaction buildRelationTx(std::string);
		Transaction buildAccountSetTx(std::string);
		Transaction buildOfferCreateTx(std::string);
		Transaction buildOfferCancelTx(std::string);
		Transaction deployContractTx(std::string);
		Transaction callContractTx(std::string);
		void _handleMessage(std::string);
	private:
		void parseUrl(std::string);  
		
		void _handleLedgerClosed(boost::property_tree::ptree);
		void _handleServerStatus(boost::property_tree::ptree);
		void _handleResponse(boost::property_tree::ptree);
		void _handleTransaction(boost::property_tree::ptree);
		void _handlePathFind(boost::property_tree::ptree);
		void _updateServerStatus(boost::property_tree::ptree);
		void _requestAccount(std::string, Request *);
		void _buildTrustSet(boost::property_tree::ptree, Transaction *);
		void _buildRelationSet(boost::property_tree::ptree, Transaction *);
		void _buildAccountSet(boost::property_tree::ptree, Transaction *);
		void _buildDelegateKeySet(boost::property_tree::ptree, Transaction *);
		void _buildSignerSet(boost::property_tree::ptree, Transaction *);


	private:
		std::string protocol;  
		std::string domain;  
		std::string port;  
		Server *server;  // server对象指针，用于连接服务器
		boost::property_tree::ptree _status;  // 储存返回数据的状态

	public:
		LRUCache *_cache;
		LRUCache *_paths;
		
	};	

	//---------------------------Request-------------------------------- 
	class Request {

	public:
		Request(Remote *);
		Request(Remote *, std::string, std::string (*)(std::string, para));	

		void submit();
		void submit(void(*callback)(std::string, std::string)); 
		void submit(void(*callback)(std::string, std::string, int));
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

		void submit(void(*callback)(std::string, std::string));  
		void setSecret(std::string);
		void setFlags(std::string);
		void addMemo(std::string);		
		std::string getAccount();
		std::string getTransactionType();
		void setSendMax(boost::property_tree::ptree);
		void setTransferRate(double);
		void setPath(std::string);

		std::string signing();
		void sign(void(*callback)(std::string, std::string, int));

	public:
		Remote *rem;
		boost::property_tree::ptree tx_json;
		std::string _secret;
		para pa;
		bool _local_sign;		
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
		int addr;  // 储存每次请求的对象的地址；
	};

	class Server {
	public:
		Server(std::string, std::string, std::string); 
		~Server();  
		bool basicconnect();  
		void connect(Remote *, void(*)(std::string, std::string, Remote *));
		int sendMessage(std::string,std::string);  
		bool disconnect();  
		void setState(std::string);
		bool isConnected();
		void _handleClose();

	public:
		
		std::string _state;
		int id;
		std::vector<bm> bmsg;  //储存每次返回的数据
		bm cbm;  //当前的返回数据

		std::string onlineStates[6] = { "syncing", "tracking", "proposing", "validating", "full", "connected" };

	private:
		void getResult(std::string);  // 提取返回数据的result字段
		ConnectNoSSL *cns;
		ConnectSSL *cs;
		int flag;
		bool _connected;
		bool _opened;
	};

	
}

std::string callback_requestServerInfo(std::string, para);
std::string callback_requestLedgerClosed(std::string, para);
std::string callback_requestLedger(std::string, para);
std::string callback_requestAccountTx(std::string, para);
int getRelationType(std::string);
std::string prepareFlag(std::string);
std::string processContract(std::string);
boost::property_tree::ptree MaxAmount(std::string);

// in transaction
void remote_callback_submit(std::string, std::string, int);
void req_callback_submit(std::string, std::string, int);

#endif JINGTUMLIB_H