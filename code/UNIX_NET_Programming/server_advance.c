//�߼���̽ӿڷ����
//����ʹ��:����setsockopt������SO_REUSEADDR�����Ϳ����ˡ�Ȼ�����۽�������˳����˿ڶ����ᱻռ��

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
#include <time.h>
#include <sys/uio.h>

 
#define BUF_LEN 102400		//�豸��ȡ����Ĵ�С
#define SERVPORT 3333	//�����������οں�
#define BACKLOG 10		//���ͬʱ����������
ssize_t						/* Read "n" bytes from a descriptor. */
readn(int fd, void *vptr, size_t n)
{
	size_t	nleft;
	ssize_t	nread;
	char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				nread = 0;		/* and call read() again */
			else
				return(-1);
		} else if (nread == 0)
			break;				/* EOF */
		
		nleft -= nread;
		printf("nread:%d , nleft :%d\n",nread,nleft);
		ptr   += nread;
	}
	return(n - nleft);		/* return >= 0 */
}
/* end readn */
ssize_t
Readn(int fd, void *ptr, size_t nbytes)
{
	ssize_t		n;

	if ( (n = readn(fd, ptr, nbytes)) < 0)
		printf("readn error");
	return(n);
}

int main(void)
{
	//��ʼ��
	int fd=0;
	ssize_t r=0;
	char buf[BUF_LEN];
	char strbuf[1024];

      
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
				size_t len;
//read write ��֤
/*				
				len=51803;
				r=Readn(client_fd,buf,len);
				if(r>=0)printf("read buf : %s r= %d len=%d \n",buf,r,len);
					buf[0]='\0';
				printf("Readn end!\r\n");
				
				
				r=0;
*/				
				//MSG_PEEK��֤
/*				
				len=1024;
				r= recv (client_fd,strbuf,len,MSG_PEEK);
				printf("recv1: %s ,nrd=:%d  !\n",strbuf,r);
				
				strbuf[0]='\0';
				r=recv(client_fd,strbuf,len,0);
				printf("recv2: %s ,r=:%d  !\n",strbuf,r);
				
				strbuf[0]='\0';
				r=recv(client_fd,strbuf,len,0);
				printf("recv3: %s ,r=:%d  !\n",strbuf,r);
*/				
				
				//MSG_WAITALL ��֤
				/*
				len=48;
				r = recv (client_fd,strbuf,len,MSG_WAITALL);
				printf("recv4: %s ,r=:%d  !\n",strbuf,r);
				*/
				
				//readv writev��֤server
				struct iovec vector[3];
				char buf1[12];
				char buf2[2];
				char buf3[128];
				
				vector[0].iov_base=buf1;
				vector[0].iov_len=12;
				vector[1].iov_base=buf2;
				vector[1].iov_len=2;
				vector[2].iov_base=buf3;
				vector[2].iov_len=128;
				r=readv(client_fd,vector,3);
				printf("r: %d ,\nbuf1: %s \nbuf2: %s \nbuf3: %s \n",r,buf1,buf2,buf3);
				
				
				close(client_fd);
				exit(0);
			}
			close(client_fd);
		}
		return 0;
}

