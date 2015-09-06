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

	///投资者结算结果请求响应
	void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

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

};


class Carbon 
{
private:
	int			requestId;
	int			orderRef;
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
	int			frontId;
	int			sessionId;

public:
	Carbon(std::string _Server) {
		m_context	= zmq_ctx_new();
		m_socket	= zmq_socket(m_context, ZMQ_REQ);
		zmq_connect(m_socket, _Server.c_str());
		requestId = 0;
		orderRef = 0;
		init();
	}

	~Carbon() {
		if (m_socket != NULL) {
			zmq_close(m_socket);
			m_socket = NULL;
		}
	}

	void join() {
		pmda->Join();
		ptda->Join();
	}

	int init() {
		md_front = msg("md_front");
		std::cout << "===获取行情地址[MD_Front]===" << std::endl;
		std::cout << md_front << std::endl;
		td_front = msg("td_front");
		std::cout << "===获取交易地址[TD_Front]===" << std::endl;
		std::cout << td_front << std::endl;
		brokerid = msg("broker_id");
		std::cout << "===获取座席编号[BrokerID]===" << std::endl;
		std::cout << brokerid << std::endl;
		accountNum = msg("account_num");
		std::cout << "===获取账号[InvestorID]===" << std::endl;
		std::cout << accountNum << std::endl;
		accountPwd = msg("account_pwd");
		std::cout << "===获取密码[Password]===" << std::endl;
		std::cout << "ok!" << std::endl;
		symbol = msg("this_symbol");
		std::cout << "===设定合约===" << std::endl;
		std::cout << symbol << std::endl;

		char *md_buf = new char[strlen(md_front.data()) + 1];
		strcpy(md_buf, md_front.c_str());

		char *td_buf = new char[strlen(td_front.data()) + 1];
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
		std::cout << "TD 开始初始化..." << std::endl;

		return 0;
	}

	int get_requestId() {
		return ++requestId;
	}

	int get_orderRef() {
		return ++orderRef;
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
		zmsg _msg(s.data());
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

	void tdReqUserLogin() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		CThostFtdcReqUserLoginField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, brokerid.data());
		strcpy(req.UserID, accountNum.data());
		strcpy(req.Password, accountPwd.data());
		int ret = ptda->ReqUserLogin(&req, get_requestId());
		std::cout << "TD 发送登录请求: " << ((ret == 0) ? "成功" : "失败") << std::endl;
	}

	void tdOnRspUserLogin(CThostFtdcRspUserLoginField *p) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		frontId = p->FrontID;
		sessionId = p->SessionID;
		orderRef = atoi(p->MaxOrderRef);
		std::cout << "TD 登录成功 >>> FrontID: " << frontId << " SessionID: " << sessionId << " OrderRef: " << orderRef << std::endl;
		tdReqQrySettlementInfoConfirm();
	}

	void tdReqQrySettlementInfoConfirm() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		CThostFtdcQrySettlementInfoConfirmField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, brokerid.data());
		strcpy(req.InvestorID, accountNum.data());
		int ret = ptda->ReqQrySettlementInfoConfirm(&req, get_requestId());
		std::cout << "TD 发送结算单确认请求: " << ((ret == 0) ? "成功" : "失败") << std::endl;
	}

	void tdOnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *p) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		std::cout << "TD 结算单确认成功:" << std::endl;
		std::cout << "结算单: " << p->ConfirmDate << p->ConfirmTime << " 已确认" << std::endl;
		tdReqQryInvestorPosition();
	}

	void tdReqQryTradingAccount() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		CThostFtdcQryTradingAccountField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, brokerid.data());
		strcpy(req.InvestorID, accountNum.data());
		int ret = ptda->ReqQryTradingAccount(&req, get_requestId());
		std::cout << "TD 发送帐户查询请求: " << ((ret == 0) ? "成功" : "失败") << std::endl;
	}

	void tdOnRspQryTradingAccount(CThostFtdcTradingAccountField *p) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		std::cout << "TD 帐户查询成功:" << std::endl;
		std::cout << "可用资金: " << p->Available << std::endl;
		tdReqQryInvestorPosition();
	}

	void tdReqQryInvestorPosition() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		CThostFtdcQryInvestorPositionField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, brokerid.data());
		strcpy(req.InvestorID, accountNum.data());
		strcpy(req.InstrumentID, symbol.data());
		int ret = ptda->ReqQryInvestorPosition(&req, get_requestId());
		std::cout << "TD 发送持仓查询请求: " << ((ret == 0) ? "成功" : "失败") << std::endl;
	}

	void tdOnRspQryInvestorPosition(CThostFtdcInvestorPositionField *p) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		std::cout << "TD 持仓查询成功:" << std::endl;
		std::cout << "今日持仓: " << p->TodayPosition << std::endl;
		std::cout << "TD 初始化结束..." << std::endl;
		std::cout << "MD 开始初始化..." << std::endl;
		pmda->Init();
	}

	void mdReqUserLogin() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		CThostFtdcReqUserLoginField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, brokerid.data());
		strcpy(req.UserID, accountNum.data());
		strcpy(req.Password, accountPwd.data());
		int ret = pmda->ReqUserLogin(&req, get_requestId());
		std::cout << "MD 发送登录请求: " << ((ret == 0) ? "成功" : "失败") << std::endl;
	}

	void mdOnRspUserLogin(CThostFtdcRspUserLoginField *p) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		std::cout << "MD 登录成功..." << std::endl;
		mdSubscribeMarketData();
	}

	void mdSubscribeMarketData() {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		int len = 1;
		char** pInstId = new char*[len];
		char *out = new char[strlen(symbol.data()) + 1];
		strcpy(out,symbol.data());
		for (int i = 0; i < len; i++)	pInstId[i] = out;
		int ret = pmda->SubscribeMarketData(pInstId, len);
		std::cout << "MD 发送订阅行情请求: " << ((ret == 0) ? "成功" : "失败") << std::endl;
	}

	void mdOnRspSubMarketData(CThostFtdcSpecificInstrumentField *p) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
		std::cout << "MD 订阅行情成功..." << std::endl;
	}

	void mdOnRtnDepthMarketData(CThostFtdcDepthMarketDataField *p) {
		std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	}
};
//===========================================================================================================================

