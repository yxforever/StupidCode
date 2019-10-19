#include <string.h>
#include <stdlib.h>
#include <errno.h> 
#include <sys/types.h>
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

#ifdef	HAVE_SOCKADDR_DL_STRUCT
#include	<net/if_dl.h>
#endif	
#define	BUFSIZE	1500
extern int	optind, opterr, optopt;
	/* globals */
char	 sendbuf[BUFSIZE];

int	 datalen;	/* # bytes of data following ICMP header */
char	*host;
int	 nsent;	/* add 1 for each sendto() */
pid_t	 pid;	/* our PID */
int	 sockfd;
int	 verbose;//v选项

	/* function prototypes */
void	 proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
void	 send_v4(void);
void	 readloop(void);
void	 sig_alrm(int);
void	 tv_sub(struct timeval *, struct timeval *);
uint16_t	in_cksum(uint16_t *addr, int len);

struct proto {
  void	 (*fproc)(char *, ssize_t, struct msghdr *, struct timeval *);
  void	 (*fsend)(void);
  void	 (*finit)(void);
  struct sockaddr  *sasend;	/* sockaddr{} for send, from getaddrinfo */
  struct sockaddr  *sarecv;	/* sockaddr{} for receiving */
  socklen_t	    salen;	/* length of sockaddr{}s */
  int	   	    icmpproto;	/* IPPROTO_xxx value for ICMP */
} *pr;




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
/* end host_serv */

/*
 * There is no easy way to pass back the integer return code from
 * getaddrinfo() in the function above, short of adding another argument
 * that is a pointer, so the easiest way to provide the wrapper function
 * is just to duplicate the simple function as we do here.
 */

struct addrinfo *
Host_serv(const char *host, const char *serv, int family, int socktype)
{
	int	n;
	struct addrinfo	hints, *res;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;	/* always return canonical name */
	hints.ai_family = family;	/* 0, AF_INET, AF_INET6, etc. */
	hints.ai_socktype = socktype;	/* 0, SOCK_STREAM, SOCK_DGRAM, etc. */

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
	{
	fprintf(stderr,"host_serv error for %s, %s: %s",
	 (host == NULL) ? "(no hostname)" : host,
	 (serv == NULL) ? "(no service name)" : serv,
	 gai_strerror(n));
	exit(1);
	}

	return(res);	/* return pointer to first on linked list */
}



char *
sock_ntop_host(const struct sockaddr *sa, socklen_t salen)
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

char *
Sock_ntop_host(const struct sockaddr *sa, socklen_t salen)
{
	char	*ptr;

	if ( (ptr = sock_ntop_host(sa, salen)) == NULL)
	{
	fprintf(stderr,"sock_ntop_host error");	/* inet_ntop() sets errno */
	exit(1);
	}
	
	return(ptr);
}

void
readloop(void)
{
	int	size;
	char recvbuf[BUFSIZE];
	char controlbuf[BUFSIZE];
	struct msghdr	msg;
	struct iovec	iov;
	ssize_t	n;
	struct timeval	tval;

	sockfd = socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
	setuid(getuid());	/* don't need special permissions any more */
	if (pr->finit)
	(*pr->finit)();

	size = 60 * 1024;	/* OK if setsockopt fails */
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	sig_alrm(SIGALRM);	/* send first packet */

	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);
	msg.msg_name = pr->sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = controlbuf;
	for ( ; ; ) {
	msg.msg_namelen = pr->salen;
	msg.msg_controllen = sizeof(controlbuf);
	n = recvmsg(sockfd, &msg, 0);
	if (n < 0) {
	if (errno == EINTR)
	continue;
	else
	{
	fprintf(stderr,"recvmsg error");
	exit(1);
	}
	}

	gettimeofday(&tval, NULL);
	(*pr->fproc)(recvbuf, n, &msg, &tval);
	}
}


void
proc_v4(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv)
{
	int	hlen1, icmplen;
	double	rtt;
	struct ip	*ip;
	struct icmp	*icmp;
	struct timeval	*tvsend;

	ip = (struct ip *) ptr;	/* start of IP header */
	hlen1 = ip->ip_hl << 2;	/* length of IP header */
	if (ip->ip_p != IPPROTO_ICMP)
	return;	/* not ICMP */

	icmp = (struct icmp *) (ptr + hlen1);	/* start of ICMP header */
	if ( (icmplen = len - hlen1) < 8)
	return;	/* malformed packet */

	if (icmp->icmp_type == ICMP_ECHOREPLY) {
	if (icmp->icmp_id != pid)
	return;	/* not a response to our ECHO_REQUEST */
	if (icmplen < 16)
	return;	/* not enough data to use */

	tvsend = (struct timeval *) icmp->icmp_data;
	tv_sub(tvrecv, tvsend);
	rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;

	printf("%d bytes from %s: seq=%u, ttl=%d, rtt=%.3f ms\n",
	icmplen, sock_ntop_host(pr->sarecv, pr->salen),
	icmp->icmp_seq, ip->ip_ttl, rtt);

	} else if (verbose) {
	//接收到的数据不ICMP_ECHOREPLY	0 回射应答报文
	//可能为#define ICMP_DEST_UNREACH	3  目标不可达
	//对本地进行ping会收到#define ICMP_ECHO	8  回射请求报文
	printf("  %d bytes from %s: type = %d, code = %d\n",
	icmplen, sock_ntop_host(pr->sarecv, pr->salen),
	icmp->icmp_type, icmp->icmp_code);
	}
}

void
tv_sub(struct timeval *out, struct timeval *in)
{
	if ( (out->tv_usec -= in->tv_usec) < 0) {	/* out -= in */
	--out->tv_sec;
	out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

void
sig_alrm(int signo)
{
	(*pr->fsend)();

	alarm(1);
	return;
}

void
send_v4(void)
{
	int	len;
	struct icmp	*icmp;

	icmp = (struct icmp *) sendbuf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = pid;
	icmp->icmp_seq = nsent++;
	memset(icmp->icmp_data, 0xa5, datalen);	/* fill with pattern */
	gettimeofday((struct timeval *) icmp->icmp_data, NULL);

	len = 8 + datalen;	/* checksum ICMP header and data */
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = in_cksum((u_short *) icmp, len);

	sendto(sockfd, sendbuf, len, 0, pr->sasend, pr->salen);
}

uint16_t
in_cksum(uint16_t *addr, int len)
{
	int nleft = len;
	uint32_t	sum = 0;
	uint16_t	*w = addr;
	uint16_t	answer = 0;

	/*
	 * Our algorithm is simple, using a 32 bit accumulator (sum), we add
	 * sequential 16 bit words to it, and at the end, fold back all the
	 * carry bits from the top 16 bits into the lower 16 bits.
	 */
	while (nleft > 1)  {
	sum += *w++;
	nleft -= 2;
	}

	/* 4mop up an odd byte, if necessary */
	if (nleft == 1) {
	*(unsigned char *)(&answer) = *(unsigned char *)w ;
	sum += answer;
	}

	/* 4add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);	/* add carry */
	answer = ~sum;	/* truncate to 16 bits */
	return(answer);
}
