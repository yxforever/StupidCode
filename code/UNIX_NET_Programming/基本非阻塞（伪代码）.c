/* 头文件 */
/* 定义本地处理函数 */
int main(int x)
{
	int sockfd; 
	struct sockaddr_in servaddr;
	int nLocalFin=0;
	int nFin=0;
	int nConnected=0;
	static int nOut=0;
	static int nFirst=0;//发送标记
	int x1，x2;
	if((sockfd1=socket(AF_INET，SOCK_STREAM，0))<0)
	{
		fprintf(stderr，"Socket error");
	  	exit(1);
	}
	bzero(&servaddr，sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(SERVER_PORT);
	if(inet_aton(ADDR，&servaddr.sin_addr)==0)
  		exit(1);
	int flag=fcntl(sockfd，F_GETFL，0);	
	/* 设置非阻塞标志 */
	fcntl( sockfd，F_SETFL，flag | O_NONBLOCK );
	int n=0;
	while((nFin==0 || (nLocalFin==0))
	{
		if(n<0 && errno==EINTR)			
		/* 收到中断信号，继续运行程序 */
			continue;
		if(nFin==0)    					
		/* 读服务器的返回值 */
		{
      	if(nConnected==0)/* 未连接 */
        {
        	n=connect(sockfd，(struct sockaddr*)&servaddr，sizeof(servaddr));
         	if(n<0 && errno==EINPROGRESS)nConnect=1;				
				/* 设定正在连接标志 */
					else if(n>0)
					{
		   			nConnected=2; nOut=0;			
						/* 连接成功 */
		   			fprintf("Connected.");
		 			}
        }
        else if(nConnected==1)				
				/* 正在连接 */
				{
        	n=connect(sockfd，(struct sockaddr*)&servaddr，sizeof(servaddr));
					if(n<0 && errno!=EINPROGRESS)
		  		{
		     			nConnected=0;				
							/* 连接出错，重新连接 */
		     			nOut++;
		     			if(nOut>9) 				
							/* 连续10次都连接出错，终止程序，返回错误 */
		    			exit(1);
					}
					else if(n>=0)		/* 连接成功 */
					{
		  			nConnected=2; nOut=0;
		  			fprintf("Connected.");
					}
      	}
      	else//nConnected==2
      	{
        	if(nFirst==0) 		
					/* 建立连接后，首先向服务器发出数据 */
					{n=write(sockfd，&x，sizeof(buf)); nFirst=1;}
					n=read(sockfd，buf，128);
         	if(n==0)			
					/* 连接被断开，设置未连接标志，重新连接 */
	      	{
	        	fprintf("Socket1 is disconnected.");
	        	nConnected=0; nFirst=0;
	      	}
					else if(n<0  && errno!=EINTR && errno!=EWOULDBLOCK) /* 发生错误，退出 */
         	{
	       		perror("An Error.");			
          	exit(2);
	        }
	    		else if(n>0)
	      	{
	       		x1=… 		
						/* 从中读出服务器的返回值，赋给x1 */
						fprintf("Remote task is over.");
	        	nFin=1;//完成f1
	      	}
       	}
    }//完成远程服务器的访问
	
		if(nLocalFin==0)  
    { 
    	x2=…  	/* 本地数据处理，结果数值赋给x2 */
    	nLocalFin=1;
      fprintf("Local task is over.");
     }//完成本地的访问
  }//循环结束
      	
	close(sockfd);
	return x1+x2;
}
