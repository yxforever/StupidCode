//�߼���̽ӿڷ����,close��shutdown����
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
			printf("received a connection from %s:%d \n",inet_ntoa(remote_addr.sin_addr),ntohs(remote_addr.sin_port));
		
			if(!fork())
			{
				
				size_t len=33;
				
/*				if(r=recv(client_fd,strbuf,len,0)) 
				{
					printf("recv: %s r=:%d  !\n",strbuf,r);
					strbuf[0]='\0';
					shutdown(client_fd,SHUT_RD);
					sprintf(strbuf,"ACK,shutdown SHUT_RD! \n");
					r=send(client_fd,strbuf,strlen(strbuf),0);
					r=recv(client_fd,strbuf,len,0);
					printf("send ACK : %s r=%d \n",strbuf,r);
				}
				
				strbuf[0]='\0';
				r=recv(client_fd,strbuf,len,0);
				printf("Last recv: %s r=:%d  !\n",strbuf,r);
				
				printf("close client_fd \n");
*/
			if(r=recv(client_fd,strbuf,len,0)) 
			{
				printf("recv: %s r=:%d  !\n",strbuf,r);
				strbuf[0]='\0';
			}
			shutdown(client_fd,SHUT_WR);
			if(r=recv(client_fd,strbuf,len,0)) 
			{
				printf("after SHUT_WR recv: %s r=:%d  !\n",strbuf,r);
				strbuf[0]='\0';
			}
			
			close(client_fd);
			exit(0);
			}
			close(client_fd);
		}
		return 0;
}

