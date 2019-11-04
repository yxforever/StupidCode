#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<strings.h>
#include<ctype.h>
#include<stdlib.h>
#include<signal.h>
#include<wait.h>
#include <time.h>

void do_sig(int num){
    while(waitpid(0,NULL,WNOHANG)>0);
}
int main(void){
    	//lfd：socket返回的文件描述符，用lfd监听并接受客户端连接
   		//cfd：客户端链接上来返回的cfd，用cfd和客户端通信
        int lfd,cfd,len,i,n=0,count=0;
        time_t now;
        char buf[1024];//缓冲区
        //serv_addr服务器地址
        //client_addr客户端地址
        struct sockaddr_in serv_addr,client_addr;
        socklen_t client_len;
        //进程id
        pid_t pid;
        //杀死僵尸进程 
    	struct sigaction act;
    	act.sa_handler = do_sig;
   	 	act.sa_flags = 0;
    	sigemptyset(&act.sa_mask);
    	sigaction(SIGCHLD,&act,NULL);

        lfd = socket(AF_INET,SOCK_STREAM,0);      //socket创建服务器返回lfd文件描述符
        bzero(&serv_addr,sizeof(serv_addr));    
        serv_addr.sin_family = AF_INET;         //ipv4
        serv_addr.sin_port = htons(9999);       //端口 本地字节序转网络字节序 host to net short 大小端存储问题
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);      //ip host to net long
        //bind
        bind(lfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
        //监听是否有客户连接
        listen(lfd,128);
        printf("等待客户端连接:\n");
        while(1){
           client_len = sizeof(client_addr);//客户端地址长度
           //连接成功返回cfd，用cfd和客户通信
           cfd = accept(lfd,(struct sockaddr*)&client_addr,&client_len);
           printf("客户端%d已连接\n",count++);
           //创建进程
           pid = fork();
           if(pid==0){//child
                close(lfd);//子进程用不到lfd，因为lfd是父进程用来接收客户端连接的
                while(1){
                    memset(buf, 0, sizeof(buf));
                    n = recv(cfd, buf, sizeof(buf),0);/*接收发送方数据*/
                    if(n > 0 && !strncmp(buf, "TIME", 4))/*判断是否合法接收数据*/
                    {   
                        memset(buf, 0, sizeof(buf));
                        now = time(NULL);
                        sprintf(buf, "%12s\r\n",ctime(&now));    
                        send(cfd, buf, sizeof(buf),0);/*发送数据*/
                        close(cfd);//客户端断开连接 
                        //sleep(10);//此处沉睡10秒
                    }
		        }
		        close(cfd);//客户端断开连接
                //printf("客户端%d已断开",count); 
		        return 0;
            }
      else if(pid>0) {//parent
	   	    close(cfd);//父亲进程用不到cfd，因为cfd是子进程用来和客户端通信的 
        }
      else{
		    perror("fork fail...");
		exit(1);
	    }
	}
}