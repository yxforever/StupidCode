
#include <string.h>
//network init
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
 
#define BUF_LEN 1024		//�豸��ȡ����Ĵ�С
#define SERVPORT 3333	//�����������οں�
#define BACKLOG 10		//���ͬʱ����������

int main(void)
{
	//��ʼ��
	int fd=0;
	int r=0;
	char buf[BUF_LEN];

      
	time_t now;
	char *str_now;

	//�����������ʼ��
	int sock_fd,client_fd;//sock_fd:����socket;client_fd:���ݴ���socket
	struct sockaddr_in my_addr;//���ص�ַ 
	struct sockaddr_in remote_addr;//�ͻ���ַ��Ϣ 
	
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(SERVPORT);
	my_addr.sin_addr.s_addr=INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);
	
	if (-1==(sock_fd=socket(PF_INET,SOCK_STREAM,0)))
	{	
		printf("socket �������� ��");
		return -1;
	}
	if(-1==bind(sock_fd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr)))
	{
		printf("bind ����");	
		return -1;
	}
	//printf("print 'I' to select iterative server!\nprint 'C' to select concurrent server!\n"); 	//I�ظ���������C����������
	printf("����'I'ѡ�ظ�������!\n����'C'ѡ����������!\n"); 	//I�ظ���������C����������
	//r='I';
	r=getchar();
		
	if('I'==r)
	{
		printf("getchar:%c\n",r);
		//ѭ��������
		if(-1==listen(sock_fd,100))//�������1������
		{
			printf("listen����!");	
			return -1;
		}
		
	
		while(1)
		{
			int sin_size=sizeof(struct sockaddr_in);
			if(-1==(client_fd=accept(sock_fd,(struct sockaddr *)&remote_addr,&sin_size)))
			{
				printf("accept ����");
				continue;
			}
			printf("received a connection from %s \n",inet_ntoa(remote_addr.sin_addr));
		
	
			if(-1==send(client_fd,"Hello,you are connected !\n",BUF_LEN,0))//���ͻ�ӭ��
			printf("send ����");
			if(-1==(r=recv(client_fd,buf,BUF_LEN,0)))//����RD
			printf("recv ����");
			buf[r]='\0';
			printf("RD?:%s\n",buf);
	
			printf("will send the data! \n");
				
			if(-1==(r=time(&now)))printf("time ����");
				//printf("now:%d r:%d",now,r);
			str_now=ctime(&now);
	
	
			if((buf[0]='R')&&(buf[1]=='D'))//�ͻ���׼�����˸�������Ϣ
			{
				sprintf(buf,"Service time : %s",str_now);
				send(client_fd,buf,BUF_LEN,0);
			}
	
			printf("print 'Enter' to end current socket!\n"); 		
		//	for(;;)
		//	{
		//		r=getchar();
		//		if (r!='\0') break;
		//		printf("put into a char! \n");
		//	}

			sleep(100);
			printf("send END !\n");
			close(client_fd);
		}
	}//ѭ��������end	


	else if('C'==r)
	{
		
		printf("getchar:%c\n",r);
		//����������
		if(-1==listen(sock_fd,BACKLOG))
		{
			printf("listen����!");	
			return -1;
		}
		
	
		while(1)
		{
			int sin_size=sizeof(struct sockaddr_in);
			int re;
			if(-1==(client_fd=accept(sock_fd,(struct sockaddr *)&remote_addr,&sin_size)))
			{
				printf("accept ����");
				continue;
			}
			printf("received a connection from %s \n",inet_ntoa(remote_addr.sin_addr));
		
			if(!fork())
			{
				if(-1==send(client_fd,"Hello,you are connected !\n",BUF_LEN,0))//���ͻ�ӭ��
				printf("send ����");
				if(-1==(re=recv(client_fd,buf,BUF_LEN,0)))//����RD
				printf("recv ����");
	
				printf("RD?:%s\n",buf);
	
				printf("will send the data! \n");
				
				if(-1==(r=time(&now)))printf("time ����");
				//printf("now:%d r:%d",now,r);
				str_now=ctime(&now);
	
	
				if((buf[0]='R')&&(buf[1]=='D'))//�ͻ���׼�����˸�������Ϣ
				{
					sprintf(buf,"Service time : %s",str_now);
					send(client_fd,buf,BUF_LEN,0);
				}
				
				
				printf("send END !\n");
				close(client_fd);
				exit(0);
			}
			close(client_fd);
		}
	
	
	}

		
		return 0;
		
}
