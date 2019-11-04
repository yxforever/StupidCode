#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFLEN 1024
#define SERVER_PORT 12346
int main(int argc, char *argv[])
{
    int s;    /*服务器套接字文件描述符*/
    struct sockaddr_in server;    /*本地地址*/
    char buff[BUFFLEN];
    int n = 0;    
    
    /*建立TCP套接字*/
    s = socket(AF_INET, SOCK_STREAM, 0);
    
    /*初始化地址*/
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;/*AF_INET协议族*/
    server.sin_addr.s_addr = htonl(INADDR_ANY);/*任意本地地址*/
    server.sin_port = htons(SERVER_PORT);/*服务器端口*/    
    
    /*连接服务器*/
    int err = connect(s, (struct sockaddr*)&server,sizeof(server));
    memset(buff, 0, BUFFLEN);
    strcpy(buff, "TIME");
    /*发送数据*/
    send(s, buff, sizeof(buff), 0);
    memset(buff, 0, BUFFLEN);
    /*接收数据*/    
    n = recv(s, buff, sizeof(buff), 0);
    //if(n >0){
        printf("TIME:%s",buff);    
    //}
    sleep(10);
    close(s);
    
    return 0;        
}
