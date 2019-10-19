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

void udps_respon(int sockfd,struct sockaddr* pcliaddr,socklen_t clilen)
{
	int n;
  	char msg[1024];
  	socklen_t len;
 	for(;;)
  	{
		len=clilen;
		n=recvfrom(sockfd,msg,1024,0,pcliaddr,&len);
 		/* ��Ӧ�ͻ������� */
 		msg[n]='\0';
 		printf("recvfrom :%s \n",msg);
 		sendto(sockfd,msg,n,0,pcliaddr,len);
  	}
}

int main(int argc,char*argv[])
{
	int sockfd;
 	struct sockaddr_in servaddr,cliaddr;
 	/* ����һ��UDP���ݱ����͵��׽��� */
 	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
 	{
		fprintf(stderr,"Socket error");
		exit(1);
 	}
	bzero(&servaddr,sizeof(servaddr));
 	servaddr.sin_family=AF_INET;
  	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
 	servaddr.sin_port=htons(SERVPORT);

	/* ������Ϊ�׽��ְ�һ���˿ں� */
 	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
 	{
		fprintf(stderr,"Bind error");
		exit(1);
	}
	/* ����ͨ�ź�����ͻ��˽���ͨ�� */
	udps_respon(sockfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));	   
   	close(sockfd); /* �ر��׽��� */
}
