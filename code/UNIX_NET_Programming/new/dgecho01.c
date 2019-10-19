/* include dgecho1 */
#include	"my.h"

static int		sockfd;

#define	QSIZE	   8		/* ������г��� */
#define	MAXDG	4096		/* ������ݱ���С */ 

typedef struct {
  void		*dg_data;		/* ָ�����ݱ�ʵ�����ݵ�ָ�� */
  size_t	dg_len;			/*���ݳ���*/
  struct sockaddr  *dg_sa;	/* ָ��ͻ��˵ĵ�ַָ�� */
  socklen_t	dg_salen;		/*��ַ����*/
} DG;
static DG	dg[QSIZE];			/* ����������ݱ����У���Ϊ���λ����� */
static long	cntread[QSIZE+1];	/* ����õļ����� */

static int	iget;		/* ��ѭ����Ҫ���������λ��*/
static int	iput;		/* �źŴ�������������д���ݵ�λ�� */
static int	nqueue;		/*�ɹ�����ѭ�������������� */
static socklen_t clilen;/* ��ַ��󳤶� */

static void	sig_io(int);
static void	sig_hup(int);
static void	sig_usr(int);
/* end dgecho1 */

/* include dgecho2 */
void
dg_echo(int sockfd_arg, SA *pcliaddr, socklen_t clilen_arg)
{
	int			i;
	const int	on = 1;
	sigset_t	zeromask, newmask, oldmask;

	sockfd = sockfd_arg;//�׽ӿ�������ȫ�ֻ�
	clilen = clilen_arg;

	for (i = 0; i < QSIZE; i++) {	/* ��ʼ�����л��� */
		dg[i].dg_data = Malloc(MAXDG);
		dg[i].dg_sa = Malloc(clilen);
		dg[i].dg_salen = clilen;
	}
	iget = iput = nqueue = 0;

	Signal(SIGHUP, sig_hup);//�������Ŀ���źŰ�װ
	Signal(SIGIO, sig_io);//�û��ź�����IO
	Signal(SIGUSR1, sig_usr);//������
		
	Fcntl(sockfd, F_SETOWN, getpid());//�����׽ӿ�����
	Ioctl(sockfd, FIOASYNC, &on);//�����׽ӿ�Ϊ�ź�����IO
	Ioctl(sockfd, FIONBIO, &on);//�����׽ӿ�Ϊ������IO

	Sigemptyset(&zeromask);		/*��ʼ��3���źż� */
	Sigemptyset(&oldmask);
	Sigemptyset(&newmask);
	Sigaddset(&newmask, SIGIO);	/* ׼���������ź� */

	Sigprocmask(SIG_BLOCK, &newmask, &oldmask);//����newmask�е������źż���������SIGIO
	for ( ; ; ) {//ѭ���еȴ����ݵ���źŴ�������ɣ���������
		while (nqueue == 0)//nqueue��SIGIO�źŴ�������ʹ�ã������ڸ��ź������²����ж�
			sigsuspend(&zeromask);	
		/*ʹ��zeromask������,ֱ��һ���źŵִﲢ��ɴ��������ָ������֡���������SIGIO*/
		//������SIGIO�źţ���SIGUSR1�źţ�����ѭ��while
		/* 4unblock SIGIO */
		Sigprocmask(SIG_SETMASK, &oldmask, NULL);
		//��oldmask�������źż�����Ϊ��ǰ�������ϣ��⿪SIGIO����
		//iget�����źŴ�����ʹ�ã�����Ҫ����SIGIO
	
		Sendto(sockfd, dg[iget].dg_data, dg[iget].dg_len, 0,
			   dg[iget].dg_sa, dg[iget].dg_salen);
		if (++iget >= QSIZE)
			iget = 0;//���λ��崦��

			/* 4block SIGIO */
		Sigprocmask(SIG_BLOCK, &newmask, &oldmask);//����SIGIO
		nqueue--;
	}
}
/* end dgecho2 */

/* include sig_io */
static void
sig_io(int signo)
{
	ssize_t		len;
	int			nread;//���δ������н������ݱ�����
	DG			*ptr;
	
	
	
	for (nread = 0; ; ) {
		if (nqueue >= QSIZE)//������������������ֹ����
			err_quit("receive overflow");

		ptr = &dg[iput];	//Ҫ��д�Ļ�����
		ptr->dg_salen = clilen;
		len = recvfrom(sockfd, ptr->dg_data, MAXDG, 0,
					   ptr->dg_sa, &ptr->dg_salen);
		if (len < 0) {
			if (errno == EWOULDBLOCK)//���õķ������׽ӿڽ��գ�ֱ���׽ӿ�������ȫ����ȡ��
				break;		/* all done; no more queued to read */
			else
				err_sys("recvfrom error");
		}
		
		ptr->dg_len = len;
		
		sleep(1);//cntread[0]���Գ��ַ�0
		//�źŴ������б��յ�IO�źţ��������µ��źš�ʵ�ʸ��źŵ������ڱ���ѭ���Ѿ�����
		nread++;
		nqueue++;
		if (++iput >= QSIZE)
			iput = 0;

	}
	cntread[nread]++;		/*�������źŴ���������nread���ݵĴ���*/
}
/* end sig_io */

/* include sig_hup */
static void
sig_hup(int signo)
{
	int		i;

	for (i = 0; i <= QSIZE; i++)
		printf("cntread[%d] = %ld\n", i, cntread[i]);
}
/* end sig_hup */
static void sig_usr(int signo)
{
	printf("SIG USR1\n");
}
