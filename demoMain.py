# encoding: UTF-8

"""
该文件中包含的是交易平台的主函数，
将底层、中层、上层的功能导入，并运行。
"""
import logging
import win32api, win32gui
ct = win32api.GetConsoleTitle()
hd = win32gui.FindWindow(0,ct)
#win32gui.ShowWindow(hd,0)
logger = logging.getLogger("log")
hfile = logging.FileHandler("log.txt")
logger.addHandler(hfile)

import ctypes
import sys
import zmq
from datetime import datetime
from time import sleep

from demoEngine import MainEngine
from demoUi import *

acc={}
#----------------------------------------------------------------------
def main():
    """主程序入口"""
    args = sys.argv[1:]
    
    context = zmq.Context()

    socket = context.socket(zmq.REQ)
    socket.connect("tcp://192.168.1.234:9999")
    
    socket.send(b"md_front")
    acc['mdfront'] = socket.recv()

    socket.send(b"td_front")
    acc['tdfront'] = socket.recv()

    socket.send(b"broker_id")
    acc['brokerid'] = socket.recv()

    socket.send(b"account_num")
    acc['account'] = socket.recv()

    socket.send(b"account_pwd")
    acc['password'] = socket.recv()

    socket.send(b"this_symbol")
    acc['symbol'] = socket.recv()

    ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID('vn.py demo')  # win7以下请注释掉该行   
    
    app = QtGui.QApplication(sys.argv)
    app.setWindowIcon(QtGui.QIcon('ico.ico'))
    
    me = MainEngine()
    me.set_app(app)
    me.set_socket(socket)
    me.set_symbol(acc['symbol'])
    me.login(acc['account'],acc['password'],acc['brokerid'],acc['mdfront'],acc['tdfront'])
        
    mw = MainWindow(me.ee, me)
    mw.showMaximized()
    
    sys.exit(app.exec_())

def isTrade(_d):
    if _d.hour==9 and _d.minute>=15:
        return True
    elif _d.hour==15 and _d.minute<15:
        return True
    elif _d.hour==11 and _d.minute<30:
        return True
    elif _d.hour in [10,13,14]:
        return True
    else:
        return False

if __name__ == '__main__':
    _now = datetime.now()
    while not isTrade(_now):
        sleep(1)
        print(_now)
        _now = datetime.now()

    while 1:
        try:
            main()
        finally:
            print("exit...")
