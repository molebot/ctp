# encoding: UTF-8

"""
该文件中包含的是交易平台的主函数，
将底层、中层、上层的功能导入，并运行。
"""
import ctypes
import sys
import zmq

from demoEngine import MainEngine
from demoUi import *

acc={}
#----------------------------------------------------------------------
def main():
    """主程序入口"""
    context = zmq.Context()

    #  Socket to talk to server
    print("Connecting to core_server...")
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
    acc['symbol'] = (socket.recv(),"SHFE")

    ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID('vn.py demo')  # win7以下请注释掉该行   
    
    app = QtGui.QApplication(sys.argv)
    app.setWindowIcon(QtGui.QIcon('ico.ico'))
    
    me = MainEngine()
    me.set_socket(socket)
    me.set_symbol(acc['symbol'])
    me.login(acc['account'],acc['password'],acc['brokerid'],acc['mdfront'],acc['tdfront'])
        
    mw = MainWindow(me.ee, me)
    mw.showMaximized()
    
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
    
