
#include "pch.h"
#include "jingtumlib.h"

// for BasicConnect
jingtumlib::BasicConnect::BasicConnect(std::string protocol, std::string domain, std::string port) {
	_protocol = protocol;
	_domain = domain;
	_port = port;
}

std::string jingtumlib::BasicConnect::Message(std::string text) {
	if (_protocol == "ws") {
		ws.write(boost::asio::buffer(std::string(text)));
		boost::beast::multi_buffer buffer;
		ws.read(buffer);
		std::stringstream ss;
		ss << boost::beast::buffers(buffer.data());
		return ss.str();
	}
	else if (_protocol == "wss") {
		wss.write(boost::asio::buffer(std::string(text)));
		boost::beast::multi_buffer buffer;
		wss.read(buffer);
		std::stringstream ss;
		ss << boost::beast::buffers(buffer.data());
		return ss.str();
	}
	return 0;	
}

bool jingtumlib::BasicConnect::CloseConnect() {
	if (_protocol == "ws") {
		try {
			ws.close(websocket::close_code::normal);
			return true;
		}
		catch (std::exception ex) {
			std::cout << ex.what() + std::string("Server Information Parse Error!") << std::endl;
			exit(0);
		}
		
	}
	else if (_protocol == "wss") {		
		try {
			wss.close(websocket::close_code::normal);
			return true;
		}
		catch (std::exception ex) {
			std::cout << ex.what() + std::string("Server Information Parse Error!") << std::endl;
			exit(0);
		}
	}
	return false;
	
}

// for SSL
jingtumlib::ConnectSSL::ConnectSSL(std::string protocol, std::string domain, std::string port) : BasicConnect(protocol, domain, port) {
}


void jingtumlib::ConnectSSL::Connect() {
	auto const results = resolver.resolve(_domain, _port);
	boost::asio::connect(wss.next_layer().next_layer(), results.begin(), results.end());
	wss.next_layer().handshake(ssl::stream_base::client);
	wss.handshake(_domain, "/");
}

// for NoSSL
jingtumlib::ConnectNoSSL::ConnectNoSSL(std::string protocol, std::string domain, std::string port) : BasicConnect(protocol, domain, port) {
}

void jingtumlib::ConnectNoSSL::Connect() {
	auto const results = resolver.resolve(_domain, _port);
	boost::asio::connect(ws.next_layer(), results.begin(), results.end());
	ws.handshake(_domain, "/");

}

// for Server
jingtumlib::Server::Server(std::string protocol, std::string domain, std::string port) {
	_connected = false;
	_opened = false;
	_state = "offline";
	id = 0;
	/*boost::regex domain_RE("^(?=.{1,255}$)[0-9A-Za-z](?:(?:[0-9A-Za-z]|[-_]){0,61}[0-9A-Za-z])?(?:\.[0-9A-Za-z](?:(?:[0-9A-Za-z]|[-_]){0,61}[0-9A-Za-z])?)*\.?$");
	boost::smatch m;
	try {
		if (!regex_match(domain, m, domain_RE)) throw std::string("Server host incorrect");
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }*/
			
	/*try {
		int Port;
		if (isInt(port)) {
			std::stringstream str(port);
			str >> Port;
			if (Port < 1 || Port > 65535) throw std::string("Server host incorrect");
		}
		else throw std::string("server port is not a number");		
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }*/

	if (protocol == "ws") {
		flag = 0;
		cns = new ConnectNoSSL(protocol, domain, port);
	}
	else if (protocol == "wss") {
		flag = 1;
		cs = new ConnectSSL(protocol, domain, port);
	}
}

jingtumlib::Server::~Server() {
	if (cns) {
		delete cns;
	}
	if (cs) {
		delete cns;
	}
}

bool jingtumlib::Server::basicconnect() {	
	if (flag == 0) {
		try {
			cns->Connect();
		}
		catch (std::exception ex) {
			std::cout << ex.what() << "Connecting to Server Error!" << std::endl;
			exit(0);
		}
		return true;		
	}
	else if (flag == 1) {
		try {
			cs->Connect();
		}
		catch (std::exception ex) {
			std::cout << ex.what() << "Connecting to Server Error!" << std::endl;
			exit(0);
		}
		return true;
	}
	return false;
}

