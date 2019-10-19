#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>
#define SERVPORT 3333	
static int count;
static void recvfrom_int(int);

void udps_respon(int sockfd,struct sockaddr* pcliaddr,socklen_t clilen)
{
	int n;
  	char msg[1024];
  	socklen_t len;
 	for(;;)
  	{
		len=clilen;
		n=recvfrom(sockfd,msg,1024,0,pcliaddr,&len);
 		/* 响应客户机请求 */
 		msg[n]='\0';
 		printf("recvfrom :%s \n",msg);
 		sendto(sockfd,msg,n,0,pcliaddr,len);
  	}
}

void udps_echo_loop(int sockfd,struct sockaddr* pcliaddr,socklen_t clilen)//缺乏流量控制
{
	char msg[1024];
  	socklen_t len;
  	signal(SIGINT,recvfrom_int);//捕获信号设置
 	for(;;)
  	{
		len=clilen;
		recvfrom(sockfd,msg,1024,0,pcliaddr,&len);
 		count++; 		
  	}
}
static void recvfrom_int(int signo)
{
	printf("\n received %d datagrams \n",count);
	exit(0);
	}
int main(int argc,char*argv[])
{
	int sockfd;
 	struct sockaddr_in servaddr,cliaddr;
 	/* 创建一个UDP数据报类型的套接字 */
 	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
 	{
		fprintf(stderr,"Socket error");
		exit(1);
 	}
	bzero(&servaddr,sizeof(servaddr));
 	servaddr.sin_family=AF_INET;
 	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
 	servaddr.sin_port=htons(SERVPORT);

	/* 服务器为套接字绑定一个端口号 */
 	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
 	{
		fprintf(stderr,"Bind error");
		exit(1);
	}
	/* 调用通信函数与客户端进行通信 */
	udps_respon(sockfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));	 //正常服务器通信处理  
   	//udps_echo_loop(sockfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));	//流量控制   
   	close(sockfd); /* 关闭套接字 */
}
