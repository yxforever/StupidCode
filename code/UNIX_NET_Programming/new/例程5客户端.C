#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h> 
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define SOCK_PATH "exmp_sock"

/* 一次所能够接收的最大字节数 */
#define MAXDATASIZE 512
int main()
{
  int sockfd， n;
  char buf[MAXDATASIZE];
  struct sockaddr_un their_addr;
if ((sockfd = socket(AF_UNIX，SOCK_STREAM，0)) == -1) 
	/* 如果socket()调用出现错误，则显示错误信息并退出*/
	{
			perror("socket error");
			exit(1);
	}
		their_addr.sin_family = AF_INET;
		/* 网络字节顺序，短整型*/
		their_addr.sun_family = AF_UNIX;
		strcpy(their_addr.sun_path，SOCK_PATH);
		len = strlen(their_addr.sun_path) + sizeof(their_addr.sun_family);
		if(connect(sockfd， (struct sockaddr *)&their_addr，len) == -1)
		{
			perror("connect error");
			exit(1);
		}
		while(printf(">",fgets(buf,MAXDATASIZE,stdin),!feof(stdin)))
		{
			if(send(sockfd,buf,strlen(buf),0)==-1)
				{
					perror("send error");
					exit(1);
					}
			if (n = recv(sockfd，buf，MAXDATASIZE，0) > 0)
  		{ 
   			buf[n] ='\0';
				printf(">%s"，buf);
			}  else  {
				if (n < 0 )  perror("recv error");
				else
						printf("server closed connected\n");
				exit(1);
			}		
}  /* while */
	close(sockfd);
	return 0;
}