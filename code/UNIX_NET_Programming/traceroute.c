#include	"traceroute.h"
#include <netinet/udp.h>

struct proto	proto_v4 = { icmpcode_v4, recv_v4, NULL, NULL, NULL, NULL, 0, 
							 IPPROTO_ICMP, IPPROTO_IP, IP_TTL };
/*
#ifdef	IPV6
struct proto	proto_v6 = { icmpcode_v6, recv_v6, NULL, NULL, NULL, NULL, 0,
							 IPPROTO_ICMPV6, IPPROTO_IPV6, IPV6_UNICAST_HOPS };
#endif
*/

int		datalen = sizeof(struct rec);	/* defaults */
int		max_ttl = 30;//最大TTL，用户可通过-m命令行选项修改该值
int		nprobes = 3;//对于每个TTL值，发送3个控测分组
u_short	dport = 32768 + 666;//目的端口初始值，每发送一个UDP数据报其值递增一次

int
main(int argc, char **argv)
{
	int				c;
	struct addrinfo	*ai;
	char *h;


//#ifdef __FAVOR_BSD
//printf("first \n");
//#else
//printf("second \n");
//#endif
//	struct udphdr * pUDP;
//	pUDP=calloc(1,sizeof(struct udphdr));
//	pUDP->source=htons(50);
	


	opterr = 0;		/* don't want getopt() writing to stderr */
	while ( (c = getopt(argc, argv, "m:v")) != -1) {
		switch (c) {
		case 'm':
			if ( (max_ttl = atoi(optarg)) <= 1)
			{
				fprintf(stderr,"invalid -m value");	
				exit(1);
			}	
				
			break;

		case 'v':
			verbose++;
			break;

		case '?':
			{
				fprintf(stderr,"unrecognized option: %c", c);	
				exit(1);
			}
			
		}
	}

	if (optind != argc-1)
	{
			fprintf(stderr,"usage: traceroute [ -m <maxttl> -v ] <hostname>");	
			exit(1);
	}
		
	host = argv[optind];
	//char strhost[12]="IPaddr";
	//host =strhost;
	
	//printf("host = %s\n",host);
	
	pid = getpid();
	signal(SIGALRM, sig_alrm);

	ai = host_serv(host, NULL, 0, 0);
	if(!ai)
	{
		printf("host unknown! \n");
		exit (1);
	}
	h = sock_ntop_host(ai->ai_addr, ai->ai_addrlen);
	printf("traceroute to %s (%s): %d hops max, %d data bytes\n",
		   ai->ai_canonname ? ai->ai_canonname : h,
		   h, max_ttl, datalen);

		/* initialize according to protocol */
	if (ai->ai_family == AF_INET) {
		pr = &proto_v4;

/*
#ifdef	IPV6
	} else if (ai->ai_family == AF_INET6) {
		pr = &proto_v6;
		if (IN6_IS_ADDR_V4MAPPED(&(((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr)))
			err_quit("cannot traceroute IPv4-mapped IPv6 address");
#endif
*/
	} else
		
	{
		fprintf(stderr,"unknown address family %d", ai->ai_family);	
		exit(1);
	}
			

	pr->sasend = ai->ai_addr;		/* contains destination address */
	pr->sarecv = calloc(1, ai->ai_addrlen);
	pr->salast = calloc(1, ai->ai_addrlen);
	pr->sabind = calloc(1, ai->ai_addrlen);
	pr->salen = ai->ai_addrlen;

	traceloop();

	exit(0);
}

