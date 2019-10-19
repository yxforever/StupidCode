//使用pselect解决冲突问题
#include	"my.h"

static void	recvfrom_alarm(int);
 
void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	const int		on = 1;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	fd_set			rset;
	sigset_t		sigset_alrm, sigset_empty;
	socklen_t		len;
	struct sockaddr	*preply_addr;
 
	preply_addr = Malloc(servlen);

	Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	FD_ZERO(&rset);//清空描述字集

	Sigemptyset(&sigset_empty);//定义空信号掩码
	Sigemptyset(&sigset_alrm);//定义要阻塞SIGALRM信号的掩码
	Sigaddset(&sigset_alrm, SIGALRM);

	Signal(SIGALRM, recvfrom_alarm);//安装信号

	while (Fgets(sendline, MAXLINE, fp) != NULL) {
		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		Sigprocmask(SIG_BLOCK, &sigset_alrm, NULL);//阻塞ALRM信号
		alarm(5);//5秒后触发ALRM信号
		for ( ; ; ) {
			FD_SET(sockfd, &rset);
			n = pselect(sockfd+1, &rset, NULL, NULL, NULL, &sigset_empty);
			//仅仅在pselect阻塞期间使用sigset_empty信号掩码，即阻塞期间解阻塞ALRM信号
			if (n < 0) {
				if (errno == EINTR)
					break;
				else
					err_sys("pselect error");
			} else if (n != 1)
				err_sys("pselect error: returned %d", n);

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
	return;		/* just interrupt the recvfrom() */
}
