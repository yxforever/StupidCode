//问题：一旦被客户端触发以后，会不断发送数据并且自己可以接受（本机测试时），进入“发送——》接收”的死循环
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
void raws_respon(int sockfd,struct sockaddr* pcliaddr,socklen_t clilen)
{
	int n,hlen1;
  	char msg[1024];
  	socklen_t len;
  	struct ip *ip;
  	struct sockaddr_in *addr;
  	char *data;
  	union {
		int  s;
		char   c[sizeof(int)];
    	} un;
 	for(;;)
  	{
  	n=0;
		len=clilen;
		n=recvfrom(sockfd,msg,1024,0,pcliaddr,&len);
 		if(n>0)
 		{
	 		ip = (struct ip *) msg;	
	 		hlen1 = ip->ip_hl << 2;	//IP首部长度＝ip->ip_hl×4
	 		printf("ip_P: %d----",ip->ip_p);
	 //		if((ip->ip_p!=14)||n<532)continue;//不是我们的数据不处理
	 		data=(char*)(msg+hlen1);
	 		addr=(struct sockaddr_in*)pcliaddr;
	 		printf("recvfrom IP:%s data:%s  n=%d\n",inet_ntoa(addr->sin_addr),data,n);
	 		printf("sendto data : %s datalen=%d \n",data,strlen(data));
	 		sprintf(msg,"get data:%s",data);
			sendto(sockfd,msg,strlen(msg),0,pcliaddr,len);
		
 		}
 		else if(errno == EINTR)
 		continue;
 		else {
 			fprintf(stderr,"recvfrom error");
			exit(1);
 			}
  	}
}


int main(int argc,char*argv[])
{
	int sockfd;
 	struct sockaddr_in servaddr,cliaddr;
 	/* 创建一个RAW数据报类型的套接字 */
 	sockfd=socket(AF_INET,SOCK_RAW,14);//协议字段设置为14
	if(sockfd<0)
 	{
		fprintf(stderr,"Socket error! %s\n",strerror(errno));
		exit(1);
 	}
	bzero(&servaddr,sizeof(servaddr));
 	servaddr.sin_family=AF_INET;
 	 if(argc!=2)
 	{
 		printf("./RAW_server IP地址 \n");
 		exit(1);
 	}
 	inet_aton(argv[1],&servaddr.sin_addr);
  	//servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
 	//servaddr.sin_port=htons(SERVPORT);

	/* 服务器为套接字绑定IP地址，地址为0时表示通配符 */
 	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
 	{
		fprintf(stderr,"Bind error");
		exit(1);
	}
	/* 调用通信函数与客户端进行通信 */
	raws_respon(sockfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));	 //正常服务器通信处理  
   	close(sockfd); /* 关闭套接字 */
}
