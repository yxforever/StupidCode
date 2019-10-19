
#include <string.h>
//network init
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
 
#define BUF_LEN 1024		//设备读取缓存的大小
#define SERVPORT 3333	//服务器监听段口号
#define BACKLOG 10		//最大同时连接请求数

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
	
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(SERVPORT);
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
	//printf("print 'I' to select iterative server!\nprint 'C' to select concurrent server!\n"); 	//I重复服务器，C并发服务器
	printf("输入'I'选重复服务器!\n输入'C'选并发服务器!\n"); 	//I重复服务器，C并发服务器
	//r='I';
	r=getchar();
		
	if('I'==r)
	{
		printf("getchar:%c\n",r);
		//循环服务器
		if(-1==listen(sock_fd,100))//最大允许1个请求
		{
			printf("listen出错!");	
			return -1;
		}
		
	
		while(1)
		{
			int sin_size=sizeof(struct sockaddr_in);
			if(-1==(client_fd=accept(sock_fd,(struct sockaddr *)&remote_addr,&sin_size)))
			{
				printf("accept 出错！");
				continue;
			}
			printf("received a connection from %s \n",inet_ntoa(remote_addr.sin_addr));
		
	
			if(-1==send(client_fd,"Hello,you are connected !\n",BUF_LEN,0))//发送欢迎词
			printf("send 出错！");
			if(-1==(r=recv(client_fd,buf,BUF_LEN,0)))//接收RD
			printf("recv 出错！");
			buf[r]='\0';
			printf("RD?:%s\n",buf);
	
			printf("will send the data! \n");
				
			if(-1==(r=time(&now)))printf("time 出错！");
				//printf("now:%d r:%d",now,r);
			str_now=ctime(&now);
	
	
			if((buf[0]='R')&&(buf[1]=='D'))//客户端准备好了给他们信息
			{
				sprintf(buf,"Service time : %s",str_now);
				send(client_fd,buf,BUF_LEN,0);
			}
	
			printf("print 'Enter' to end current socket!\n"); 		
		//	for(;;)
		//	{
		//		r=getchar();
		//		if (r!='\0') break;
		//		printf("put into a char! \n");
		//	}

			sleep(100);
			printf("send END !\n");
			close(client_fd);
		}
	}//循环服务器end	


	else if('C'==r)
	{
		
		printf("getchar:%c\n",r);
		//并发服务器
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
				if(-1==send(client_fd,"Hello,you are connected !\n",BUF_LEN,0))//发送欢迎词
				printf("send 出错！");
				if(-1==(re=recv(client_fd,buf,BUF_LEN,0)))//接收RD
				printf("recv 出错！");
	
				printf("RD?:%s\n",buf);
	
				printf("will send the data! \n");
				
				if(-1==(r=time(&now)))printf("time 出错！");
				//printf("now:%d r:%d",now,r);
				str_now=ctime(&now);
	
	
				if((buf[0]='R')&&(buf[1]=='D'))//客户端准备好了给他们信息
				{
					sprintf(buf,"Service time : %s",str_now);
					send(client_fd,buf,BUF_LEN,0);
				}
				
				
				printf("send END !\n");
				close(client_fd);
				exit(0);
			}
			close(client_fd);
		}
	
	
	}

		
		return 0;
		
}
