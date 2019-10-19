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
#define SERVPORT 3333	
#define NDG 2000
#define DGLEN 1400

void dg_cli_loop(FILE *fp,int sockfd,const struct sockaddr* pservaddr,socklen_t servlen)
{
	int i;
	char sendline[DGLEN];
	for(i=0;i<NDG;i++)
	{
		sendto(sockfd,sendline,DGLEN,0,pservaddr,servlen);
	}
	printf("\n sended %d datagrams \n",NDG);
	
}


void dg_cli(FILE *fp,int sockfd,const struct sockaddr* pservaddr,socklen_t servlen)//ȱ����������
{
	int n;
	char sendline[1024],recvline[1025];
	while(fgets(sendline,1024,fp)!=NULL)
	{
		sendto(sockfd,sendline,strlen(sendline),0,pservaddr,servlen);
		n=recvfrom(sockfd,recvline,1024,0,NULL,NULL);
		recvline[n]='\0';
		fputs(recvline,stdout);
	}
	
}


void dg_cliconn(FILE *fp, int sockfd, const struct sockaddr* pservaddr, socklen_t servlen)//������UDP�׽ӿ�
{
	int	n;
	char	sendline[1024], recvline[1025];

	connect(sockfd, (struct sockaddr*) pservaddr, servlen);
	while (fgets(sendline, 1024, fp) != NULL) {
//����û������Ӧ��ַ��˿������з������ģ�write����ICMP����TCP�ͻ����̵���connectʱ��ͷ���ͬ������
		write(sockfd, sendline, strlen(sendline));
		n=0;
		n = read(sockfd, recvline, 1024);
		recvline[n] = 0;	/* null terminate */
		fputs(recvline, stdout);
	}
}

int main(int argc,char*argv[])
{
	int sockfd;
 	struct sockaddr_in servaddr;
 	
 	char buff[1024];
 	int n,len;
 	
// 	if(argc !=2) 
// 		{
 //			printf("usage: UDPclient <IPaddress>\n");
 //			exit (1);
 //		}
 	
 	/* ����һ��UDP���ݱ����͵��׽��� */
 	
	bzero(&servaddr,sizeof(servaddr));
 	servaddr.sin_family=AF_INET;
  	inet_aton("127.0.0.1",&servaddr.sin_addr);
 	servaddr.sin_port=htons(SERVPORT);
 	
 	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
 	{
		fprintf(stderr,"Socket error");
		exit(1);
 	}
	
	if(argc==1)
		dg_cli(stdin,sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));//�����ͻ������ݷ���
		//dg_cli_loop(stdin,sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));//��������
	else 
		dg_cliconn(stdin,sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	exit(0);
}

