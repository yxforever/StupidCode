#include	"my.h"
#include <unistd.h>
#include <fcntl.h>
 
int		listenfd, connfd;
void	sig_urg(int);
int
main(int argc, char **argv)
{
	int		n;
	char	buff[100];
	
	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: tcprecv01 [ <host> ] <port#>");

	connfd = Accept(listenfd, NULL, NULL);
	signal(SIGURG, sig_urg);//安装信号处理函数
	Fcntl(connfd, F_SETOWN, getpid());//设置已连接套接口属主
	for ( ; ; ) {
		if ( (n = read(connfd, buff, sizeof(buff)-1)) == 0) {
			printf("received EOF\n");
			exit(0);
		}
		else if(n<0)
		 {
		 	if (errno==EINTR)continue;  
			else
			{
				printf("read erro! %s\n",strerror(errno));
				exit(0);
			}
		}
		else {
			printf("n= %d \n",n);
			buff[n] = 0;	/* null terminate */
			printf("read %d bytes: %s\n", n, buff);
		}
	}
}

void
sig_urg(int signo)
{
	int		n;
	char	buff[100];
	printf("SIGURG received !\n");
	n = Recv(connfd, buff, sizeof(buff)-1, MSG_OOB);//读取带外数据
	buff[n] = 0;		/* null terminate */
	printf("read %d OOB byte: %s\n", n, buff);
}
