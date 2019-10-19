#include	"my.h"
#include <unistd.h>
#include <string.h>
void	str_cli(FILE *, int,char* );
int
main(int argc, char **argv)
{
	int					sockfd; 
	int t;
	struct sockaddr_in	servaddr;
	char buf[1024];

	if (argc != 3)
		err_quit("usage: tcpcli <IPaddress> <server sleep time>");
	//t=atoi(argv[2]);
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(3333);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	strcpy(buf,argv[2]);
	str_cli(stdin, sockfd,buf);		/* do it all */
	exit(0);
}


void
str_cli(FILE *fp,int sockfd,char* tbuf)
{
	int			maxfdp1, val, stdineof;
	ssize_t		n, nwritten;
	fd_set		rset;
	int a,b,i,flga=0,flgb=0,flgt=0;
	char buf[1024];
	char buf2[1024];
	
	val = Fcntl(sockfd, F_GETFL, 0);
	Fcntl(sockfd, F_SETFL, val | O_NONBLOCK);//�׽ӿ�����Ϊ������
	val = Fcntl(STDIN_FILENO, F_GETFL, 0);
	Fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);//��׼��������Ϊ������

	maxfdp1 = sockfd + 1;
	for (;;) {
		printf("ѭ��flga=%d,flgb=%d,flgt=%d\n",flga,flgb,flgt);
		if(flgt==0){
			printf("����t\n");
			nwritten = Write(sockfd, tbuf, 1024);
			flgt=1;
		}
		FD_ZERO(&rset);
		//FD_ZERO(&wset);
		if(flga==0)FD_SET(STDIN_FILENO, &rset);	/* ��׼����ɶ� */
		if(flgb==0)FD_SET(sockfd, &rset);			/* ���׽ӿڶ��� */
		Select(maxfdp1, &rset, NULL, NULL, NULL);
		if (FD_ISSET(STDIN_FILENO, &rset)) {//��׼����ɶ�
			printf("��׼����ɶ�\n");
			if ( (n = read(STDIN_FILENO, buf, sizeof(buf))) < 0) 
				{
					if (errno != EWOULDBLOCK)
					err_sys("read error on stdin");
				}
			else if(n>0)
			{
						buf[n]=0;
						a=atoi(buf);
						printf("��׼���룺%d\n",a);
						flga=1;
			}
			printf("n=%d a=%d\n",n,a);
		}//��׼����ɶ�����
		
		if (FD_ISSET(sockfd, &rset)) 
			{//�׽ӿڿɶ�
				printf("�׽ӿڿɶ�\n");
				if ( (n = read(sockfd, buf2, 1024)) < 0)
				{
					if (errno != EWOULDBLOCK)err_sys("read error on socket");			
				}
			else if(n>0){
						buf2[n]=0;
						b=atoi(buf2);
						printf("����������ֵ��%s\n",buf2);
						flgb=1;
				}
			else if(n==0)
				{
						printf("����������\n");
						
				}
		}//��������������
		if((flga==1)&&(flgb==1))
			{
				printf("���εõ������%d+%d=%d\n",a,b,a+b);
				sleep(2);
				flga=0;
				flgb=0;
				flgt=0;
				//exit(0);
			}
	}	
}
