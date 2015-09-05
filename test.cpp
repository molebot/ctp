// ctp over zmq  by: molebot email: molebot@qq.com
#include "stdafx.h"
#include <iostream>
#include <sstream>
#include "zmsg.hpp"
#include <vector>
#include <string>
#include <map>
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"
// typedef 
typedef TThostFtdcBrokerIDType		BrokerID;
typedef TThostFtdcUserIDType		UserID;
typedef TThostFtdcPasswordType		UserPassword;
typedef	TThostFtdcSessionIDType		SessionID;
typedef TThostFtdcFrontIDType		FrontID;
typedef	TThostFtdcOrderRefType		OrderRef;

// end typedef
std::string CoreServer = "tcp://192.168.1.234:5555";
using std::string;
class Carbon;

class MD : public CThostFtdcMdSpi
{
private:
	Carbon *pC;
public:
	MD(Carbon* _pC):pC(_pC) {}

	///错误应答
	void OnRspError(CThostFtdcRspInfoField *pRspInfo,int nRequestID, bool bIsLast);

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	void OnHeartBeatWarning(int nTimeLapse);

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	void OnFrontConnected();

	///登录请求响应
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行情应答
	void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	///	用户登录
	void	ReqUserLogin();

	///	订阅合约
	void	SubscribeMarketData();

};

class TD : public CThostFtdcTraderSpi
{
private:
	Carbon *pC;
public:
	TD(Carbon* _pC) :pC(_pC) {}
	
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。

	void OnFrontConnected();

	///登录请求响应
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者结算结果确认响应
	void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约响应
	void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询资金账户响应
	void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓响应
	void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单录入请求响应
	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单操作请求响应
	void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	void OnHeartBeatWarning(int nTimeLapse);

	///报单通知
	void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///成交通知
	void OnRtnTrade(CThostFtdcTradeField *pTrade);

	///请求查询报单响应
	void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询成交响应
	void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓明细响应
	void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///用户登录请求
	void ReqUserLogin();

	///投资者结算结果确认
	void ReqSettlementInfoConfirm();

	///请求查询合约
	void ReqQryInstrument();

	///请求查询资金账户
	void ReqQryTradingAccount();

	///请求查询投资者持仓
	void ReqQryInvestorPosition();

	///报单录入请求
	void ReqOrderInsert();

	///报单操作请求
	void ReqOrderAction();

	///报单查询请求
	void ReqQryOrder();

	///成交查询请求
	void ReqQryTrade();

	//请求查询投资者持仓明细
	void ReqQryInvestorPositionDetail();

};


class Carbon 
{
private:
	int			requestID;
	void			*m_socket;	//	for zmq
	void			*m_context;	//	for zmq
	MD			*pmds;	//	Md Spi
	TD			*ptds;	//	Td Spi
	CThostFtdcMdApi		*pmda;	//	Md Api
	CThostFtdcTraderApi	*ptda;	//	Td Api
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
		ptds = new TD(this);
		ptda->RegisterSpi((CThostFtdcTraderSpi*)ptds);
		ptda->SubscribePublicTopic(THOST_TERT_QUICK);
		ptda->SubscribePrivateTopic(THOST_TERT_QUICK);
		ptda->RegisterFront(td_buf);
		
		pmda = CThostFtdcMdApi::CreateFtdcMdApi(".\\mdflow\\");
		pmds = new MD(this);
		pmda->RegisterSpi(pmds);
		pmda->RegisterFront(md_buf);
		
		//  启动初始化
		ptda->Init();	//	触发TD::OnFrontConnected，启动初始化链条
		//初始化链条完毕后pmda->init()；然后一起join
		std::cout << "ptda->init()" << std::endl;

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

	bool isErrorRspInfo(CThostFtdcRspInfoField *pRspInfo){
		bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
		if (ret){
			std::cout << "!!! CTP报错 [ " << error(pRspInfo->ErrorID) << " ] !!!" << std::endl;
		}
		return ret;
	}
};
//===========================================================================================================================

Carbon*	pC;

//===========================================================================================================================
// for MD Spi
void MD::SubscribeMarketData() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void MD::ReqUserLogin() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void MD::OnRspError(CThostFtdcRspInfoField *pRspInfo,int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	pC->isErrorRspInfo(pRspInfo);
};

void MD::OnFrontDisconnected(int nReason) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void MD::OnHeartBeatWarning(int nTimeLapse) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void MD::OnFrontConnected() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void MD::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void MD::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void MD::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void MD::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

//===========================================================================================================================
// for TD Spi
void TD::ReqUserLogin() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::ReqSettlementInfoConfirm() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::ReqQryInstrument() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::ReqQryTradingAccount() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::ReqQryInvestorPosition() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::ReqOrderInsert() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::ReqQryOrder() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::ReqQryTrade() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::ReqQryInvestorPositionDetail() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnFrontConnected() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	pC->isErrorRspInfo(pRspInfo);
};

void TD::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnFrontDisconnected(int nReason) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnHeartBeatWarning(int nTimeLapse) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnRtnOrder(CThostFtdcOrderField *pOrder) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnRtnTrade(CThostFtdcTradeField *pTrade) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void TD::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};


//===========================================================================================================================

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

