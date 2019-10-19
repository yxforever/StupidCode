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
void do_sig(int num){
    while(waitpid(0,NULL,WNOHANG)>0);
}
int main(void){
    	//lfd：socket返回的文件描述符，用lfd监听并接受客户端连接
   		//cfd：客户端链接上来返回的cfd，用cfd和客户端通信
        int lfd,cfd,len,i;
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

    	//socket创建服务器返回lfd文件描述符
        lfd = socket(AF_INET,SOCK_STREAM,0);
        bzero(&serv_addr,sizeof(serv_addr));
        //ipv4
        serv_addr.sin_family = AF_INET;
        //端口 本地字节序转网络字节序 host to net short 大小端存储问题
        serv_addr.sin_port = htons(9999);
        //ip host to net long
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        //bind
        bind(lfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
        //监听是否有客户连接
        listen(lfd,128);
        printf("wait for connect...\n");
        while(1){
           client_len = sizeof(client_addr);//客户端地址长度
           //连接成功返回cfd，用cfd和客户通信
           cfd = accept(lfd,(struct sockaddr*)&client_addr,&client_len);
           printf("connect success...\n");
           //创建进程
           pid = fork();
           if(pid==0){//child
                close(lfd);//子进程用不到lfd，因为lfd是父进程用来接收客户端连接的
                while(1){

			len = read(cfd,buf,sizeof(buf));
			printf("receive: %s\n",buf);
			if(len<=0)break;
			for(i=0;i<len;++i)
		    		 buf[i] = toupper(buf[i]);
			write(cfd,buf,len);
		}
		close(cfd);//客户端断开连接 
		return 0;
      }else if(pid>0) {//parent
	   	close(cfd);//父亲进程用不到cfd，因为cfd是子进程用来和客户端通信的 
      }else{
		perror("fork fail...");
		exit(1);
	    }
	}

}
/*
————————————————
版权声明：本文为CSDN博主「酒醉梦醒」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/LiuRenyou/article/details/99642126
*/