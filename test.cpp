// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include "zmsg.hpp"
//#include "ThostFtdcMdApi.h"
//#include "MdSpi.h"

std::string CoreServer = "tcp://192.168.1.234:5555";

class Carbon {
private:
	int				requestID;
	void			*m_socket;
	void			*m_context;
	std::string		md_front,td_front;
	std::string		brokerid;
	std::string		accountNum;
	std::string		accountPwd;
	std::string		symbol;

public:
	Carbon(std::string _Server) {
		m_context	= zmq_ctx_new();
		m_socket	= zmq_socket(m_context, ZMQ_REQ);
		zmq_connect(m_socket, _Server.c_str());
		requestID = 0;
		init();
	}

	~Carbon() {
		if (m_socket != NULL) {
			zmq_close(m_socket);
			m_socket = NULL;
		}
	}

	int init() {
		md_front = msg("md_front");
		std::cout << "===获取行情地址===" << std::endl;
		std::cout << md_front << std::endl;
		td_front = msg("td_front");
		std::cout << "===获取交易地址===" << std::endl;
		std::cout << td_front << std::endl;
		brokerid = msg("broker_id");
		std::cout << "===获取座席编号===" << std::endl;
		std::cout << brokerid << std::endl;
		accountNum = msg("account_num");
		std::cout << "===获取账号===" << std::endl;
		std::cout << accountNum << std::endl;
		accountPwd = msg("account_pwd");
		std::cout << "===获取密码===" << std::endl;
		std::cout << "ok!" << std::endl;
		symbol = msg("this_symbol");
		std::cout << "===设定合约===" << std::endl;
		std::cout << symbol << std::endl;
		return 0;
	}

	int getID() {
		return ++requestID;
	}

	std::string log(std::string s) {
		msg("log_" + s);
		return "logged:"+s;
	}

	std::string error(int err) {
		std::stringstream ss;
		std::string sto;
		ss << err;
		ss >> sto;
		return msg("error_" + sto);
	}

	std::string msg(std::string s) {
		zmsg msg(s.c_str());
		msg.send((zmq::socket_t &)m_socket);
		msg.recv((zmq::socket_t &)m_socket);
		return msg.body();
	}


};

Carbon *pC;

int main(int argc,const char* argv[])
{
	SetConsoleTitle(_T("CTP交易终端 [qq:129769]"));
	if ( argc > 1) {
		std::cout << ">>>连接自定义服务器: " << argv[1] << std::endl;
		pC = new Carbon(argv[1]);
	}
	else{
		std::cout << ">>>连接默认服务器: " << CoreServer << std::endl;
		pC = new Carbon(CoreServer);
	}


	std::cout << pC->getID() << std::endl;
	std::cout << pC->getID() << std::endl;
	std::cout << pC->error(0) << std::endl;
	std::cout << pC->log("ok") << std::endl;
	system("pause");
    return 0;
}

