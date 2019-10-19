#This is a makefile

CNDTN=error.o readline.o readn.o writen.o wrapstdio.o wrapsock.o
#时间服务器和客户机
daytimetcpcli:$(CNDTN) daytimetcpcli.o
	gcc $(CNDTN) daytimetcpcli.o -o daytimetcpcli

daytimetcpsrv:$(CNDTN) daytimetcpsrv.o
	gcc $(CNDTN) daytimetcpsrv.o -o daytimetcpsrv

#回射服务器客户机
tcpcli01:$(CNDTN) str_cli.o tcpcli01.o
	gcc $(CNDTN) str_cli.o tcpcli01.o -o tcpcli01

tcpserv01:$(CNDTN) str_echo.o tcpserv01.o
	gcc $(CNDTN) str_echo.o tcpserv01.o -o tcpserv01

#实验2
Pro2Cli:$(CNDTN) Pro2str_cli.o Pro2Cli.o
	gcc $(CNDTN) Pro2str_cli.o Pro2Cli.o -o Pro2Cli

Pro2Serv:$(CNDTN) Pro2Serv.o Pro2str_write.o 
	gcc $(CNDTN) Pro2Serv.o Pro2str_write.o -o Pro2Serv

#select 多路复用
tcpservselect01:$(CNDTN) tcpservselect01.o
	gcc $(CNDTN)  tcpservselect01.o -o tcpservselect01

tcpcliselect01:$(CNDTN) strcliselect02.o tcpcli01.o
	gcc $(CNDTN) strcliselect02.o tcpcli01.o -o tcpcliselect01
	
#udp 回射客户端
udpcli01:$(CNDTN) dg_cli.o udpcli01.o
	gcc $(CNDTN) dg_cli.o udpcli01.o -o udpcli01
	
#回射多路复用实现TCP和UDP回射服务程序,使用上面的回射客户端UDP和TCP
udpservselect01:$(CNDTN) str_echo.o udpservselect01.o sigchldwaitpid.o signal.o
	gcc $(CNDTN) str_echo.o udpservselect01.o sigchldwaitpid.o signal.o -o udpservselect01

#带wait处理的服务器
tcpserv01_wait:$(CNDTN) str_echo.o tcpserv01_wait.o signal.o
	gcc $(CNDTN) str_echo.o tcpserv01_wait.o signal.o -o tcpserv01_wait
#客户端创建5个套接口并建立连接
tcpcli04:$(CNDTN) str_cli.o tcpcli04.o
	gcc $(CNDTN) str_cli.o tcpcli04.o -o tcpcli04
	
#信号驱动IO的UDP回射服务器程序（客户端为UDP回射客户端）
#gcc udpserv01.c signal.c error.c readline.c readn.c writen.c wrapstdio.c wrapsock.c dgecho01.c -o udpserv01
udpserv01:udpserv01.o signal.o error.o readline.o readn.o writen.o wrapstdio.o wrapsock.o dgecho01.o
gcc udpserv01.o signal.o error.o readline.o readn.o writen.o wrapstdio.o wrapsock.o dgecho01.o -o udpserv01

#udp 广播回射客户端
udpclibcast:$(CNDTN) dg_clibcast.o udpclibcast.o signal.o sock_ntop_host.o
	gcc $(CNDTN) dg_clibcast.o udpclibcast.o signal.o sock_ntop_host.o -o udpclibcast
	
#实验5
PR5cli:$(CNDTN)PR5cli.o
	gcc $(CNDTN) PR5cli.o -o PR5cli
PR5serv:$(CNDTN) PR5serv.o
	gcc $(CNDTN) PR5serv.o -o PR5serv
	
%.o:%.c
	gcc -c -o $@ $<

clean:
	rm -f *.o