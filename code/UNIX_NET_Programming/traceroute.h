#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
//#include <sys/cdefs.h> 
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <netinet/udp.h>

#define	BUFSIZE		1500
extern int	optind, opterr, optopt;
extern char *optarg;




struct rec {					/* format of outgoing UDP data */
  u_short	rec_seq;			/* sequence number */
  u_short	rec_ttl;			/* TTL packet left with */
  struct timeval	rec_tv;		/* time packet left */
};

			/* globals */
char	 recvbuf[BUFSIZE];
char	 sendbuf[BUFSIZE];

int		 datalen;			/* # bytes of data following ICMP header */
char	*host;
u_short	 sport, dport;

int		 nsent;				/* add 1 for each sendto() */
pid_t	 pid;				/* our PID */
int		 probe, nprobes;	//测探分组数量
int		 sendfd, recvfd;	/* send on UDP sock, read on raw ICMP sock */
int		 ttl, max_ttl;
int		 verbose;

			/* function prototypes */
const char	*icmpcode_v4(int);
const char	*icmpcode_v6(int);
int		 recv_v4(int, struct timeval *);
int		 recv_v6(int, struct timeval *);
void	 sig_alrm(int);
void	 traceloop(void);
void	 tv_sub(struct timeval *, struct timeval *);
void sock_set_port(struct sockaddr *, socklen_t, int);
char * sock_ntop_host(const struct sockaddr *, socklen_t );

struct proto {
  const char	*(*icmpcode)(int);
  int	 (*recv)(int, struct timeval *);
  struct sockaddr  *sasend;	/* sockaddr{} for send, from getaddrinfo */
  struct sockaddr  *sarecv;	/* sockaddr{} for receiving */
  struct sockaddr  *salast;	/* last sockaddr{} for receiving */
  struct sockaddr  *sabind;	/* sockaddr{} for binding source port */
  socklen_t   		salen;	/* length of sockaddr{}s */
  int			icmpproto;	/* IPPROTO_xxx value for ICMP */
  int	   ttllevel;		/* setsockopt() level to set TTL */
  int	   ttloptname;		/* setsockopt() name to set TTL */
} *pr;

#ifdef	IPV6

#include	<netinet/ip6.h>
#include	<netinet/icmp6.h>

#endif


void traceloop(void)
{
	int	seq, code, done;
	double	rtt;
	struct rec *rec;
	struct timeval	tvrecv;

	recvfd = socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
	setuid(getuid());		/* don't need special permissions anymore */
/*
#ifdef	IPV6
	if (pr->sasend->sa_family == AF_INET6 && verbose == 0) {
		struct icmp6_filter myfilt;
		ICMP6_FILTER_SETBLOCKALL(&myfilt);
		ICMP6_FILTER_SETPASS(ICMP6_TIME_EXCEEDED, &myfilt);
		ICMP6_FILTER_SETPASS(ICMP6_DST_UNREACH, &myfilt);
		setsockopt(recvfd, IPPROTO_IPV6, ICMP6_FILTER,
					&myfilt, sizeof(myfilt));
	}
#endif
*/
	sendfd = socket(pr->sasend->sa_family, SOCK_DGRAM, 0);

	pr->sabind->sa_family = pr->sasend->sa_family;
	sport = (getpid() & 0xffff) | 0x8000;	/* our source UDP port # */
	sock_set_port(pr->sabind, pr->salen, htons(sport));
	bind(sendfd, pr->sabind, pr->salen);

	//sig_alrm(SIGALRM);

	seq = 0;
	done = 0;
	for (ttl = 1; ttl <= max_ttl && done == 0; ttl++) {
		setsockopt(sendfd, pr->ttllevel, pr->ttloptname, &ttl, sizeof(int));
		bzero(pr->salast, pr->salen);

		printf("%2d ", ttl);
		fflush(stdout);

		for (probe = 0; probe < nprobes; probe++) {
			rec = (struct rec *) sendbuf;
			rec->rec_seq = ++seq;
			rec->rec_ttl = ttl;
			gettimeofday(&rec->rec_tv, NULL);

			sock_set_port(pr->sasend, pr->salen, htons(dport + seq));
			sendto(sendfd, sendbuf, datalen, 0, pr->sasend, pr->salen);

			if ( (code = (*pr->recv)(seq, &tvrecv)) == -3)
				printf(" *");		/* timeout, no reply */
			else {
				char	str[NI_MAXHOST];

				if (sock_cmp_addr(pr->sarecv, pr->salast, pr->salen) != 0) {
					if (getnameinfo(pr->sarecv, pr->salen, str, sizeof(str),
									NULL, 0, 0) == 0)
						printf(" %s (%s)", str,
								sock_ntop_host(pr->sarecv, pr->salen));
					else
						printf(" %s",
								sock_ntop_host(pr->sarecv, pr->salen));
					memcpy(pr->salast, pr->sarecv, pr->salen);
				}
				tv_sub(&tvrecv, &rec->rec_tv);
				rtt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
				printf("  %.3f ms", rtt);

				if (code == -1)		/* port unreachable; at destination */
					done++;
				else if (code >= 0)
					printf(" (ICMP %s)", (*pr->icmpcode)(code));
			}
			fflush(stdout);
		}
		printf("\n");
	}
}


