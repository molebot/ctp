
ctp over zmq
============

对上期的ctp协议进行封装，通过zmq获取核心策略服务器的交易指令

核心策略服务器返回:
正值  持对应数量多单
0     空仓
负值  持对应数量空单

由此方便用其他语言及架构开发自动交易策略。


```xml
   default zmq server:
        192.168.1.234:9999

   对zmq的详细用法，请参考源码　demoMain.py
```

注意事项：
    不要使用zmq默认端口


Main features
=============

自动开平仓，自动补仓
断线重连


Thanks
======

vnpy    https://github.com/vnpy/vnpy
