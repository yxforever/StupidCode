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

char hello[]="欢迎来到游戏大厅！\n你可以选择问题编号进行作答，正确解答后会有相应得分哦~";
char error[]="你输入的指令有误，请重新输入~";
char tip[]="提示：play";
char getname[]="输入你的id开始游戏吧！！";
char congratitions[]="恭喜你进入第二关！";
char commond[]="1:任务1(1积分)\n2:任务2(2积分)\n3:任务3(3积分)\n4.查询积分\n5.进入下一关";
char commond2[]="1:任务4(1积分)\n2:任务5(2积分)\n3:任务6(3积分)\n4.查询积分";
char request[]="请输入你选择的问题编号:";
char dennied[]="你当前积分不足哦，继续答题取得权限。";
char task1[]="5*5=?";//25
char task2[]="3+5+8+1+20+11+22=?";//70
char task3[]="3*4*5*6=?";//360
char task4[]="2^4=?";//16
char task5[]="4*12+7*2=?";//62
char task6[]="3^4=?";//81
char answer[]="请输入你的答案：";
char wrong[]="答案错误,再试一次:";
char right[]="答案正确,请选择另一问题：";
char swi[]="你已跳过选中的任务,可重新选取任务：";
int score[10]={0};
int handle(struct sockaddr_in client_addr,char *buf);

int main(int argc,char* argv[]){
    int sockfd;
    int pid;
    char buf[1024];

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int addrlen = sizeof(struct sockaddr);  

    sockfd = socket(AF_INET,SOCK_DGRAM,0);  
    if(sockfd < 0){  
        perror("socket");  
    }
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)    perror("bind");

    while(1)  
    {  
        int n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&addrlen);  
		if(n < 0){
            perror("recvfrom ");
        }
        buf[n] = '\0';

        if(!strcmp(buf,"play"))
        {
            pid = fork();
            if(pid < 0)
            {  
                perror("Fail to fork");  
                exit(EXIT_FAILURE);  
            }  
            if(pid == 0){
                close(sockfd);  
                handle(client_addr,buf);
                exit(EXIT_SUCCESS);  
            }
		}
		else{
        sendto(sockfd,error,strlen(error),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
        sendto(sockfd,tip,strlen(tip),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
        continue;
          }
    }
}

