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

	Signal(SIGALRM, recvfrom_alarm);//��װ�źŴ�����

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		alarm(5);//alarm5��󴥷�ALRM�ź�
		for ( ; ; ) {
			if (sigsetjmp(jmpbuf, 1) != 0)
				break;
				//������ת������,���浱ǰ���ź������֣�����0��
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
	//�Զ���ALRM�ź�����
	siglongjmp(jmpbuf, 1);
	//�ָ��ź�������,��ת��sigsetjmp����������sigsetjmp����1
}
