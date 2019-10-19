#include	"ping.h"

struct proto	proto_v4 = { proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_ICMP };
int	datalen = 56;		/* 随同回射请求发送的可选ICMP数据量设置为56字节，导致84字节的IPv4数据报（20字节IP头，8字节ICMP头）*/
int
main(int argc, char **argv) 
{
	int c;
	struct addrinfo	*ai;
	char *h;

	opterr = 0;		/* don't want getopt() writing to stderr */
	while ( (c = getopt(argc, argv, "v")) != -1) {
		switch (c) {
		case 'v'://查找到v参数
			verbose++;
			break;

		case '?'://没有匹配的字符"v"
			fprintf(stderr,"unrecognized option: %c", c);
			exit(1);
		}
	}

	if (optind != argc-1)
		{
			fprintf(stderr,"usage: ping [ -v ] <hostname>");
			exit(1);
		}
		
	host = argv[optind];

	pid = getpid() & 0xffff;	/* ICMP ID field is 16 bits */
	signal(SIGALRM, sig_alrm);



/*
	struct hostent *strHost ;
	int i;
	// 取得主机信息 
	if ((strHost=gethostbyname(host))==NULL)
	{
	// 如果gethostbyname失败,则给出错误信息 
		printf("gethostbyname error:%s\n",hstrerror(h_errno));

		// 然后退出 
		exit (1);
	}
	// 列出程序取得的信息 
	
	printf("h_name:%s\n",strHost->h_name);
	printf("h_length:%d\n",strHost->h_length);
	printf("h_addrtype:%d",strHost->h_addrtype);
	for(i=0;strHost->h_aliases[i] !=NULL;i++)
		printf("h_aliases%d:%s\n",i+1,strHost->h_aliases[i]); 
	for(i=0;strHost->h_addr_list[i]!=NULL;i++){
		struct in_addr *addr;
		addr=(struct in_addr *)strHost->h_addr_list[i];
		printf("ip%d:%s\n",(i+1),inet_ntoa(*addr));
	}
	
*/
	
	


	ai = host_serv(host, NULL, 0, 0);//获取地址信息
	if(!ai)
		{
		printf("host unknown! \n");
		exit (1);
		}
	h = sock_ntop_host(ai->ai_addr, ai->ai_addrlen);//地址转换成点分十进制
	printf("PING %s (%s): %d data bytes\n",
			ai->ai_canonname ? ai->ai_canonname : h,
			h, datalen);

		/* 4initialize according to protocol */
	if (ai->ai_family == AF_INET) {
		pr = &proto_v4;
	} else
		{
			fprintf(stderr,"unknown address family %d", ai->ai_family);
			exit(1);
		}

	pr->sasend = ai->ai_addr;
	pr->sarecv = calloc(1, ai->ai_addrlen);
	pr->salen = ai->ai_addrlen;

	readloop();

	exit(0);
}
