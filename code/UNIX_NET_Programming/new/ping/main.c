#include	"ping.h"

 

struct proto	proto_v4 = { proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_ICMP };
/*
#ifdef	IPV6
struct proto	proto_v6 = { proc_v6, send_v6, init_v6, NULL, NULL, 0, IPPROTO_ICMPV6 };
#endif
*/

int	datalen = 56;		/* 随同回射请求发送的可选ICMP数据量设置为56字节，导致84字节的IPv4数据报（20字节IP头，8字节ICMP头） */

int
main(int argc, char **argv)
{
	int				c;
	struct addrinfo	*ai;
	char *h;

	opterr = 0;		/* don't want getopt() writing to stderr */
	while ( (c = getopt(argc, argv, "v")) != -1) {
		switch (c) {
		case 'v'://查找到v参数
			verbose++;
			break;

		case '?'://没有匹配的字符"v"
			err_quit("unrecognized option: %c", c);
		}
	}

	if (optind != argc-1)
		err_quit("usage: ping [ -v ] <hostname>");
	host = argv[optind];

	pid = getpid() & 0xffff;	/* ICMP ID field is 16 bits */
	Signal(SIGALRM, sig_alrm);

	ai = Host_serv(host, NULL, 0, 0);//getaddrinfo()函数获取目标主机名等地址信息

	h = Sock_ntop_host(ai->ai_addr, ai->ai_addrlen);//inet_ntop地址转换
	printf("PING %s (%s): %d data bytes\n",
			ai->ai_canonname ? ai->ai_canonname : h,
			h, datalen);

		/* 4initialize according to protocol */
	if (ai->ai_family == AF_INET) {
		pr = &proto_v4;

/*
#ifdef	IPV6
	} else if (ai->ai_family == AF_INET6) {
		pr = &proto_v6;
		if (IN6_IS_ADDR_V4MAPPED(&(((struct sockaddr_in6 *)
								 ai->ai_addr)->sin6_addr)))
			err_quit("cannot ping IPv4-mapped IPv6 address");
#endif
*/
	} else
		err_quit("unknown address family %d", ai->ai_family);

	pr->sasend = ai->ai_addr;
	pr->sarecv = Calloc(1, ai->ai_addrlen);
	pr->salen = ai->ai_addrlen;

	readloop();
	exit(0);
}
