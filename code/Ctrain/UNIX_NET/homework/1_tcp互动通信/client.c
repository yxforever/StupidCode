#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h> 
#define DEST_PORT 9999//目标地址端口号 
#define DEST_IP "127.32.255.2"/*目标地址IP，这里设为本机，不一定非得是127.0.0.1,只要127开头并且不是127.0.0.0和127.255.255.255即可*/ 
#define MAX_DATA 100//接收到的数据最大程度 
 
int main(){
	int sockfd;
	struct sockaddr_in dest_addr;
	char buf[MAX_DATA];
 
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	
	dest_addr.sin_family=AF_INET;
 	dest_addr.sin_port=htons(DEST_PORT);
	dest_addr.sin_addr.s_addr=inet_addr(DEST_IP);
	bzero(&(dest_addr.sin_zero),8);
	connect(sockfd,(struct sockaddr*)&dest_addr,sizeof(struct sockaddr));

	printf("connect success\n");
	while(1){
		char send_buf[512] = "";
        printf("send:");        
		scanf("%s",&send_buf);
		write(sockfd,send_buf,sizeof(send_buf));

        read(sockfd,send_buf,sizeof(send_buf));
    	printf("receive:%s\n",send_buf);        



	}

	return 0;
}