Carbon*	pC;

//===========================================================================================================================
// for MD Spi
void MD::OnRspError(CThostFtdcRspInfoField *pRspInfo,int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	pC->isErrorRspInfo(pRspInfo);
};

void MD::OnFrontDisconnected(int nReason) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	std::cout << "!!! 客户端连接中断..." << std::endl;
};

void MD::OnHeartBeatWarning(int nTimeLapse) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	std::cout << "!!! 心跳超时警告..." << std::endl;
};

void MD::OnFrontConnected() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	pC->mdReqUserLogin();
};

void MD::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	if (!pC->isErrorRspInfo(pRspInfo) && pRspUserLogin) {
		pC->mdOnRspUserLogin(pRspUserLogin);
	}
};

void MD::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	if (!pC->isErrorRspInfo(pRspInfo)) {
		pC->mdOnRspSubMarketData(pSpecificInstrument);
	}
};

void MD::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

void MD::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	pC->mdOnRtnDepthMarketData(pDepthMarketData);
};

//===========================================================================================================================
// for TD Spi
//	初始化链条
void TD::OnFrontConnected() {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	pC->tdReqUserLogin();
};
//	初始化链条
void TD::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	if (!pC->isErrorRspInfo(pRspInfo) && pRspUserLogin){
		pC->tdOnRspUserLogin(pRspUserLogin);
	}
};

void TD::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};

//	初始化链条
void TD::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	if (!pC->isErrorRspInfo(pRspInfo) && pSettlementInfoConfirm) {
		pC->tdOnRspSettlementInfoConfirm(pSettlementInfoConfirm);
	}
};

void TD::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};
//	初始化链条
void TD::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	if (!pC->isErrorRspInfo(pRspInfo) && pTradingAccount) {
		pC->tdOnRspQryTradingAccount(pTradingAccount);
	}
};
//	初始化链条
void TD::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	if (!pC->isErrorRspInfo(pRspInfo) && pInvestorPosition) {
		pC->tdOnRspQryInvestorPosition(pInvestorPosition);
	}
};

void TD::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	pC->isErrorRspInfo(pRspInfo);
};

void TD::OnFrontDisconnected(int nReason) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	std::cout << "!!! 客户端连接中断..." << std::endl;
};

void TD::OnHeartBeatWarning(int nTimeLapse) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
	std::cout << "!!! 心跳超时警告..." << std::endl;
};
///	报单查询响应
void TD::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};
///	报单响应
void TD::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};
///	撤单响应
void TD::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};
///	保单回报
void TD::OnRtnOrder(CThostFtdcOrderField *pOrder) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};
///	成交回报
void TD::OnRtnTrade(CThostFtdcTradeField *pTrade) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};
///	成交查询响应
void TD::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "===#" << __FUNCTION__ << "#===" << std::endl;
};
///	持仓详细查询响应
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

	pC->join();
	system("pause");
    return 0;
}

