//���⣺һ�����ͻ��˴����Ժ󣬻᲻�Ϸ������ݲ����Լ����Խ��ܣ���������ʱ�������롰���͡��������ա�����ѭ��
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
	 		hlen1 = ip->ip_hl << 2;	//IP�ײ����ȣ�ip->ip_hl��4
	 		printf("ip_P: %d----",ip->ip_p);
	 //		if((ip->ip_p!=14)||n<532)continue;//�������ǵ����ݲ�����
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
 	/* ����һ��RAW���ݱ����͵��׽��� */
 	sockfd=socket(AF_INET,SOCK_RAW,14);//Э���ֶ�����Ϊ14
	if(sockfd<0)
 	{
		fprintf(stderr,"Socket error! %s\n",strerror(errno));
		exit(1);
 	}
	bzero(&servaddr,sizeof(servaddr));
 	servaddr.sin_family=AF_INET;
 	 if(argc!=2)
 	{
 		printf("./RAW_server IP��ַ \n");
 		exit(1);
 	}
 	inet_aton(argv[1],&servaddr.sin_addr);
  	//servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
 	//servaddr.sin_port=htons(SERVPORT);

	/* ������Ϊ�׽��ְ�IP��ַ����ַΪ0ʱ��ʾͨ��� */
 	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
 	{
		fprintf(stderr,"Bind error");
		exit(1);
	}
	/* ����ͨ�ź�����ͻ��˽���ͨ�� */
	raws_respon(sockfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));	 //����������ͨ�Ŵ���  
   	close(sockfd); /* �ر��׽��� */
}