void sock_set_port(struct sockaddr *sa, socklen_t salen, int port)
{
	switch (sa->sa_family) {
	case AF_INET: {
		struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

		sin->sin_port = port;
		return;
	}

#ifdef	IPV6
	case AF_INET6: {
		struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

		sin6->sin6_port = port;
		return;
	}
#endif
	}

    return;
}

int gotalarm;

void sig_alrm(int signo)
{
	gotalarm = 1;	/* set flag to note that alarm occurred */
	return;			/* and interrupt the recvfrom() */
}




/*
 * Return: -3 on timeout
 *		   -2 on ICMP time exceeded in transit (caller keeps going)
 *		   -1 on ICMP port unreachable (caller is done)
 *		 >= 0 return value is some other ICMP unreachable code
 */

int recv_v4(int seq, struct timeval *tv)
{
	int				hlen1, hlen2, icmplen, ret;
	socklen_t		len;
	ssize_t			n;
	struct ip		*ip, *hip;
	struct icmp	*icmp;
	struct udphdr	*udp;

	gotalarm = 0;
	alarm(3);
	for ( ; ; ) {
		if (gotalarm)
			return(-3);		/* alarm expired */
		len = pr->salen;
		n = recvfrom(recvfd, recvbuf, sizeof(recvbuf), 0, pr->sarecv, &len);
		if (n < 0) {
			if (errno == EINTR)
				continue;
			else
				
				{
					fprintf(stderr,"recvfrom error");	
					exit(1);
				}
		}

		ip = (struct ip *) recvbuf;	/* start of IP header */
		hlen1 = ip->ip_hl << 2;		/* length of IP header */
	
		icmp = (struct icmp *) (recvbuf + hlen1); /* start of ICMP header */
		if ( (icmplen = n - hlen1) < 8)
			continue;				/* not enough to look at ICMP header */
	
		if (icmp->icmp_type == ICMP_TIMXCEED &&
			icmp->icmp_code == ICMP_TIMXCEED_INTRANS) {
			if (icmplen < 8 + sizeof(struct ip))
				continue;			/* not enough data to look at inner IP */

			hip = (struct ip *) (recvbuf + hlen1 + 8);
			hlen2 = hip->ip_hl << 2;
			if (icmplen < 8 + hlen2 + 4)
				continue;			/* not enough data to look at UDP ports */

			udp = (struct udphdr *) (recvbuf + hlen1 + 8 + hlen2);
			
 			if ((hip->ip_p == IPPROTO_UDP) && (udp->source == htons(sport)) &&(udp->dest == htons(dport + seq)))
 			 {
				ret = -2;		/* we hit an intermediate router */
				break;
			}

		} else if (icmp->icmp_type == ICMP_UNREACH) {
			if (icmplen < 8 + sizeof(struct ip))
				continue;			/* not enough data to look at inner IP */

			hip = (struct ip *) (recvbuf + hlen1 + 8);
			hlen2 = hip->ip_hl << 2;
			if (icmplen < 8 + hlen2 + 4)
				continue;			/* not enough data to look at UDP ports */

			udp = (struct udphdr *) (recvbuf + hlen1 + 8 + hlen2);
 			
 			if (hip->ip_p == IPPROTO_UDP &&	udp->source == htons(sport) && udp->dest == htons(dport + seq)) 
 			{
				if (icmp->icmp_code == ICMP_UNREACH_PORT)
					ret = -1;	/* have reached destination */
				else
					ret = icmp->icmp_code;	/* 0, 1, 2, ... */
				break;
			}
		}
		if (verbose) {
			printf(" (from %s: type = %d, code = %d)\n",
					sock_ntop_host(pr->sarecv, pr->salen),
					icmp->icmp_type, icmp->icmp_code);
		}
		/* Some other ICMP error, recvfrom() again */
	}
	alarm(0);					/* don't leave alarm running */
	gettimeofday(tv, NULL);		/* get time of packet arrival */
	return(ret);
}

