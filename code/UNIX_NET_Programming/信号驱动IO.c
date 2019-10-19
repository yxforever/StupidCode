char buf[256];
int temperature1[4];  
int temperature2[4];  
int nOK[2];
struct sockaddr_in addr; 
int addrlen;
int sockfd;
void sigio_handler(int sig)
{
	int temp[4];
	int j;
	n=recvfrom(sockfd��buf��256��0��(struct sockaddr*)&addr1��&addrlen);
	if(n<=0  && errno!=EINTR)
	exit(2);      
	if(nOK[0]==1)		/* Ѱ����Ч�����ݻ����� */
	for(j=0;j<4;j++)
		temp[j]=htonl(temperature1[j]);
  	else if(nOK[1]==1)
    		for(j=0;j<4;j++)
			temp[j]=htonl(temperature2[j]);
 	 else
   		 for(j=0;j<4;j++)				
	/* û����Ч���ݣ����ء������ݡ���Ϣ */
			temp[j]=htonl(-275);
	sendto(sockfd��temp��4*sizeof(int)��0��(struct sockaddr*)&addr��sizeof(addr));
}
int main()
{
  struct sigaction sigact;
  int i;
  sigact.sa_handler=sigio_handler;
  sigact.sa_mask=0;
  sigact.sa_flags=0;
  sigaction(SIGIO��&sigact��NULL);
  nOK[0]=0;nOK[1]=0;
for(int i=0;i<4;i++)
{
    temperature1[i]=-275;			
	/* ��Ϊ�Ƿ����ݣ�-275����ʾ�������� */
    temperature2[i]=-275;
  }
  
	sockfd=socket(AF_INET��SOCK_DGRAM��0);
	if(sockfd<0) 	exit(1);
	bzero(&addr1��sizeof(addr));
	addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=htonl(INADDR_ANY);
	addr.sin_port=htons(SERVER_PORT);
	if(bind(sockfd��(struct sockaddr*)&addr��sizeof(addr))<0)
 	exit(1);
	fcntl(sockfd��F_SETOWN��getpid());	
	/* �����׽���������Ϊ��ǰ���� */
	i=1;
  ioctl(sockfd��FIOASYNC��&i); 	
	/* �����ź�����ģʽ */
	int t[4];
	for(;;)
	{
	  GetAllTemp(t����);			
		/* ȡ�ֳ����� */
   	nOK[0]=0;
   	for(int j=0;j<4;j++)
		temperature1[j]=t[j]; 		
		/* д�뻺����1 */
		nOK[0]=1;
  	nOK[1]=0;
		for(int j=0;j<4;j++)
		temperature2[j]=t[j];  			
		/* д�뻺����2 */
		nOK[1]=1;
 	 }
 	 close(sockfd);
	}
