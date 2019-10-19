//含解决冲突的方法一（只能减小出错的窗口）
#include	"my.h"
static void	recvfrom_alarm(int);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen) 
{
	int				n;
	const int		on = 1;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	//sigset_t		sigset_alrm; 方法一：
	socklen_t		len;
	struct sockaddr	*preply_addr;
 
	preply_addr = Malloc(servlen);//为由recvfrom返回的服务器地址分配空间
	
	//Sigemptyset(&sigset_alrm);方法一：
	//Sigaddset(&sigset_alrm, SIGALRM);方法一：声明一个信号集，并仅仅打开SIGALRM对应位。
	Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	//设置SO_BROADCAST套接口选项

	Signal(SIGALRM, recvfrom_alarm);//安装SIGALRM信号

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
		//接收到标准输入后，发送一个广播数据报，会收到多个应答

		alarm(5);//5秒钟后产生SIGALRM信号
		//alarm(1);改为1秒，增加sleep
		
		
		for ( ; ; ) {//无限循环接收应答
			len = servlen;
			//Sigprocmask(SIG_UNBLOCK, &sigset_alrm, NULL);方法一：解阻塞信号
			n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
			//Sigprocmask(SIG_BLOCK, &sigset_alrm, NULL);方法一：阻塞信号
			
			//若信号不在recvfrom期间到达（概率较小），则定时器失效
			if (n < 0) {
				if (errno == EINTR)//信号抵达，如SIGALRM信号，跳出循环
					break;		/* waited long enough for replies */
				else
					err_sys("recvfrom error");
			} else {
				recvline[n] = 0;	/* null terminate */
				//sleep(1);增加sleep，alarm信号在sleep中到达，当套接口接收缓冲区中数据读完之后，程序则一直阻塞在recvfrom中
				
				printf("from %s: %s",
						Sock_ntop_host(preply_addr, len), recvline);//显示应答信息。
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