void jingtumlib::Server::connect(Remote *rem, void(*callback)(std::string, std::string, Remote *)) {

	if (_connected) return;
		
	try {
		_connected = basicconnect();
	}
	catch (std::exception ex) {
		std::cout << ex.what() << "Server Connecting Wrong!" << std::endl;
		callback(ex.what(), "", NULL);
		exit(0);
	}
	
	while (_connected) {

		//�¼���
		//on
		_opened = true;
		std::string text = "[\"ledger\",\"server\"]";
		Request req = rem->subscribe(text);
		req.submit();
		rem->_handleMessage(bmsg[0].backmsg);		
		callback("", StringToJsonFormat(bmsg[0].dataresult), rem);
		//message
		//close
		//error
		
	}

	
}

int jingtumlib::Server::sendMessage(std::string command, std::string data) {

	if (!_opened) {
		std::cout << "connection is not open!" << std::endl;
		exit(0);
	}

	cbm.backmsg = "";
	std::string text;
	if (data != "") {
		if (data.find(",") == 0) {
			text = "{\"id\":" + std::to_string(id) + ",\"command\":\"" + command + "\"" + data + "}";
		}
		else {
			text = "{\"id\":" + std::to_string(id) + ",\"command\":\"" + command + "\"," + data + "}";
		}
		
	}
	else {
		text = "{\"id\":" + std::to_string(id) + ",\"command\":\"" + command  + "\"}";
	}	

	std::cout << "text:" << text << std::endl;
	
	std::string msgt;
	if (flag == 0) {
		msgt = cns->Message(text);
	}
	else if (flag == 1) {
		msgt = cs->Message(text);
	}
	cbm.backmsg = msgt;
	getResult(msgt);

	bmsg.push_back(cbm);
	return id++;
}

bool jingtumlib::Server::disconnect() {
	if (flag == 0) {
		_connected = !cns->CloseConnect();
		return _connected;
	}
	else if (flag == 1) {
		_connected = !cs->CloseConnect();
		return _connected;
	}
	setState("offline");
	return false;
}

void jingtumlib::Server::getResult(std::string msg) {
	std::stringstream str_stream(msg);
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::read_json(str_stream, pt);
	}
	catch (std::exception ex) {
		std::cout << ex.what() << "Backend message parse error!" << std::endl;
		exit(0);
	}
	cbm.datastatus = pt.get<std::string>("status");
	if (cbm.datastatus == "success") {
		cbm.datatype = pt.get<std::string>("type");
		cbm.dataid = pt.get<int>("id");

		boost::property_tree::ptree tmp;
		tmp = pt.get_child("result");
		try {
			cbm.dataresultServerStatus = tmp.get<std::string>("server_status");
		}
		catch (std::exception ex) {
			cbm.dataresultServerStatus = "";
		}

		std::stringstream res;
		boost::property_tree::write_json(res, tmp);
		cbm.dataresult = res.str();
	}
	else if (cbm.datastatus == "error") {
		std::string error;
		std::string exception;
		try {
			error = pt.get<std::string>("error_message");
		}
		catch (std::exception ex) {
			error = "";
			std::cout << "error" << std::endl;
		}
		try {
			exception = pt.get<std::string>("error_exception");
		}
		catch (std::exception ex) {
			exception = "";
		}
		cbm.dataresult = error != "" ? error : exception;
	}
	
	std::cout << "msg:" << msg << std::endl;
	
}

void jingtumlib::Server::setState(std::string state) {
	if (_state == state) return;

	_state = state;
	_connected = (state == "online");
	if (!_connected) {
		_opened = false;
	}
}

bool jingtumlib::Server::isConnected() {
	return _connected;
}

void jingtumlib::Server::_handleClose() {
	if (_state == "offline") return;
	setState("offline");
	disconnect();
}

