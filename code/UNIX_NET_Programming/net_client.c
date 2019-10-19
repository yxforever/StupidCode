//P14 并发服务器与重复服务器的获取服务器时间，客户
#include<fcntl.h>
#include <stdio.h>
#include <string.h>
//network init 
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>

#define BUF_LEN 1024		//设备读取缓存的大小
#define SERVPORT 3333	//服务器监听段口号
#define BACKLOG 10		//最大同时连接请求数

int main(int argc,char*argv[])
{
	//读取设备初始化
	int fd=0;
	int r=0;
	char buf[BUF_LEN];
	char IP[16];
	//char stderr[64];
	
	//网络服务器初始化
	int client_fd;//socket;client_fd:数据传输socket
	struct sockaddr_in my_addr;//本地地址 
	struct sockaddr_in remote_addr;//服务器地址信息 
	struct hostent *host;
	
	sprintf(IP,"127.0.0.1");
	printf("IP:%s\n",IP);
	
	if((host=gethostbyname(IP))==NULL)//(argv[1]))==NULL) 
	 { 
       		printf("Gethostname error\n"); 
       		 exit(1); 
	} 

	
	if (-1==(client_fd=socket(AF_INET,SOCK_STREAM,0)))
	{	
		printf("socket 创建错误 ！");
		 exit(1);
	}
	
	
	
	
//填充服务端地址信息

	remote_addr.sin_family=AF_INET;
	remote_addr.sin_port=htons(SERVPORT);
	remote_addr.sin_addr=*((struct in_addr *)host->h_addr); 
	bzero(&(remote_addr.sin_zero),8);
	
	if(connect(client_fd,(struct sockaddr *)(&remote_addr),sizeof(struct sockaddr))==-1) 
	{ 
	  printf("Connect Error\a\n"); 
	  exit(1); 
	} 	
	
	if ((r=recv(client_fd,buf,BUF_LEN,0)) == -1) 		
	{ 
	        printf("Read Error\n"); 
	        exit(1); 
	 } 
	 
 	buf[r]='\0'; 
 	printf("I have received:%s\n",buf); 
 	
 	
 	if(-1==send(client_fd,"RD",2,0))//准备接收数据，发送信息请求时间信息
	printf("send 出错！");
 	
 	if ((r=recv(client_fd,buf,BUF_LEN,0)) == -1) 		
	{ 
	        printf("Read Time Error:\n"); 
	        exit(1); 
	 } 
	 
 	buf[r]='\0'; 
 	printf("Server Time is :%s\n",buf); 
 	printf("print 'Enter' to end socket!\n"); 	
 	getchar();
 
/* 结束通信 */ 
 close(client_fd); 




	
	exit(0);
	
}
