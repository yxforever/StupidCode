#include	"my.h"
void sig_chld(int signo);
int
main(int argc, char **argv) 
{
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	Signal ( SIGCHLD, sig_chld );
	Listen(listenfd, LISTENQ);
	for ( ; ; ) {
	clilen = sizeof(cliaddr);
	/*	connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);	*/

	if((connfd = accept(listenfd, (SA *) &cliaddr, &clilen))<0){
		if(errno==EINTR)
				continue;
			else
					err_sys("accept error");					
	}

if ( (childpid = Fork()) == 0) {	/* child process */
			Close(listenfd);	/* close listening socket */
			str_echo(connfd);	/* process the request */
			exit(0);
		}
		Close(connfd);			/* parent closes connected socket */
	}
}

void sig_chld(int signo)
{
	pid_t	pid;
	int		stat;
	pid = wait(&stat);
	printf("child %d terminated\n", pid);
	return;
}
