#include<stdlib.h> 
#include	"my.h"

int
main(int argc, char **argv)
{
	int					i, maxi, maxfd, listenfd, connfd, sockfd;
	int					nready, client[FD_SETSIZE];
	ssize_t				n; 
	fd_set				rset, allset;
	int				t,b;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
	char buf[1024];
	char buf2[1024]="23";

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(3333);

	Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;			/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	for ( ; ; ) {
		rset = allset;
		nready = Select(maxfd+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)) {	/* new client connection */
			printf("listening socket readable\n");
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
#ifdef	NOTDEF
			printf("new client: %s, port %d\n",
					Inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
					ntohs(cliaddr.sin_port));
#endif

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					client[i] = connfd;	/* save descriptor */
					break;
				}
			if (i == FD_SETSIZE)
				err_quit("too many clients");

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		for (i = 0; i <= maxi; i++) {	/*获取客户端提供的sleep时间，返回rand产生的随机数 */
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				if ( (n = read(sockfd, buf, sizeof(buf))) == 0) {
						 /* connection closed by client */
					printf("结束接收 \n");
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				}
			else	if(n>0){
					buf[n]=0;
					t=atoi(buf);
					printf("get sleep time %d \n",t);
					sleep(t);
					b=rand();
					//buf2[1024]="12";
					//write(sockfd,buf2,1024);
					sprintf(buf2,"%d",b);
					printf("send rand nunber b  %d %s\n",b,buf2);
					printf("write :%d",Write(sockfd, buf2, sizeof(buf2)));
					}
				if (--nready <= 0)
				break;				/* no more readable descriptors */
			}
		}
	}
}
