#include <��>
#define  ADDR_PROVIDER "192.168.0.1"
#define  ADDR_TRASLATOR "192.168.0.2" 
#define  PORT1 8081
#define  PORT2 8082
/* GetAPaper()�������ADDR_PROVIDER��PORT1��Ҫһƪ���ģ�������Ŀ��pTitle�У����ĳ���(�ֽ���)����*pLen�з��ء������ĳ����޷�����ȷ�������Բ���˫ָ���������*ppData����ʵ����Ҫ����ռ䣬��*ppDataָ�����������׵�ַ��*/
int GetAPaper(char* pTitle��int* pLen��char** ppData)
{
	int sockfd;
	struct sockaddr_in servaddr;
	sockfd=socket(AF_INET��SOCK_STREAM��0);
  	if(sockfd<0)
    		exit(1);
  	bzero(&servaddr��sizeof(servaddr));
	servaddr.sin_family=AF_INET;
  	servaddr.sin_port=htons(PORT1);
 	if(inet_aton(ADDR_PROVIDER��&servaddr.sin_addr)==0)
  		exit(1);
  	if(connect(sockfd��(struct sockaddr*)&servaddr��sizeof(servaddr))<0)
		{ close(sockfd); exit(1);}
	char tx[100];
  	char* str="Get_A_Paper";
  	int nCmd=htonl(sizeof(str));				
	/* ָ���ֳ��� */
  	char* pCh=(char*)&nCmd;
  	tx[0]=0xee;tx[1]=0xee;tx[2]=0xee;tx[3]=0xee;	
	/* ���ͬ���� */
 	 tx[4]=*pCh;pCh++;						
	/* ��ָ���ֳ�����ӵ����ݰ��� */ 
	tx[5]=*pCh;pCh++;
  	tx[6]=*pCh;pCh++;
  	tx[7]=*pCh;pCh++;
  
  	int i;
  	pCh=str;						
 	/* ��ָ����������ӵ����ݰ��� */
  	for(i=8;i<8+nCmd;i++)
	{tx[i]=*pCh; pCh++;}
	int nDat=htonl(pTitle);		
	/* ����(�˴��Ǳ���)���� */
	pCh=(char*)&nDat;
	for(int j=0;j<4;j++)
		{tx[i]=*pCh;pCh++;i++;}	
		/* �����ݳ�����ӵ����ݰ��� */
		pCh=pTitle;					
		/* ��ָ����������ӵ����ݰ��� */
		 for(int k=0;k<nDat;k++)
			{ tx[i]=*pCh;pCh++;i++;}
	int nbytes=write(sockfd��tx��i);	
	/* �����ݰ����ͳ�ȥ */
 	if(nbytes<=0)
	{ close(sockfd); exit(1);}
  	char* pPaper=new int[65536];		/* ���ý��ջ��������ں����˳�ǰ����ɾ�� */
  	nbytes=read(sockfd��(char*)pPaper��65536);
  	if(nbytes<=0) 
  	{ close(sockfd); delete pPaper; return -1;}
	int nNum=0;
  	��.		/* �����յ������ݰ�����ȡ���е����ݳ��ȷ���nNum������������ʼ�±긳i */
  	*ppData=new char[nNum];	/* �����ݷ���*ppData��ʼ�Ŀռ�����ݳ��ȷ���*pLen�� */
  	char* pData=*ppData;
  	for(;i<nNum;i++)
	{ *pData=pPaper[i]; pData++;}
  	*pLen=nNum;close(sockfd);delete pPaper
  	return 0; 
}
