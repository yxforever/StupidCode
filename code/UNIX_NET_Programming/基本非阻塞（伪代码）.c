/* ͷ�ļ� */
/* ���屾�ش����� */
int main(int x)
{
	int sockfd; 
	struct sockaddr_in servaddr;
	int nLocalFin=0;
	int nFin=0;
	int nConnected=0;
	static int nOut=0;
	static int nFirst=0;//���ͱ��
	int x1��x2;
	if((sockfd1=socket(AF_INET��SOCK_STREAM��0))<0)
	{
		fprintf(stderr��"Socket error");
	  	exit(1);
	}
	bzero(&servaddr��sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(SERVER_PORT);
	if(inet_aton(ADDR��&servaddr.sin_addr)==0)
  		exit(1);
	int flag=fcntl(sockfd��F_GETFL��0);	
	/* ���÷�������־ */
	fcntl( sockfd��F_SETFL��flag | O_NONBLOCK );
	int n=0;
	while((nFin==0 || (nLocalFin==0))
	{
		if(n<0 && errno==EINTR)			
		/* �յ��ж��źţ��������г��� */
			continue;
		if(nFin==0)    					
		/* ���������ķ���ֵ */
		{
      	if(nConnected==0)/* δ���� */
        {
        	n=connect(sockfd��(struct sockaddr*)&servaddr��sizeof(servaddr));
         	if(n<0 && errno==EINPROGRESS)nConnect=1;				
				/* �趨�������ӱ�־ */
					else if(n>0)
					{
		   			nConnected=2; nOut=0;			
						/* ���ӳɹ� */
		   			fprintf("Connected.");
		 			}
        }
        else if(nConnected==1)				
				/* �������� */
				{
        	n=connect(sockfd��(struct sockaddr*)&servaddr��sizeof(servaddr));
					if(n<0 && errno!=EINPROGRESS)
		  		{
		     			nConnected=0;				
							/* ���ӳ����������� */
		     			nOut++;
		     			if(nOut>9) 				
							/* ����10�ζ����ӳ�����ֹ���򣬷��ش��� */
		    			exit(1);
					}
					else if(n>=0)		/* ���ӳɹ� */
					{
		  			nConnected=2; nOut=0;
		  			fprintf("Connected.");
					}
      	}
      	else//nConnected==2
      	{
        	if(nFirst==0) 		
					/* �������Ӻ�������������������� */
					{n=write(sockfd��&x��sizeof(buf)); nFirst=1;}
					n=read(sockfd��buf��128);
         	if(n==0)			
					/* ���ӱ��Ͽ�������δ���ӱ�־���������� */
	      	{
	        	fprintf("Socket1 is disconnected.");
	        	nConnected=0; nFirst=0;
	      	}
					else if(n<0  && errno!=EINTR && errno!=EWOULDBLOCK) /* ���������˳� */
         	{
	       		perror("An Error.");			
          	exit(2);
	        }
	    		else if(n>0)
	      	{
	       		x1=�� 		
						/* ���ж����������ķ���ֵ������x1 */
						fprintf("Remote task is over.");
	        	nFin=1;//���f1
	      	}
       	}
    }//���Զ�̷������ķ���
	
		if(nLocalFin==0)  
    { 
    	x2=��  	/* �������ݴ��������ֵ����x2 */
    	nLocalFin=1;
      fprintf("Local task is over.");
     }//��ɱ��صķ���
  }//ѭ������
      	
	close(sockfd);
	return x1+x2;
}
