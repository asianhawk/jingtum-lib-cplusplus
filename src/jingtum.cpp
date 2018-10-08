// jingtum.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "jingtumlib.h"
#include "Serializer.h"

void callback_print(std::string, std::string, jingtumlib::Remote *);  // 3. connect方法回调
void callback_requestServerInfo(std::string, std::string, jingtumlib::Remote *);  // 4. requestServerInfo方法回调
void callback_requestLedgerClosed(std::string, std::string, jingtumlib::Remote *);  // 5. requestLedgerClosed方法回调
void callback_requestLedger(std::string, std::string, jingtumlib::Remote *);  // 6. requestLedger方法回调
void callback_requestTx(std::string, std::string, jingtumlib::Remote *);  // 7. requestTx方法回调
void callback_requestAccountInfo(std::string, std::string, jingtumlib::Remote *);  // 8. requestAccountInfo方法回调
void callback_requestAccountTums(std::string, std::string, jingtumlib::Remote *);  // 9. requestAccountTums方法回调
void callback_requestAccountRelations(std::string, std::string, jingtumlib::Remote *);  // 10. requestAccountRelations方法回调
void callback_requestAccountOffers(std::string, std::string, jingtumlib::Remote *);  // 11. requestAccountOffers方法回调
void callback_requestAccountTx(std::string, std::string, jingtumlib::Remote *);  // 12. requestAccountTx方法回调
void callback_requestOrderBook(std::string, std::string, jingtumlib::Remote *);  // 13. requestOrderBook方法回调

int main()
{	
	/* -------说明：需要测试哪个功能，按照序号范围消除注释，并将其他功能关掉即可---------------*/

	/*---------------------------Wallet 测试-----------------------------------------------*/
	// 1. 生成钱包
	//jingtumlib::Wallet wallet;
	/*std::string ss;
	ss = wallet.generate();
	std::cout << ss << std::endl;*/

	// 2. 从私钥生成地址
	/*jingtumlib::Wallet wallet;
	std::string ss;
	ss = wallet.fromSecret("ss2A7yahPhoduQjmG7z9BHu3uReDk");
	std::cout << ss << std::endl; */
	
	/*--------------------------------Remote 类测试-----------------------------------------*/
	std::string serverInfo = "{\"server\":\"ws://ts5.jingtum.com:5020\",\"local_sign\":\"true\"}";   // 传入以字符串形式表示的JSON数据
	//jingtumlib::Remote remote(serverInfo);  //创建remote对象
	//remote.connect(callback_print);  // 3. Remote::connect方法
	//remote.connect(callback_requestServerInfo);  // 4. Remote::requestServerInfo方法
	//remote.connect(callback_requestLedgerClosed);  // 5. Remote::requestLedgerClosed方法
	//remote.connect(callback_requestLedger);  // 6. Remote::requestLedger方法
	//remote.connect(callback_requestTx);  // 7. Remote::requestTx方法
	//remote.connect(callback_requestAccountInfo);  // 8. Remote::requestAccountInfo方法
	//remote.connect(callback_requestAccountTums);  // 9. Remote::requestAccountTums方法
	//remote.connect(callback_requestAccountRelations);  // 10. Remote::requestAccountRelations方法
	//remote.connect(callback_requestAccountOffers);  // 11. Remote::requestAccountOffers方法
	//remote.connect(callback_requestAccountTx);  // 12. Remote::requestAccountTx方法
	//remote.connect(callback_requestOrderBook);  // 13. Remote::requestOrderBook方法

	//remote.disConnect();


}

void callback_print(std::string ex, std::string msg, jingtumlib::Remote *remote) {
	if (ex != "") {
		std::cout << ex << std::endl;
	}
	else {
		std::cout << "result:" << msg << std::endl;
	}
}

