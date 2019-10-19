//P14并发服务器与重复服务器的获取服务器时间，服务端
//可以使用:调用setsockopt，设置SO_REUSEADDR参数就可以了。然后无论进程如何退出，端口都不会被占用

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
 
#define BUF_LEN 102400		//设备读取缓存的大小
#define SERVPORT 3333	//服务器监听段口号
#define BACKLOG 10		//最大同时连接请求数
ssize_t						/* Read "n" bytes from a descriptor. */
readn(int fd, void *vptr, size_t n)
{
	size_t	nleft;
	ssize_t	nread;
	char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				nread = 0;		/* and call read() again */
			else
				return(-1);
		} else if (nread == 0)
			break;				/* EOF */

		nleft -= nread;
		ptr   += nread;
	}
	return(n - nleft);		/* return >= 0 */
}
/* end readn */
ssize_t
Readn(int fd, void *ptr, size_t nbytes)
{
	ssize_t		n;

	if ( (n = readn(fd, ptr, nbytes)) < 0)
		printf("readn error");
	return(n);
}

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
				int len=102400;
				r=Readn(client_fd,buf,len);
				if(r>=0)printf("read buf : %s r= %d len=%d \n",buf,r,len);
					buf[0]='\0';
				
				printf("server !\n");
				close(client_fd);
				exit(0);
			}
			close(client_fd);
		}
		return 0;
}

