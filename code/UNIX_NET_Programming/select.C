int main(int argc,char*argv[])
{
	int sockfd[NUMBER];			 
	/* NUMBER为需要建立的套接字数量 */
	struct sockaddr_in servaddr[NUMBER];
	fd_set rfds;
 	char buf[1024];
	int i;
	for(i=0;i<NUMBER;i++) 
  { 
		sockfd[i]=socket(AF_INET,SOCK_STREAM,0);
   	if(sockfd[i]<0)
			exit(1);
 	}
		  ...							
		/* 填充NUMBER个地址结构 */
 		 ...						
		/* 建立NUMBER个连接 */
	int nOK[NUMBER];
 	for(i=0;i<NUMBER;i++) {nOK[i]=0;} 
 	int nEnd=NUMBER;
 	while(nEnd!=0)
	{
		for(i=0;i<NUMBER;i++) 
		if(nOK[i]==0)			
		/* 只检查未被处理的套接字 */
		FD_SET(sockfd[i],&rds);
		
		n=select(theMax(NUMBER,sockfd)+1,&rds,NULL,NULL,NULL);
		
		if(n<0 && errno==EINTR)
		continue;
		
		for(i=0;i<NUMBER;i++) 
		{
      			if( FD_ISSET(sockfd[i],&rds) )
     			 {
        			n=read(sockfd1,buf,1024);
        			if(n<=0  && errno!=EINTR)
				{
	     				perror("An Error.");		
					/* 发生错误,这个套接字不再工作 */
	          			nOK[i]=1;nEnd--;
       	 			}
       	 		
	    			else if(n>0)
	    			{
					process(buf,...);			/* 数据处理 */
	      				nOK[i]=1;nEnd--;
	    			}
      			}	
    	}
  }
  for(i=0;i<NUMBER; i++) {close(sockfd[i]);} 
}
