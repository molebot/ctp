# encoding: UTF-8

"""
该文件中包含的是交易平台的中间层，
将API和事件引擎包装到一个主引擎类中，便于管理。

当客户想采用服务器-客户机模式，实现交易功能放在托管机房，
而图形控制功能在本地电脑时，该主引擎负责实现远程通讯。
"""
import sys
from datetime import date
from time import sleep,time
import shelve

from PyQt4 import QtCore

from demoApi import *
from eventEngine import EventEngine


########################################################################
class MainEngine:
    """主引擎，负责对API的调度"""

    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor
        :type self: object
        """
        self.ee = EventEngine()         # 创建事件驱动引擎
        
        self.md = DemoMdApi(self.ee)    # 创建API接口
        self.td = DemoTdApi(self.ee)
        
        self.ee.start()                 # 启动事件驱动引擎
        self.havedposi = False
        self.position = {}
        self.todayposition = {}
        
        self.__orders = {}
        self.__retry = 0
        # 循环查询持仓和账户相关
        self.countGet = 0               # 查询延时计数
        self.lastGet = 'Account'        # 上次查询的性质
        self.ee.register(EVENT_TDLOGIN, self.initGet)  # 登录成功后开始初始化查询
        
        # 合约储存相关
        self.dictInstrument = {}        # 字典（保存合约查询数据）
        self.ee.register(EVENT_INSTRUMENT, self.insertInstrument)
        
        self.ee.register(EVENT_TRADE_DATA, self.get_trade)
        self.ee.register(EVENT_ORDER_DATA, self.get_order)
        self.ee.register(EVENT_ERROR, self.get_error)
        self.ee.register(EVENT_MARKETDATA_DATA, self.get_data)
        self.ee.register(EVENT_POSITION_DATA, self.get_position)
        
    def set_app(self,_app):self.app = _app
    def get_order(self,event):
        _data = event.dict_['data']
        if _data['OrderStatus'] == '5':
            self.__retry += 1
            _saved = self.__orders.pop(int(_data['OrderRef'])) 
            if self.__retry>5:
                self.__retry = 0
                return 0
            print("Plus...Order")
            if _saved[6] == defineDict['THOST_FTDC_OF_Open']:
                _tr = 1
            elif _saved[6] == defineDict['THOST_FTDC_OF_Close']:
                _tr = -1
            else:
                _tr = 0
            if _saved[5] == defineDict["THOST_FTDC_D_Buy"]:
                _kr = 1
            elif _saved[5] == defineDict["THOST_FTDC_D_Sell"]:
                _kr = -1
            else:
                _kr = 0
            if _tr*_kr>0:
                price = float(_saved[2])+0.2
            else:
                price = float(_saved[2])-0.2
            _ref = self.td.sendOrder(_saved[0],_saved[1],price,_saved[3],_saved[4],_saved[5],_saved[6])
            self.__orders[_ref] = (_saved[0],_saved[1],price,_saved[3],_saved[4],_saved[5],_saved[6])
    def get_trade(self,event):
        _data = event.dict_['data']
        print('get_trade',_data['OrderRef'])
        _done = _data['Volume']
        if int(_data['OrderRef']) in self.__orders:
            _saved = self.__orders.pop(int(_data['OrderRef']))
            _goon = _saved[4] - _done
        else:
            _goon = 0
        if _goon != 0:
            self.__retry += 1
            if self.__retry>5:
                self.__retry = 0
                return 0
            print("Plus...Trade")
            if _saved[6] == defineDict['THOST_FTDC_OF_Open']:
                _tr = 1
            elif _saved[6] == defineDict['THOST_FTDC_OF_Close']:
                _tr = -1
            else:
                _tr = 0
            if _saved[5] == defineDict["THOST_FTDC_D_Buy"]:
                _kr = 1
            elif _saved[5] == defineDict["THOST_FTDC_D_Sell"]:
                _kr = -1
            else:
                _kr = 0
            if _tr*_kr>0:
                price = float(_saved[2])+0.2
            else:
                price = float(_saved[2])-0.2
            _ref = self.td.sendOrder(_saved[0],_saved[1],price,_saved[3],_goon,_saved[5],_saved[6])
            self.__orders[_ref] = (_saved[0],_saved[1],price,_saved[3],_goon,_saved[5],_saved[6])
    def set_symbol(self,_s):
        self.symbol = _s
    def set_socket(self,_s):
        self.socket = _s
    def get_position(self,event):
        _data = event.dict_['data']
        if _data['TodayPosition']:
            self.todayposition[_data['PosiDirection']] = _data['TodayPosition']
        if _data['Position']:pass
#            self.position[_data['PosiDirection']] = _data['Position']
        self.havedposi = True
        self.__orders = {}
    def openPosition(self,tr,volume):
        self.__retry = 0
        offset = defineDict['THOST_FTDC_OF_Open']
        pricetype = defineDict['THOST_FTDC_OPT_LimitPrice']
        if tr>0:
            price = self.ask+0.2*2.0
            direction = defineDict["THOST_FTDC_D_Buy"]
        else:   
            price = self.bid-0.2*2.0
            direction = defineDict["THOST_FTDC_D_Sell"]
        _ref = self.td.sendOrder(self.symbol,self.exchangeid,price,pricetype,volume,direction,offset)
        self.__orders[_ref] = (self.symbol,self.exchangeid,price,pricetype,volume,direction,offset)
    def closePosition(self,tr,volume):
        self.__retry = 0
        offset = defineDict['THOST_FTDC_OF_Close']
        pricetype = defineDict['THOST_FTDC_OPT_LimitPrice']
        if tr<0:
            price = self.ask+0.2*2.0
            direction = defineDict["THOST_FTDC_D_Buy"]
        else:   
            price = self.bid-0.2*2.0
            direction = defineDict["THOST_FTDC_D_Sell"]
        _ref = self.td.sendOrder(self.symbol,self.exchangeid,price,pricetype,volume,direction,offset)
        self.__orders[_ref] = (self.symbol,self.exchangeid,price,pricetype,volume,direction,offset)
    def closeTodayPosition(self,tr,volume):
        self.__retry = 0
        offset = defineDict['THOST_FTDC_OF_CloseToday']
        pricetype = defineDict['THOST_FTDC_OPT_LimitPrice']
        if tr<0:
            price = self.ask+0.2*2.0
            direction = defineDict["THOST_FTDC_D_Buy"]
        else:   
            price = self.bid-0.2*2.0
            direction = defineDict["THOST_FTDC_D_Sell"]
        _ref = self.td.sendOrder(self.symbol,self.exchangeid,price,pricetype,volume,direction,offset)
        self.__orders[_ref] = (self.symbol,self.exchangeid,price,pricetype,volume,direction,offset)
    def get_error(self,event):
        _err = event.dict_['ErrorID']
        self.socket.send(bytes("error_%s"%_err))
        _bk = self.socket.recv()
        print("ERROR",event.dict_)
        self.__orders = {}
    def get_data(self,event):
        _data = event.dict_['data']
        self.ask = _data['AskPrice1']
        self.bid = _data['BidPrice1']
        price = (self.ask+self.bid)/2.0
        self.socket.send(bytes(str(price)))
        _bk = int(self.socket.recv())
        self.todo = _bk
#        print '%.0f  %s  =  %d'%(time(),_data['LastPrice'],_bk)
        if self.__orders:
            print(self.__orders)
        elif self.havedposi:
            _long = defineDict["THOST_FTDC_PD_Long"]
            _short = defineDict["THOST_FTDC_PD_Short"]
            if self.todo==0:
                if self.position.get(_long,0)>0:
                    self.closePosition(1,self.position[_long])
                    self.havedposi = False
                if self.todayposition.get(_long,0)>0:
                    self.closeTodayPosition(1,self.todayposition[_long])
                    self.havedposi = False
                    
                if self.position.get(_short,0)>0:
                    self.closePosition(-1,self.position[_short])
                    self.havedposi = False
                if self.todayposition.get(_short,0)>0:
                    self.closeTodayPosition(-1,self.todayposition[_short])
                    self.havedposi = False
                    
            def do_it(_todo,_pass,_reverse,d_pass,d_reverse):
                if self.position.get(_reverse,0)>0:
                    self.closePosition(d_reverse,self.position[_reverse])
                    self.havedposi = False
                if self.todayposition.get(_reverse,0)>0:
                    self.closeTodayPosition(d_reverse,self.todayposition[_reverse])
                    self.havedposi = False
                _haved = self.position.get(_pass,0)+self.todayposition.get(_pass,0)
                if _todo>_haved:
                    self.openPosition(d_pass,_todo-_haved)
                    self.havedposi = False
                if _todo<_haved:
                    if self.position.get(_pass,0)>0:
                        self.closePosition(d_pass,min(self.position[_pass],_haved-_todo))
                        self.havedposi = False
                        if self.position[_pass]<_haved-_todo:
                            self.closeTodayPosition(d_pass,_haved-_todo-self.position[_pass])
                            self.havedposi = False
                    elif self.todayposition.get(_pass,0)>0:
                        self.closeTodayPosition(d_pass,_haved-_todo)
                        self.havedposi = False

            if self.todo>0:
                _todo = self.todo
                _pass = _long
                _reverse = _short
                d_pass = 1
                d_reverse = -1

                do_it(_todo,_pass,_reverse,d_pass,d_reverse)
                
            if self.todo<0:
                _todo = abs(self.todo)
                _pass = _short
                _reverse = _long
                d_pass = -1
                d_reverse = 1

                do_it(_todo,_pass,_reverse,d_pass,d_reverse)
                
            if not self.havedposi:
                self.todayposition = {}
                self.position = {}
    #----------------------------------------------------------------------
    def login(self, userid, password, brokerid, mdAddress, tdAddress):
        """登陆"""
        self.td.login(tdAddress, userid, password, brokerid)
        self.md.login(mdAddress, userid, password, brokerid)
    
    #----------------------------------------------------------------------
    def subscribe(self, instrumentid, exchangeid):
        """订阅合约"""
        self.md.subscribe(instrumentid, exchangeid)
        
    #----------------------------------------------------------------------
    def getAccount(self):
        """查询账户"""
        self.td.getAccount()
        
    #----------------------------------------------------------------------
    def getInvestor(self):
        """查询投资者"""
        self.td.getInvestor()
        
    #----------------------------------------------------------------------
    def getPosition(self):
        """查询持仓"""
        self.td.getPosition()
    
    #----------------------------------------------------------------------
    def getInstrument(self):
        """获取合约"""
        event = Event(type_=EVENT_LOG)
        log = u'查询合约信息'
        event.dict_['log'] = log
        self.ee.put(event)          
        
        self.td.getInstrument()
        
    #----------------------------------------------------------------------
    def sendOrder(self, instrumentid, exchangeid, price, pricetype, volume, direction, offset):
        """发单"""
        self.td.sendOrder(instrumentid, exchangeid, price, pricetype, volume, direction, offset)
        
    #----------------------------------------------------------------------
    def cancelOrder(self, instrumentid, exchangeid, orderref, frontid, sessionid):
        """撤单"""
        self.td.cancelOrder(instrumentid, exchangeid, orderref, frontid, sessionid)
        
    #----------------------------------------------------------------------
    def getAccountPosition(self, event):
        """循环查询账户和持仓"""
        self.countGet = self.countGet + 1
        
        # 每1秒发一次查询
        if self.countGet >= 20:
            self.countGet = 0
            if self.lastGet == 'Account':
                self.getPosition()
                self.lastGet = 'Position'
            else:
                self.lastGet = 'Account'
                self.getAccount()
    #----------------------------------------------------------------------
    def initGet(self, event):
        """在交易服务器登录成功后，开始初始化查询"""
        # 打开设定文件setting.vn
        f = shelve.open('setting.vn')
        
        # 尝试读取设定字典，若该字典不存在，则发出查询请求
        try:
            d = f['instrument']
            
            # 如果本地保存的合约数据是今日的，则载入，否则发出查询请求
            today = date.today()
            if d['date'] == today:
                self.dictInstrument = d['dictInstrument']
                
                event = Event(type_=EVENT_LOG)
                log = u'合约信息读取完成'
                event.dict_['log'] = log
                self.ee.put(event)            
    
                self.getInvestor()
                
                # 开始循环查询
                self.ee.register(EVENT_TIMER, self.getAccountPosition)                 
            else:
                self.getInstrument()
        except KeyError:
            self.getInstrument()
            
        f.close()
#        _exchangeid = self.dictInstrument[self.symbol]['ExchangeID']
        self.exchangeid = ""#_exchangeid
        self.subscribe(self.symbol,self.exchangeid)
    #----------------------------------------------------------------------
    def insertInstrument(self, event):
        """插入合约对象"""
        data = event.dict_['data']
        last = event.dict_['last']
        
        self.dictInstrument[data['InstrumentID']] = data
        
        # 合约对象查询完成后，查询投资者信息并开始循环查询
        if last:
            # 将查询完成的合约信息保存到本地文件，今日登录可直接使用不再查询
            self.saveInstrument()
            
            event = Event(type_=EVENT_LOG)
            log = u'合约信息查询完成'
            event.dict_['log'] = log
            self.ee.put(event)            

            self.getInvestor()
            # 开始循环查询
            self.ee.register(EVENT_TIMER, self.getAccountPosition)              
        
    #----------------------------------------------------------------------
    def selectInstrument(self, instrumentid):
        """获取合约信息对象"""
        try:
            instrument = self.dictInstrument[instrumentid]
        except KeyError:
            instrument = None
        return instrument
    
    #----------------------------------------------------------------------
    def exit(self):
        """退出"""
        # 销毁API对象
        self.td = None
        self.md = None
        
        # 停止事件驱动引擎
        self.ee.stop()
        
    #----------------------------------------------------------------------
    def saveInstrument(self):
        """保存合约属性数据"""
        f = shelve.open('setting.vn')
        d = {}
        d['dictInstrument'] = self.dictInstrument
        d['date'] = date.today()
        f['instrument'] = d
        f.close()
