//��֤getsockname,getpeername 
#include<fcntl.h>
#include <stdio.h>
#include <string.h>
//network init
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#define BUF_LEN 1024
#define BACKLOG 10 
int main(int argc,char*argv[])
{
	//��ȡ�豸��ʼ��
	int fd=0;
	int r=0;
	char buf[BUF_LEN],buft[BUF_LEN];
	char IP[16];
	//char stderr[64];
	
	//�����������ʼ��
	int client_fd;//socket;client_fd:���ݴ���socket
	struct sockaddr_in my_addr;//���ص�ַ 
	struct sockaddr_in remote_addr;//��������ַ��Ϣ 
	struct hostent *host;
	
	sprintf(IP,"192.168.1.4");
	printf("IP:%s\n",IP);
	
	if((host=gethostbyname(IP))==NULL)//(argv[1]))==NULL) 
	 { 
       		printf("Gethostname error\n"); 
       		 exit(1); 
	} 

	
	if (-1==(client_fd=socket(AF_INET,SOCK_STREAM,0)))
	{	
		printf("socket �������� ��");
		 exit(1);
	}
	//������˵�ַ��Ϣ
	remote_addr.sin_family=AF_INET;
	remote_addr.sin_port=htons(33333);
	remote_addr.sin_addr=*((struct in_addr *)host->h_addr); 
	bzero(&(remote_addr.sin_zero),8);
	
	if(connect(client_fd,(struct sockaddr *)(&remote_addr),sizeof(struct sockaddr))==-1) 
	{ 
	  printf("Connect Error\a\n"); 
	  exit(1); 
	} 	
		
	socklen_t len;
	len=sizeof(my_addr);
	if(getsockname(client_fd,(struct sockaddr*)&my_addr,&len))
	return(-1);
	printf("client addr: IP:%s port:%d \n",inet_ntoa(my_addr.sin_addr),ntohs(my_addr.sin_port));
	

	close(client_fd); 
	exit(0);
}
