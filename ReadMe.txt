中期ctp通过zmq连外部服务器

需要用到的lib:
libzmq
CThostFtdcMdApi
CThostFtdcTraderApi

控制逻辑全部在Carbon类中
ctp账户信息通过zmq从外部服务器获取(详见Carbon::init函数)

author:
molebot@qq.com

ENJOY IT
