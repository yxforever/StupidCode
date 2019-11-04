#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include<signal.h>
#include<wait.h>
#include <time.h>
#include<signal.h>
#include <time.h>

int main(int argc,char* argv[]){
    int sockfd;  
    char buf[1024];  
    struct sockaddr_in server_addr;  
    int addrlen = sizeof(struct sockaddr);  
    if(argc < 3){  
        fprintf(stderr,"运行格式: %s ip port.\n",argv[0]);  
        exit(EXIT_FAILURE);  
    }

    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0){  
        perror("socket");  
    }
 
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));        //指定远程通信ip
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);   //指定远程端口

    while(1)
    {//一发两收模式
        printf(">");
        fgets(buf,1024,stdin);
        buf[strlen(buf)-1] = '\0';
        if(strcmp(buf,"quit")==0){
            sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *)&server_addr,addrlen);
            break;                //quit
        }
        int n = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *)&server_addr,addrlen);

        n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&server_addr,&addrlen);  
        buf[n] = '\0';
		printf("%s\n",buf);  
       
  	    n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&server_addr,&addrlen);  
        buf[n] = '\0';
		printf("%s\n",buf);  
    }  

return 0;
}


/*
void do_cli(FILE *fp, int sockfd,struct sockaddr_in server_addr){
    while(1)
    {
        int max=sockfd;
        char buf[1024];
        int n=0;
        fd_set rset;   
        FD_ZERO(&rset);
        FD_SET(sockfd,&rset);
        FD_SET(fileno(fp),&rset);

        select(sockfd+1,&rset,NULL,NULL,NULL);

        if (FD_ISSET(sockfd, &rset)) {
            memset(buf,2,10);
            n=recvfrom(sockfd, &buf, sizeof(buf), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
            buf[n]='\0';
            printf("(remote)>");
            fputs(buf,stdout);
		}

        if (FD_ISSET(fileno(fp), &rset)) {
            fgets(buf,1000,stdin);
            sendto(sockfd, &buf, sizeof(buf), 0,(struct sockaddr*)&server_addr, sizeof(server_addr));
		}
    }
close(sockfd);
}
*/