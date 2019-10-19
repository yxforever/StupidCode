//�������ͻ�ķ���һ��ֻ�ܼ�С����Ĵ��ڣ�
#include	"my.h"
static void	recvfrom_alarm(int);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen) 
{
	int				n;
	const int		on = 1;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	//sigset_t		sigset_alrm; ����һ��
	socklen_t		len;
	struct sockaddr	*preply_addr;
 
	preply_addr = Malloc(servlen);//Ϊ��recvfrom���صķ�������ַ����ռ�
	
	//Sigemptyset(&sigset_alrm);����һ��
	//Sigaddset(&sigset_alrm, SIGALRM);����һ������һ���źż�����������SIGALRM��Ӧλ��
	Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	//����SO_BROADCAST�׽ӿ�ѡ��

	Signal(SIGALRM, recvfrom_alarm);//��װSIGALRM�ź�

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
		//���յ���׼����󣬷���һ���㲥���ݱ������յ����Ӧ��

		alarm(5);//5���Ӻ����SIGALRM�ź�
		//alarm(1);��Ϊ1�룬����sleep
		
		
		for ( ; ; ) {//����ѭ������Ӧ��
			len = servlen;
			//Sigprocmask(SIG_UNBLOCK, &sigset_alrm, NULL);����һ���������ź�
			n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
			//Sigprocmask(SIG_BLOCK, &sigset_alrm, NULL);����һ�������ź�
			
			//���źŲ���recvfrom�ڼ䵽����ʽ�С������ʱ��ʧЧ
			if (n < 0) {
				if (errno == EINTR)//�źŵִ��SIGALRM�źţ�����ѭ��
					break;		/* waited long enough for replies */
				else
					err_sys("recvfrom error");
			} else {
				recvline[n] = 0;	/* null terminate */
				//sleep(1);����sleep��alarm�ź���sleep�е�����׽ӿڽ��ջ����������ݶ���֮�󣬳�����һֱ������recvfrom��
				
				printf("from %s: %s",
						Sock_ntop_host(preply_addr, len), recvline);//��ʾӦ����Ϣ��
			}
		//printf("in for !\n");
		}
		//printf("wait for fgets !\n");
	}
	free(preply_addr);
}

static void
recvfrom_alarm(int signo)
{
	return;		/* just interrupt the recvfrom() */
}
