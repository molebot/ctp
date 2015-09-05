// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include "zmsg.hpp"
#include <vector>
#include <string>
#include <map>
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"

std::string CoreServer = "tcp://192.168.1.234:5555";

using std::string;

class MD : public CThostFtdcMdSpi
{
public:

	///错误应答
	void OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		IsErrorRspInfo(pRspInfo);
	}

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	void OnFrontDisconnected(int nReason) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	void OnHeartBeatWarning(int nTimeLapse) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	void OnFrontConnected() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///登录请求响应
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///订阅行情应答
	void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///取消订阅行情应答
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///深度行情通知
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}



public:

	///	用户登录
	void	ReqUserLogin() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}
	///	订阅合约
	void	SubscribeMarketData() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}
	///	读取错误信息
	bool	IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		return true;
	}

};

class TD : public CThostFtdcTraderSpi
{
public:

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	void OnFrontConnected() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///登录请求响应
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///投资者结算结果确认响应
	void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///请求查询合约响应
	void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///请求查询资金账户响应
	void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///请求查询投资者持仓响应
	void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///报单录入请求响应
	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///报单操作请求响应
	void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///错误应答
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	void OnFrontDisconnected(int nReason) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	void OnHeartBeatWarning(int nTimeLapse) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///报单通知
	void OnRtnOrder(CThostFtdcOrderField *pOrder) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///成交通知
	void OnRtnTrade(CThostFtdcTradeField *pTrade) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///请求查询报单响应
	void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///请求查询成交响应
	void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///请求查询投资者持仓明细响应
	void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

public:
	///用户登录请求
	void ReqUserLogin() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///投资者结算结果确认
	void ReqSettlementInfoConfirm() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///请求查询合约
	void ReqQryInstrument() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///请求查询资金账户
	void ReqQryTradingAccount() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///请求查询投资者持仓
	void ReqQryInvestorPosition() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///报单录入请求
	void ReqOrderInsert() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///报单操作请求
	void ReqOrderAction(CThostFtdcRspInfoField *pRspInfo) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	////错误响应
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		return true;
	}

	///报单查询请求
	void ReqQryOrder() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	///成交查询请求
	void ReqQryTrade() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

	//请求查询投资者持仓明细
	void ReqQryInvestorPositionDetail() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}

};


class Carbon {
private:
	int				requestID;
	void			*m_socket;
	void			*m_context;
	MD				*pmds;
	TD				*ptds;
	CThostFtdcMdApi	*pmda;
	CThostFtdcTraderApi		*ptda;
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

		char *md_buf = new char[strlen(md_front.c_str()) + 1];
		strcpy(md_buf, md_front.c_str());

		char *td_buf = new char[strlen(td_front.c_str()) + 1];
		strcpy(td_buf, td_front.c_str());

		ptda = CThostFtdcTraderApi::CreateFtdcTraderApi(".\\tdflow\\");
		ptds = new TD();
		ptda->RegisterSpi((CThostFtdcTraderSpi*)ptds);
		ptda->SubscribePublicTopic(THOST_TERT_QUICK);
		ptda->SubscribePrivateTopic(THOST_TERT_QUICK);
		ptda->RegisterFront(td_buf);
		pmda = CThostFtdcMdApi::CreateFtdcMdApi(".\\mdflow\\");
		pmds = new MD();
		pmda->RegisterSpi(pmds);
		pmda->RegisterFront(md_buf);
		//  启动初始化
		ptda->Init();
		std::cout << "ptda->init()" << std::endl;
		ptda->Join();
		std::cout << "ptda->join()" << std::endl;
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
		zmsg _msg(s.c_str());
		_msg.send((zmq::socket_t &)m_socket);
		_msg.recv((zmq::socket_t &)m_socket);
		return _msg.body();
	}


};

Carbon*					pC;

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


	system("pause");
    return 0;
}

