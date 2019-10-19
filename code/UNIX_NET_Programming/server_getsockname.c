//验证getsockname,getpeername
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
#include <time.h>
#define BUF_LEN 1024
#define BACKLOG 10 
int main(void)
{
	//初始化
	int fd=0;
	int r=0;
	char buf[BUF_LEN];

      
	time_t now;
	char *str_now;

	//网络服务器初始化
	int sock_fd,client_fd;//sock_fd:监听socket;client_fd:数据传输socket
	struct sockaddr_in my_addr;//本地地址 
	struct sockaddr_in remote_addr;//客户地址信息 
	struct sockaddr_in client_addr;//客户地址信息 
	
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(33333);
	my_addr.sin_addr.s_addr=INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);
	
	if (-1==(sock_fd=socket(PF_INET,SOCK_STREAM,0)))
	{	
		printf("socket 创建错误 ！");
		return -1;
	}
	if(-1==bind(sock_fd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr)))
	{
		printf("bind 出错！");	
		return -1;
	}
		if(-1==listen(sock_fd,BACKLOG))
		{
			printf("listen出错!");	
			return -1;
		}
		
	
		while(1)
		{
			int sin_size=sizeof(struct sockaddr_in);
			int re;
			if(-1==(client_fd=accept(sock_fd,(struct sockaddr *)&remote_addr,&sin_size)))
			{
				printf("accept 出错！");
				continue;
			}
			printf("received a connection from %s \n",inet_ntoa(remote_addr.sin_addr));
		
			if(!fork())
			{
				socklen_t len;
				len=sizeof(client_addr);
				if(getpeername(client_fd,(struct sockaddr*)&client_addr,&len))
				return(-1);
				printf("client addr: IP:%s port:%d \n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
				
				close(client_fd);
				exit(0);
			}
			close(client_fd);
		}
		return 0;
}