int handle(struct sockaddr_in client_addr,char *buf)  //client_addr 客户端ip信息
{   
    time_t t;
    int id;
    int ans;
    int cmd=0;
    int cmd2=0;
    int flag;
    int addrlen = sizeof(struct sockaddr);
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);  
    if(sockfd < 0){  
        perror("socket");  
    }
    sendto(sockfd,hello,strlen(hello),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
	sendto(sockfd,getname,strlen(getname),0,(struct sockaddr *)&client_addr,sizeof(client_addr));  
	int n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&addrlen);
    if(n < 0)   perror("recvfrom");  
    buf[n] = '\0';
    id=atoi(buf);
    printf("用户%s进入游戏大厅\n",buf);
    sendto(sockfd,request,strlen(request),0,(struct sockaddr *)&client_addr,sizeof(client_addr));


    while(1){//结构为一收两发

        int cut[]={1}; //关卡，初始为1
        if(flag==1){
            sendto(sockfd,swi,strlen(swi),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
        }
        sendto(sockfd,commond,strlen(commond),0,(struct sockaddr *)&client_addr,sizeof(client_addr));//向用户发送问题列表

        n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&addrlen);//接收指令
        if(n < 0)
            perror("recvfrom");
        buf[n] = '\0';
        if(!strcmp(buf,"quit")){
            printf("用户%d离开游戏大厅\n",id);
            break;
        }
		cmd=atoi(buf);
        flag=0;
        //两发一收模式
        if(cmd==1){
            sendto(sockfd,task1,strlen(task1),0,(struct sockaddr *)&client_addr,sizeof(client_addr));   //发送所请求的任务
            sendto(sockfd,answer,strlen(answer),0,(struct sockaddr *)&client_addr,sizeof(client_addr)); //get用户答案
            memset(buf,0,sizeof(buf));
			   n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&addrlen);
               buf[n] = '\0';
               if(!strcmp(buf,"switch")){
                   flag=1;
                   continue;
               }
			   ans=atoi(buf);
               //判断用户答案
			   if(ans==25){
			    sendto(sockfd,right,strlen(right),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
                score[id]+=1;
                printf("用户%d完成任务1:+1分，总：%d\n",id,score[id]);
			   }
               else{
				 sendto(sockfd,wrong,strlen(wrong),0,(struct sockaddr *)&client_addr,sizeof(client_addr));  
			  }
        }

        else if(cmd==2){
            sendto(sockfd,task2,strlen(task2),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
            sendto(sockfd,answer,strlen(answer),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
            memset(buf,0,sizeof(buf));
			   n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&addrlen);
               buf[n] = '\0';
               if(!strcmp(buf,"switch")){
                   flag=1;
                   continue;
               }
			   ans=atoi(buf);
			   if(ans==70){
			    sendto(sockfd,right,strlen(right),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
                score[id]+=2;
                printf("用户%d完成任务2:+2分，总：%d\n",id,score[id]);
			   }
               else{
				 sendto(sockfd,wrong,strlen(wrong),0,(struct sockaddr *)&client_addr,sizeof(client_addr));  
			  }
        }

        else if(cmd==3){
            sendto(sockfd,task3,strlen(task3),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
               sendto(sockfd,answer,strlen(answer),0,(struct sockaddr *)&client_addr,sizeof(client_addr));  
            memset(buf,0,sizeof(buf));
			   n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&addrlen);
               buf[n] = '\0';
               if(!strcmp(buf,"switch")){
                   flag=1;
                   continue;
               }
			   ans=atoi(buf);
			   if(ans==360){
			    sendto(sockfd,right,strlen(right),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
                score[id]+=3;
                printf("用户%d完成任务3:+3分，总：%d\n",id,score[id]);
			   }
               else{
				 sendto(sockfd,wrong,strlen(wrong),0,(struct sockaddr *)&client_addr,sizeof(client_addr));  
			  }
        }
        
        else if(cmd==4){
        sprintf(buf,"%d",score[id]);
			int len=strlen(buf);		
		    buf[len]='\0';
            sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
        }

        else if(cmd==5){    //用户请求进入下一关，由服务器判断积分是否达到6
        if(score[id]<6)
        {
            sendto(sockfd,dennied,strlen(dennied),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
            continue;
        }

        else {
            sendto(sockfd,congratitions,strlen(congratitions),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
            while(1){
                if(flag==1){
            sendto(sockfd,swi,strlen(swi),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
        }
            sendto(sockfd,commond2,strlen(commond2),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
            n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&addrlen);  
        if(n < 0)
            perror("recvfrom");
        buf[n] = '\0';
        if(!strcmp(buf,"quit")){
            printf("用户%d离开游戏大厅\n",id);
            break;
        }
        flag=0;
		cmd2=atoi(buf);
        if(cmd2==1){
            sendto(sockfd,task4,strlen(task4),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
            sendto(sockfd,answer,strlen(answer),0,(struct sockaddr *)&client_addr,sizeof(client_addr));  
            memset(buf,0,sizeof(buf));
			   n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&addrlen);
               buf[n] = '\0';
               if(!strcmp(buf,"switch")){
                   flag=1;
                   continue;
               }
			   ans=atoi(buf);
			   if(ans==16){
			    sendto(sockfd,right,strlen(right),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
                score[id]+=1;
                printf("用户%d完成任务1:+1分，总：%d\n",id,score[id]);
			   }
               else{
				 sendto(sockfd,wrong,strlen(wrong),0,(struct sockaddr *)&client_addr,sizeof(client_addr));  
			  }
        }
        else if(cmd2==2){
            sendto(sockfd,task5,strlen(task5),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
            sendto(sockfd,answer,strlen(answer),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
            memset(buf,0,sizeof(buf));
			   n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&addrlen);
               buf[n] = '\0';
               if(!strcmp(buf,"switch")){
                   flag=1;
                   continue;
               }
			   ans=atoi(buf);
			   if(ans==62){
			    sendto(sockfd,right,strlen(right),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
                score[id]+=2;
                printf("用户%d完成任务2:+2分，总：%d\n",id,score[id]);
			   }
               else{
				 sendto(sockfd,wrong,strlen(wrong),0,(struct sockaddr *)&client_addr,sizeof(client_addr));  
			  }
        }
        else if(cmd2==3){
            sendto(sockfd,task6,strlen(task6),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
            sendto(sockfd,answer,strlen(answer),0,(struct sockaddr *)&client_addr,sizeof(client_addr));  
            memset(buf,0,sizeof(buf));
			   n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&addrlen);
               buf[n] = '\0';
               if(!strcmp(buf,"switch")){
                   flag=1;
                   continue;
               }
			   ans=atoi(buf);
			   if(ans==81){
			    sendto(sockfd,right,strlen(right),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
                score[id]+=3;
                printf("用户%d完成任务6:+3分，总：%d\n",id,score[id]);
			   }
               else{
				 sendto(sockfd,wrong,strlen(wrong),0,(struct sockaddr *)&client_addr,sizeof(client_addr));  
			  }
        }
           else if(cmd2==4){
        sprintf(buf,"%d",score[id]);
			int len=strlen(buf);		
		    buf[len]='\0';
            sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
        }
            else{
			 sendto(sockfd,wrong,strlen(wrong),0,(struct sockaddr *)&client_addr,sizeof(client_addr));  
	    }        
        }
        }

    }
    else sendto(sockfd,error,strlen(error),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
    }
    return 0;
}