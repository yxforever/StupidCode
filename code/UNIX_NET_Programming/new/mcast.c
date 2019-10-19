//gcc mcast.c error.c wrapstdio.c wrapsock.c -o mcast

#include <sys/utsname.h>
#include	"my.h"
#define	SENDRATE	5		/* send one datagram every five seconds */ 

void	recv_all(int, socklen_t);
void	send_all(int, SA *, socklen_t);
void * Malloc(size_t );
int
main(int argc, char **argv)
{
	int					sendfd, recvfd;
	const int			on = 1;
	socklen_t			salen;
	struct sockaddr_in		sasend, sarecv;
	struct ip_mreq		mreq;
	int flag=0;
	if (argc != 3)
		err_quit("usage: sendrecv <IP-multicast-address> <port#> ");
		
	bzero(&sasend, sizeof(sasend));
	sasend.sin_family = AF_INET;
	sasend.sin_port = htons(atoi(argv[2]));
	Inet_pton(AF_INET, argv[1], &sasend.sin_addr);
	//创建两个套接口，一个发送，一个接收；
	//若使用一个套接口，则导致源协议地址是多播地址，禁止出现源IP地址是多播或广播地址
	sendfd = Socket(AF_INET, SOCK_DGRAM, 0);	
	recvfd = Socket(AF_INET, SOCK_DGRAM, 0);
	
	Setsockopt(recvfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	sarecv=sasend;
	//接收套接口绑定于指定的多播地址和指定的端口号，可以防止宿端口相同的其他数据报到达本套接口
	Bind(recvfd, (SA *) &sarecv, sizeof(sarecv));

	//加入多播组
	mreq.imr_multiaddr=sarecv.sin_addr;
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	setsockopt(recvfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq));
	
	//禁止回馈
	flag=0;
	setsockopt(sendfd, IPPROTO_IP, IP_MULTICAST_LOOP,&flag, sizeof(flag));


	if (Fork() == 0)
	recv_all(recvfd, sizeof(sarecv));		/* 子进程接收*/
	send_all(sendfd, (SA *)&sasend, sizeof(sarecv));	/*父进程发送 */
}


void recv_all(int recvfd, socklen_t salen)
{
	int					n;
	char				line[MAXLINE+1];
	socklen_t			len;
	struct sockaddr		*safrom;
	safrom = Malloc(salen);

	for ( ; ; ) {
		len = salen;
		n = Recvfrom(recvfd, line, MAXLINE, 0, safrom, &len);
		line[n] = 0;	/* null terminate */
		//打印发送端地址：端口：主机名，进程号
		printf("from %s: %s", Sock_ntop(safrom, len), line);
	}
}



void	send_all(int sendfd, SA *sadest, socklen_t salen)
{
	char		line[MAXLINE];		/* hostname and process ID */
	struct utsname	myname;
	if (uname(&myname) < 0)
		err_sys("uname error");;
	//发送当前主机名，进程号
	snprintf(line, sizeof(line), "%s, %d\n", myname.nodename, getpid());

	for ( ; ; ) {
		Sendto(sendfd, line, strlen(line), 0, sadest, salen);
		sleep(SENDRATE);
	}
}
