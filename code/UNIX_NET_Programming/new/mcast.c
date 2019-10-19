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
	//���������׽ӿڣ�һ�����ͣ�һ�����գ�
	//��ʹ��һ���׽ӿڣ�����ԴЭ���ַ�Ƕಥ��ַ����ֹ����ԴIP��ַ�Ƕಥ��㲥��ַ
	sendfd = Socket(AF_INET, SOCK_DGRAM, 0);	
	recvfd = Socket(AF_INET, SOCK_DGRAM, 0);
	
	Setsockopt(recvfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	sarecv=sasend;
	//�����׽ӿڰ���ָ���Ķಥ��ַ��ָ���Ķ˿ںţ����Է�ֹ�޶˿���ͬ���������ݱ����ﱾ�׽ӿ�
	Bind(recvfd, (SA *) &sarecv, sizeof(sarecv));

	//����ಥ��
	mreq.imr_multiaddr=sarecv.sin_addr;
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	setsockopt(recvfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq));
	
	//��ֹ����
	flag=0;
	setsockopt(sendfd, IPPROTO_IP, IP_MULTICAST_LOOP,&flag, sizeof(flag));


	if (Fork() == 0)
	recv_all(recvfd, sizeof(sarecv));		/* �ӽ��̽���*/
	send_all(sendfd, (SA *)&sasend, sizeof(sarecv));	/*�����̷��� */
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
		//��ӡ���Ͷ˵�ַ���˿ڣ������������̺�
		printf("from %s: %s", Sock_ntop(safrom, len), line);
	}
}



void	send_all(int sendfd, SA *sadest, socklen_t salen)
{
	char		line[MAXLINE];		/* hostname and process ID */
	struct utsname	myname;
	if (uname(&myname) < 0)
		err_sys("uname error");;
	//���͵�ǰ�����������̺�
	snprintf(line, sizeof(line), "%s, %d\n", myname.nodename, getpid());

	for ( ; ; ) {
		Sendto(sendfd, line, strlen(line), 0, sadest, salen);
		sleep(SENDRATE);
	}
}
