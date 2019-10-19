#include	"my.h"
#include	<netinet/in_systm.h>
#include	<netinet/ip.h>
#include	<netinet/ip_icmp.h>
#include	<netinet/udp.h>
#include <unistd.h>
#include <netdb.h> 
#include <time.h>

#define	BUFSIZE		1500

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
int		 probe, nprobes;//测探分组数量
int		 sendfd, recvfd;	/* send on UDP sock, read on raw ICMP sock */
int		 ttl, max_ttl;
int		 verbose;

			/* function prototypes */
const char	*icmpcode_v4(int);
//const char	*icmpcode_v6(int);
int		 recv_v4(int, struct timeval *);
//int		 recv_v6(int, struct timeval *);
void	 sig_alrm(int);
void	 traceloop(void);
void	 tv_sub(struct timeval *, struct timeval *);

struct proto {
  const char	*(*icmpcode)(int);//ICMP数据报信息的功能类型
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

/*
#ifdef	IPV6

#include	<netinet/ip6.h>
#include	<netinet/icmp6.h>

#endif
*/
char *Sock_ntop_host(const struct sockaddr *sa, socklen_t salen);
struct addrinfo *Host_serv(const char *host, const char *serv, int family, int socktype);
void *Calloc(size_t n, size_t size);