#ifdef	HAVE_SOCKADDR_DL_STRUCT
#include	<net/if_dl.h>
#endif

int sock_cmp_addr(const struct sockaddr *sa1, const struct sockaddr *sa2,
			 socklen_t salen)
{
	if (sa1->sa_family != sa2->sa_family)
		return(-1);

	switch (sa1->sa_family) {
	case AF_INET: {
		return(memcmp( &((struct sockaddr_in *) sa1)->sin_addr,
					   &((struct sockaddr_in *) sa2)->sin_addr,
					   sizeof(struct in_addr)));
	}
/*
#ifdef	IPV6
	case AF_INET6: {
		return(memcmp( &((struct sockaddr_in6 *) sa1)->sin6_addr,
					   &((struct sockaddr_in6 *) sa2)->sin6_addr,
					   sizeof(struct in6_addr)));
	}
#endif

#ifdef	AF_UNIX
	case AF_UNIX: {
		return(strcmp( ((struct sockaddr_un *) sa1)->sun_path,
					   ((struct sockaddr_un *) sa2)->sun_path));
	}
#endif

#ifdef	HAVE_SOCKADDR_DL_STRUCT

	case AF_LINK: {
		return(-1);		// no idea what to compare here ? 
	}
#endif
*/
	}
    return (-1);
}

void tv_sub(struct timeval *out, struct timeval *in)
{
	if ( (out->tv_usec -= in->tv_usec) < 0) {	/* out -= in */
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

struct addrinfo *
host_serv(const char *host, const char *serv, int family, int socktype)
{
	int	n;
	struct addrinfo	hints, *res;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;	/* always return canonical name */
	hints.ai_family = family;	/* AF_UNSPEC, AF_INET, AF_INET6, etc. */
	hints.ai_socktype = socktype;	/* 0, SOCK_STREAM, SOCK_DGRAM, etc. */

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
	return(NULL);

	return(res);	/* return pointer to first on linked list */
}

char * sock_ntop_host(const struct sockaddr *sa, socklen_t salen)
{
    static char str[128];	/* Unix domain is largest */

	switch (sa->sa_family) {
	case AF_INET: {
	struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

	if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
		return(NULL);
		
	return(str);
	}

	default:
	snprintf(str, sizeof(str), "sock_ntop_host: unknown AF_xxx: %d, len %d",
	 sa->sa_family, salen);
	return(str);
	}
    return (NULL);
}

const char *
icmpcode_v4(int code)
{
	static char errbuf[100];
	switch (code) {
	case  0:	return("network unreachable");
	case  1:	return("host unreachable");
	case  2:	return("protocol unreachable");
	case  3:	return("port unreachable");
	case  4:	return("fragmentation required but DF bit set");
	case  5:	return("source route failed");
	case  6:	return("destination network unknown");
	case  7:	return("destination host unknown");
	case  8:	return("source host isolated (obsolete)");
	case  9:	return("destination network administratively prohibited");
	case 10:	return("destination host administratively prohibited");
	case 11:	return("network unreachable for TOS");
	case 12:	return("host unreachable for TOS");
	case 13:	return("communication administratively prohibited by filtering");
	case 14:	return("host recedence violation");
	case 15:	return("precedence cutoff in effect");
	default:	sprintf(errbuf, "[unknown code %d]", code);
				return errbuf;
	}
}