void callback_requestServerInfo(std::string ex, std::string msg, jingtumlib::Remote *remote) {
	if (ex != "") {
		std::cout << ex << std::endl;
	}
	else {
		jingtumlib::Request req = remote->requestServerInfo();
		req.submit(callback_print);
	}
}

void callback_requestLedgerClosed(std::string ex, std::string msg, jingtumlib::Remote *remote) {
	if (ex != "") {
		std::cout << ex << std::endl;
	}
	else {
		jingtumlib::Request req = remote->requestLedgerClosed();
		req.submit(callback_print);
	}
}

void callback_requestLedger(std::string ex, std::string msg, jingtumlib::Remote *remote) {
	if (ex != "") {
		std::cout << ex << std::endl;
	}
	else {
		jingtumlib::Request req = remote->requestLedger("{}");  // 默认返回最新账本信息
		//jingtumlib::Request req = remote->requestLedger("{\"ledger_index\":1000,\"transactions\":\"true\"}");
		req.submit(callback_print);
	}
}

void callback_requestTx(std::string ex, std::string msg, jingtumlib::Remote *remote) {
	if (ex != "") {
		std::cout << ex << std::endl;
	}
	else {
		jingtumlib::Request req = remote->requestTx("{\"hash\":\"B0E15F21E416E2C720E2F5C4FD2B0B9B6A3EA619ADA3461BDB1242D022273336\"}");
		req.submit(callback_print);
	}
}

void callback_requestAccountInfo(std::string ex, std::string msg, jingtumlib::Remote *remote) {
	if (ex != "") {
		std::cout << ex << std::endl;
	}
	else {
		jingtumlib::Request req = remote->requestAccountInfo("{\"account\":\"jDUjqoDZLhzx4DCf6pvSivjkjgtRESY62c\"}");
		req.submit(callback_print);
	}
}

void callback_requestAccountTums(std::string ex, std::string msg, jingtumlib::Remote *remote) {
	if (ex != "") {
		std::cout << ex << std::endl;
	}
	else {
		jingtumlib::Request req = remote->requestAccountTums("{\"account\":\"jDUjqoDZLhzx4DCf6pvSivjkjgtRESY62c\"}");
		req.submit(callback_print);
	}
}

void callback_requestAccountRelations(std::string ex, std::string msg, jingtumlib::Remote *remote) {
	if (ex != "") {
		std::cout << ex << std::endl;
	}
	else {
		jingtumlib::Request req = remote->requestAccountRelations("{\"account\":\"jDUjqoDZLhzx4DCf6pvSivjkjgtRESY62c\",\"type\":\"trust\"}");
		req.submit(callback_print);
	}
}

void callback_requestAccountOffers(std::string ex, std::string msg, jingtumlib::Remote *remote) {
	if (ex != "") {
		std::cout << ex << std::endl;
	}
	else {
		jingtumlib::Request req = remote->requestAccountOffers("{\"account\":\"jDUjqoDZLhzx4DCf6pvSivjkjgtRESY62c\"}");
		req.submit(callback_print);
	}
}

void callback_requestAccountTx(std::string ex, std::string msg, jingtumlib::Remote *remote) {
	if (ex != "") {
		std::cout << ex << std::endl;
	}
	else {
		jingtumlib::Request req = remote->requestAccountTx("{\"account\":\"jDUjqoDZLhzx4DCf6pvSivjkjgtRESY62c\"}");
		req.submit(callback_print);
	}
}

void callback_requestOrderBook(std::string ex, std::string msg, jingtumlib::Remote *remote) {
	if (ex != "") {
		std::cout << ex << std::endl;
	}
	else {		
		jingtumlib::Request req = remote->requestOrderBook("{\"gets\":{\"currency\":\"SWT\",\"issuer\":\"\"},\"pays\":{\"currency\":\"CNY\",\"issuer\":\"jBciDE8Q3uJjf111VeiUNM775AMKHEbBLS\"}}");
		req.submit(callback_print);
	}
}
