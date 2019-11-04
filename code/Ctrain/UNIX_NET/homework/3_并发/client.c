#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include <unistd.h> 
#define DEST_PORT 9999//目标地址端口号 
#define DEST_IP "127.0.0.1"/*目标地址IP，这里设为本机，不一定非得是127.0.0.1,只要127开头并且不是127.0.0.0和127.255.255.255即可*/ 
#define MAX_DATA 100//接收到的数据最大程度 
 
int main(){
	int sockfd,n=0;
	struct sockaddr_in dest_addr;
	char buf[MAX_DATA];
 
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	
	dest_addr.sin_family=AF_INET;
 	dest_addr.sin_port=htons(DEST_PORT);
	dest_addr.sin_addr.s_addr=inet_addr(DEST_IP);
	bzero(&(dest_addr.sin_zero),8);
	connect(sockfd,(struct sockaddr*)&dest_addr,sizeof(struct sockaddr));

	//	char send_buf[512] = "";
	//	scanf("%s",&send_buf);
	//	write(sockfd,send_buf,sizeof(send_buf));
	memset(buf, 0, sizeof(buf));
    strcpy(buf, "TIME");
    /*发送数据*/
    send(sockfd, buf, sizeof(buf), 0);
    memset(buf, 0, sizeof(buf));
    /*接收数据*/    
    	if(recv(sockfd, buf, sizeof(buf), 0)>0){
		printf("已连接至服务器\n");
		//read(sockfd,buf,sizeof(buf));
    	printf("TIME:%s\n",buf);
		sleep(10);
		}
		else{
			printf("无法连接至服务器\n");	
		}

	return 0;
}
