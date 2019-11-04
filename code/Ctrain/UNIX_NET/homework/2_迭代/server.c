#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#define BUFFLEN 1024
#define SERVER_PORT 12346
#define BACKLOG 5
int main(int argc, char *argv[])
{
    printf("等待客户端连接:\n");
    int s_s, s_c;    /*服务器套接字文件描述符*/
    struct sockaddr_in local, from;    /*本地地址*/
    time_t now;        
    char buff[BUFFLEN];
    int n = 0;
    int len = sizeof(from);
    
    /*建立TCP套接字*/
    s_s = socket(AF_INET, SOCK_STREAM, 0);
    
    /*初始化地址*/
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;/*AF_INET协议族*/
    local.sin_addr.s_addr = htonl(INADDR_ANY);/*任意本地地址*/
    local.sin_port = htons(SERVER_PORT);/*服务器端口*/
    
    /*将套接字文件描述符绑定到本地地址和端口*/
    int err = bind(s_s, (struct sockaddr*)&local, sizeof(local));
    err = listen(s_s, BACKLOG);/*侦听*/
    
    /*主处理过程*/
    while(1)
    {
        /*接收客户端连接*/
        s_c = accept(s_s, (struct sockaddr*)&from, &len);
        memset(buff, 0, BUFFLEN);
        n = recv(s_c, buff, BUFFLEN,0);/*接收发送方数据*/
        if(n > 0 && !strncmp(buff, "TIME", 4))/*判断是否合法接收数据*/
        {   printf("have one\n");
            memset(buff, 0, BUFFLEN);
            now = time(NULL);
            sprintf(buff, "%12s\r\n",ctime(&now));    
            send(s_c, buff, 1024,0);/*发送数据*/
            sleep(10);//此处沉睡10秒
        }
        close(s_c);
    }
    close(s_s);
    
    return 0;        
}
