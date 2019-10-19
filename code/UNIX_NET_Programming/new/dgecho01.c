/* include dgecho1 */
#include	"my.h"

static int		sockfd;

#define	QSIZE	   8		/* 输入队列长度 */
#define	MAXDG	4096		/* 最大数据报大小 */ 

typedef struct {
  void		*dg_data;		/* 指向数据报实际数据的指针 */
  size_t	dg_len;			/*数据长度*/
  struct sockaddr  *dg_sa;	/* 指向客户端的地址指针 */
  socklen_t	dg_salen;		/*地址长度*/
} DG;
static DG	dg[QSIZE];			/* 待处理的数据报队列，作为环形缓冲区 */
static long	cntread[QSIZE+1];	/* 诊断用的计数器 */

static int	iget;		/* 主循环将要处理的数据位置*/
static int	iput;		/* 信号处理函数将用来填写数据的位置 */
static int	nqueue;		/*可供给主循环处理数据总数 */
static socklen_t clilen;/* 地址最大长度 */

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

	sockfd = sockfd_arg;//套接口描述字全局化
	clilen = clilen_arg;

	for (i = 0; i < QSIZE; i++) {	/* 初始化队列缓冲 */
		dg[i].dg_data = Malloc(MAXDG);
		dg[i].dg_sa = Malloc(clilen);
		dg[i].dg_salen = clilen;
	}
	iget = iput = nqueue = 0;

	Signal(SIGHUP, sig_hup);//用于诊断目的信号安装
	Signal(SIGIO, sig_io);//用户信号驱动IO
	Signal(SIGUSR1, sig_usr);//测试用
		
	Fcntl(sockfd, F_SETOWN, getpid());//设置套接口属主
	Ioctl(sockfd, FIOASYNC, &on);//设置套接口为信号驱动IO
	Ioctl(sockfd, FIONBIO, &on);//设置套接口为非阻塞IO

	Sigemptyset(&zeromask);		/*初始化3个信号集 */
	Sigemptyset(&oldmask);
	Sigemptyset(&newmask);
	Sigaddset(&newmask, SIGIO);	/* 准备阻塞的信号 */

	Sigprocmask(SIG_BLOCK, &newmask, &oldmask);//增加newmask中的阻塞信号集，即阻塞SIGIO
	for ( ; ; ) {//循环中等待数据到达（信号处理函数完成），并回射
		while (nqueue == 0)//nqueue在SIGIO信号处理函数中使用，必需在该信号阻塞下才能判断
			sigsuspend(&zeromask);	
		/*使用zeromask屏蔽字,直到一个信号抵达并完成处理函数，恢复屏蔽字。即解阻塞SIGIO*/
		//若不是SIGIO信号，如SIGUSR1信号，则再循环while
		/* 4unblock SIGIO */
		Sigprocmask(SIG_SETMASK, &oldmask, NULL);
		//将oldmask中阻塞信号集设置为当前阻塞集合，解开SIGIO阻塞
		//iget不被信号处理函数使用，不需要阻塞SIGIO
	
		Sendto(sockfd, dg[iget].dg_data, dg[iget].dg_len, 0,
			   dg[iget].dg_sa, dg[iget].dg_salen);
		if (++iget >= QSIZE)
			iget = 0;//环形缓冲处理

			/* 4block SIGIO */
		Sigprocmask(SIG_BLOCK, &newmask, &oldmask);//阻塞SIGIO
		nqueue--;
	}
}
/* end dgecho2 */

/* include sig_io */
static void
sig_io(int signo)
{
	ssize_t		len;
	int			nread;//本次处理函数中接收数据报个数
	DG			*ptr;
	
	
	
	for (nread = 0; ; ) {
		if (nqueue >= QSIZE)//缓冲区队列已满，终止进程
			err_quit("receive overflow");

		ptr = &dg[iput];	//要填写的缓冲区
		ptr->dg_salen = clilen;
		len = recvfrom(sockfd, ptr->dg_data, MAXDG, 0,
					   ptr->dg_sa, &ptr->dg_salen);
		if (len < 0) {
			if (errno == EWOULDBLOCK)//采用的非阻塞套接口接收，直到套接口中数据全部读取。
				break;		/* all done; no more queued to read */
			else
				err_sys("recvfrom error");
		}
		
		ptr->dg_len = len;
		
		sleep(1);//cntread[0]可以出现非0
		//信号处理函数中被收到IO信号，阻塞了新的信号。实际该信号的数据在本次循环已经处理
		nread++;
		nqueue++;
		if (++iput >= QSIZE)
			iput = 0;

	}
	cntread[nread]++;		/*程序中信号处理函数读入nread数据的次数*/
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
