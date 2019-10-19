//P14 �������������ظ��������Ļ�ȡ������ʱ�䣬�ͻ�
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

#define BUF_LEN 1024		//�豸��ȡ����Ĵ�С
#define SERVPORT 3333	//�����������οں�
#define BACKLOG 10		//���ͬʱ����������

int main(int argc,char*argv[])
{
	//��ȡ�豸��ʼ��
	int fd=0;
	int r=0;
	char buf[BUF_LEN];
	char IP[16];
	//char stderr[64];
	
	//�����������ʼ��
	int client_fd;//socket;client_fd:���ݴ���socket
	struct sockaddr_in my_addr;//���ص�ַ 
	struct sockaddr_in remote_addr;//��������ַ��Ϣ 
	struct hostent *host;
	
	sprintf(IP,"127.0.0.1");
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
	remote_addr.sin_port=htons(SERVPORT);
	remote_addr.sin_addr=*((struct in_addr *)host->h_addr); 
	bzero(&(remote_addr.sin_zero),8);
	
	if(connect(client_fd,(struct sockaddr *)(&remote_addr),sizeof(struct sockaddr))==-1) 
	{ 
	  printf("Connect Error\a\n"); 
	  exit(1); 
	} 	
	
	if ((r=recv(client_fd,buf,BUF_LEN,0)) == -1) 		
	{ 
	        printf("Read Error\n"); 
	        exit(1); 
	 } 
	 
 	buf[r]='\0'; 
 	printf("I have received:%s\n",buf); 
 	
 	
 	if(-1==send(client_fd,"RD",2,0))//׼���������ݣ�������Ϣ����ʱ����Ϣ
	printf("send ����");
 	
 	if ((r=recv(client_fd,buf,BUF_LEN,0)) == -1) 		
	{ 
	        printf("Read Time Error:\n"); 
	        exit(1); 
	 } 
	 
 	buf[r]='\0'; 
 	printf("Server Time is :%s\n",buf); 
 	printf("print 'Enter' to end socket!\n"); 	
 	getchar();
 
/* ����ͨ�� */ 
 close(client_fd); 




	
	exit(0);
	
}
