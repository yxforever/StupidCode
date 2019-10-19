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
int timed_out;

void sig_handler();
int main(int argc,char*argv[])
{
	
	struct sigaction act;
	char msg[1024],buff[1024];
	
	int sockfd,num;
 	struct sockaddr_in servaddr;
 	int n,len;
	if(argc !=2) 
		{
 			printf("usage: UDPclient <IPaddress>\n");
 			exit (1);
 		}
 		
	/* 创建一个UDP数据报类型的套接字 */
	bzero(&servaddr,sizeof(servaddr));
 	servaddr.sin_family=AF_INET;
  inet_aton(argv[1],&servaddr.sin_addr);
 	servaddr.sin_port=htons(SERVPORT);
 	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
 	{
		fprintf(stderr,"Socket error");
		exit(1);
 	}
	
	act.sa_handler = sig_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGALRM,&act,NULL);


	char sendline[1024],recvline[1025];
	while(fgets(sendline,1024,stdin)!=NULL)
	{
		num=0;
		sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
		for (;;)//只完成一次应答的读取
		{
			timed_out = 0;                    
			/* 设立信号机制,发送后等待20 s后,若无返回,则认为发送超时,重新发送 */
			alarm(20); 
			/* 读取应答 */
			
			n=recvfrom(sockfd,recvline,1024,0,NULL,NULL);
			printf("recvfrom ! %d  \n",n);
			if(n>0)
			{
				recvline[n]='\0';
				fputs(recvline,stdout);
				break;
			}
			else if (n<0)
			{
				if(errno == EINTR)
				{
					if (timed_out) 
					{
	 					printf("Server not responding, retrying... num=%d\n",num);
			        		/* 重新发送数据 */
			        		if (num>3)
			        		{
			        			printf("send msg %s failed! \n",sendline);
			        			break;
			        		}
						sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
						num++;
					}		
	    				continue;
				}
	  		 	printf("recvfrom ERRO \n");
	    		}
		} /* for */
		
	}
	
	
	
	
}  /* main */

void sig_handler()       /*信号处理函数*/
{
 timed_out = 1;
};
