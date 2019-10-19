#include	"my.h"
#include	<setjmp.h>

static void			recvfrom_alarm(int); 
static sigjmp_buf	jmpbuf;

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	const int		on = 1;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr	*preply_addr;
 
	preply_addr = Malloc(servlen);

	Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	Signal(SIGALRM, recvfrom_alarm);//安装信号处理函数

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		alarm(5);//alarm5秒后触发ALRM信号
		for ( ; ; ) {
			if (sigsetjmp(jmpbuf, 1) != 0)
				break;
				//建立跳转缓冲区,保存当前的信号屏蔽字，返回0；
			len = servlen;
			n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
			recvline[n] = 0;	/* null terminate */
			printf("from %s: %s",
					Sock_ntop_host(preply_addr, len), recvline);
		}
	}
	free(preply_addr);
}

static void
recvfrom_alarm(int signo)
{
	//自动将ALRM信号阻塞
	siglongjmp(jmpbuf, 1);
	//恢复信号屏蔽字,跳转到sigsetjmp函数，导致sigsetjmp返回1
}
