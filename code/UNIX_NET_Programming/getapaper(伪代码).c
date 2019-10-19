#include <…>
#define  ADDR_PROVIDER "192.168.0.1"
#define  ADDR_TRASLATOR "192.168.0.2" 
#define  PORT1 8081
#define  PORT2 8082
/* GetAPaper()向服务器ADDR_PROVIDER：PORT1索要一篇论文，论文题目在pTitle中，论文长度(字节数)放在*pLen中返回。因论文长度无法事先确定，所以采用双指针参数，用*ppData根据实际需要申请空间，将*ppData指向论文内容首地址。*/
int GetAPaper(char* pTitle，int* pLen，char** ppData)
{
	int sockfd;
	struct sockaddr_in servaddr;
	sockfd=socket(AF_INET，SOCK_STREAM，0);
  	if(sockfd<0)
    		exit(1);
  	bzero(&servaddr，sizeof(servaddr));
	servaddr.sin_family=AF_INET;
  	servaddr.sin_port=htons(PORT1);
 	if(inet_aton(ADDR_PROVIDER，&servaddr.sin_addr)==0)
  		exit(1);
  	if(connect(sockfd，(struct sockaddr*)&servaddr，sizeof(servaddr))<0)
		{ close(sockfd); exit(1);}
	char tx[100];
  	char* str="Get_A_Paper";
  	int nCmd=htonl(sizeof(str));				
	/* 指令字长度 */
  	char* pCh=(char*)&nCmd;
  	tx[0]=0xee;tx[1]=0xee;tx[2]=0xee;tx[3]=0xee;	
	/* 添加同步字 */
 	 tx[4]=*pCh;pCh++;						
	/* 把指令字长度添加到数据包中 */ 
	tx[5]=*pCh;pCh++;
  	tx[6]=*pCh;pCh++;
  	tx[7]=*pCh;pCh++;
  
  	int i;
  	pCh=str;						
 	/* 把指令字内容添加到数据包中 */
  	for(i=8;i<8+nCmd;i++)
	{tx[i]=*pCh; pCh++;}
	int nDat=htonl(pTitle);		
	/* 数据(此处是标题)长度 */
	pCh=(char*)&nDat;
	for(int j=0;j<4;j++)
		{tx[i]=*pCh;pCh++;i++;}	
		/* 把数据长度添加到数据包中 */
		pCh=pTitle;					
		/* 把指令字内容添加到数据包中 */
		 for(int k=0;k<nDat;k++)
			{ tx[i]=*pCh;pCh++;i++;}
	int nbytes=write(sockfd，tx，i);	
	/* 把数据包发送出去 */
 	if(nbytes<=0)
	{ close(sockfd); exit(1);}
  	char* pPaper=new int[65536];		/* 设置接收缓存区，在函数退出前必须删除 */
  	nbytes=read(sockfd，(char*)pPaper，65536);
  	if(nbytes<=0) 
  	{ close(sockfd); delete pPaper; return -1;}
	int nNum=0;
  	….		/* 解析收到的数据包，提取其中的数据长度放入nNum，并将数据起始下标赋i */
  	*ppData=new char[nNum];	/* 把数据放入*ppData开始的空间里，数据长度放入*pLen中 */
  	char* pData=*ppData;
  	for(;i<nNum;i++)
	{ *pData=pPaper[i]; pData++;}
  	*pLen=nNum;close(sockfd);delete pPaper
  	return 0; 
}
