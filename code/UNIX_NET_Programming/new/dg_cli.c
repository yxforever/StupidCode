#include	"my.h"
void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{ 
	int	n,j;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];
	while (Fgets(sendline, MAXLINE, fp)!= NULL) {
		/*Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
			n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
			recvline[n] = 0;	//null terminate 
			Fputs(recvline, stdout);*/
			
		for(j=0;j<4;j++)//信号驱动IO中使用
			{
				Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
			}
			sleep(1);//cntread[0]可以出现非0,产生多次IO信号。for语句的IO信号可能在IO解阻塞前全到达则只会启动一次处理函数。
			Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
		for(j=0;j<5;j++)//信号驱动IO中使用
		{
			n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
			recvline[n] = 0;	/* null terminate */
			Fputs(recvline, stdout);
		}
	}
}
