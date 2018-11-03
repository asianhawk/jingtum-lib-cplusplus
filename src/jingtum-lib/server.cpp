#include "pch.h"
#include "stdafx.h"
#include "jingtumlib.h"

// for BasicConnect
jingtumlib::BasicConnect::BasicConnect(std::string protocol, std::string domain, std::string port) {
	_protocol = protocol;
	_domain = domain;
	_port = port;
}

void jingtumlib::BasicConnect::WriteMessage(std::string text) {
	if (_protocol == "ws") {
		ws.write(boost::asio::buffer(std::string(text)));
	}
	else if (_protocol == "wss") {
		wss.write(boost::asio::buffer(std::string(text)));
	}
}

std::string jingtumlib::BasicConnect::ReceiveMessage() {
	if (_protocol == "ws") {		
		boost::beast::multi_buffer buffer;
		ws.read(buffer);
		std::stringstream ss;
		ss << boost::beast::buffers(buffer.data());
		return ss.str();		
	}
	else if (_protocol == "wss") {
		boost::beast::multi_buffer buffer;
		wss.read(buffer);
		std::stringstream ss;
		ss << boost::beast::buffers(buffer.data());
		return ss.str();
	}
	return "";
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
	_firstopen = false;
	_message = false;
	_closing = false;
	msgid = 0;
	reqid = 0;
	boost::regex domain_RE("^(?=.{1,255}$)[0-9A-Za-z](?:(?:[0-9A-Za-z]|[-_]){0,61}[0-9A-Za-z])?(?:.[0-9A-Za-z](?:(?:[0-9A-Za-z]|[-_]){0,61}[0-9A-Za-z])?)*.?$");
	boost::smatch m;
	try {
		if (!regex_match(domain, m, domain_RE)) throw std::string("Server host incorrect");
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }
			
	try {
		int Port;
		if (isInt(port)) {
			std::stringstream str(port);
			str >> Port;
			if (Port < 1 || Port > 65535) throw std::string("Server host incorrect");
		}
		else throw std::string("server port is not a number");		
	}
	catch (std::string ex) { std::cout << ex << std::endl; exit(0); }

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
		_firstopen = true;
	}
	catch (std::exception ex) {
		std::cout << ex.what() << "Server Connecting Wrong!" << std::endl;
		callback(ex.what(), "", NULL);
		exit(0);
	}
	
	while (!_closing) {
		
		if (_firstopen) {
			_opened = true;
			std::string text = "[\"ledger\",\"server\"]";
			Request req = rem->subscribe(text);
			req.submit(callback);
			_firstopen = false;
		}
		if (_message) {	
			unsigned int oldsize; 
			do {
				oldsize = bmpack.size();
				for (unsigned int i = 0; i < bmpack.size(); ++i) {
					if (bmpack[i].handled) continue;
					else {
						std::cout << "backmessage :-----------" << bmpack[i].backmsg << std::endl;
						rem->_handleMessage(bmpack[i].backmsg);
						bmpack[i].handled = true;
					}
				}
			} while (oldsize < bmpack.size());
			_message = false;
		}	
		if (!_closing) receiveMessage();
	}
	disconnect();	
}

void jingtumlib::Server::sendMessage(std::string command, std::string data) {

	if (!_opened) {
		std::cout << "connection is not open!" << std::endl;
		exit(0);
	}
		
	std::string text;
	if (data != "") {
		if (data.find(",") == 0) {
			text = "{\"id\":" + std::to_string(reqid) + ",\"command\":\"" + command + "\"" + data + "}";
		}
		else {
			text = "{\"id\":" + std::to_string(reqid) + ",\"command\":\"" + command + "\"," + data + "}";
		}
		
	}
	else {
		text = "{\"id\":" + std::to_string(reqid) + ",\"command\":\"" + command  + "\"}";
	}	
		
	if (flag == 0) {
		cns->WriteMessage(text);
	}
	if (flag == 1) {
		cs->WriteMessage(text);
	}

	reqid++;	
}

void jingtumlib::Server::receiveMessage() {	
	std::string backmsg = "";
	if (flag == 0) {
		backmsg = cns->ReceiveMessage();
	}
	if (flag == 1) {
		backmsg = cs->ReceiveMessage();
	}
	if (backmsg != "") {		
		bm p;
		p.id = msgid;
		p.backmsg = backmsg;
		p.handled = false;
		bmpack.push_back(p);
		_message = true;
		msgid++;
	}
}

void jingtumlib::Server::disconnect() {
	if (flag == 0) {
		_connected = !cns->CloseConnect();
	}
	else if (flag == 1) {
		_connected = !cs->CloseConnect();
	}
	setState("offline");
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

