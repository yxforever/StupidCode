//问题：即使没有服务端，这个客户端也一样貌似正常的工作，因为其自己能够接收自己发送的数据（本机测试）
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




void dg_cli(FILE *fp,int sockfd,const struct sockaddr* pservaddr,socklen_t servlen)//缺乏流量控制
{

  int n,hlen;
  struct ip *ip;
	char *data;
	char sendline[512],recvline[1025];
	bzero(sendline,sizeof(sendline));
	while(fgets(sendline,1024,fp)!=NULL)
	{	
		sendline[strlen(sendline)]='\0';
		printf("sendto :%s \n",sendline);
		sendto(sockfd,sendline,sizeof(sendline),0,pservaddr,servlen);
		n=recvfrom(sockfd,recvline,1024,0,NULL,NULL);
		if(n>0)
		ip=(struct ip *)recvline;
		hlen = ip->ip_hl<<2;
		data=(char *)(recvline +hlen);
		
		fputs(data,stdout);
	}
}




int main(int argc,char*argv[])
{
	int sockfd;
 	struct sockaddr_in servaddr;
 	
 	char buff[1024];
 	int n,len;
 	

 	
 	/* 创建一个原始数据报类型的套接字 */
 	
	bzero(&servaddr,sizeof(servaddr));
 	servaddr.sin_family=AF_INET;
 	if(argc!=2)
 		{
 			printf("./RAW_client IP地址 \n");
 			exit(1);
 		}
  	inet_aton(argv[1],&servaddr.sin_addr);
 	//servaddr.sin_port=htons(SERVPORT);
 	
 	sockfd=socket(AF_INET,SOCK_RAW,14);
	if(sockfd<0)
 	{
		fprintf(stderr,"Socket error");
		exit(1);
 	}
	
	dg_cli(stdin,sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	exit(0);
}

