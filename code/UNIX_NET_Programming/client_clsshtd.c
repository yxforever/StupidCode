//高级编程接口，close与shutdown区别 
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
#include <sys/uio.h>


#define BUF_LEN 102400		//设备读取缓存的大小
#define SERVPORT 3333	//服务器监听段口号
#define BACKLOG 10		//最大同时连接请求数

ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
		}
		printf ("nwrittern: %d nleft :%d \n",nwritten,nleft);
		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}
/* end writen */

void
Writen(int fd, void *ptr, size_t nbytes)
{
	if (writen(fd, ptr, nbytes) != nbytes)
		printf("writen error");
}


int main(int argc,char*argv[])
{
	//读取设备初始化
	int fd=0;
	ssize_t r=0;
	size_t len;
	char buf[BUF_LEN],buft[BUF_LEN],strbuf[1024];
	char IP[16];
	//char stderr[64];
	
	//网络服务器初始化
	int client_fd;//socket;client_fd:数据传输socket
	struct sockaddr_in my_addr;//本地地址 
	struct sockaddr_in remote_addr;//服务器地址信息 
	struct hostent *host;
	
	sprintf(IP,"192.168.1.4");
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
	
	//本地地址也可以绑定
	
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(44444);
	my_addr.sin_addr.s_addr=INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);
	if(-1==bind(client_fd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr)))
	{
		printf("bind 出错！");	
		return -1;
	}
	
	
	if(connect(client_fd,(struct sockaddr *)(&remote_addr),sizeof(struct sockaddr))==-1) 
	{ 
	  printf("Connect Error\a\n"); 
	  exit(1); 
	} 	
	
	sprintf(strbuf,"Hello,close shutdown example 1! \n");
	r=send(client_fd,strbuf,strlen(strbuf),0);
	printf("send buf1: %s r=%d \n",strbuf,r);
	sprintf(strbuf,"Hello,close shutdown example 2! \n");
	r=send(client_fd,strbuf,strlen(strbuf),0);
	printf("send buf1: %s r=%d \n",strbuf,r);
/*	
	sprintf(strbuf,"Hello,close shutdown example 1! \n");
	r=send(client_fd,strbuf,strlen(strbuf),0);
	printf("send buf1: %s r=%d \n",strbuf,r);
	
	r=recv(client_fd,strbuf,len,0);
	printf("recv ACK1: %s r=:%d  !\n",strbuf,r);
	
	
	
	if(!fork())
	{//子进程
		sleep(2);
		strbuf[0]='\0';
		sprintf(strbuf,"This is son,close shutdown example! \n");
		r=send(client_fd,strbuf,strlen(strbuf),0);
		printf("son send buf1: %s r=%d getpid:%d\n",strbuf,r,getpid());
		
		r=recv(client_fd,strbuf,len,0);
		strbuf[r]='\0';
		printf("son recv ACK1: %s r=:%d  !\n",strbuf,r);
		close(client_fd); 
		exit(0);
	}
	else {

		printf("father getpid :%d \n",getpid());
*/		close(client_fd); 
		exit(0);
//	}

}
