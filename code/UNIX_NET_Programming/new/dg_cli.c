#include	"my.h"
void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{ 
	int	n,j;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];
	while (Fgets(sendline, MAXLINE, fp)!= NULL) {
		/*Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
			n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
			recvline[n] = 0;	//null terminate 
			Fputs(recvline, stdout);*/
			
		for(j=0;j<4;j++)//�ź�����IO��ʹ��
			{
				Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
			}
			sleep(1);//cntread[0]���Գ��ַ�0,�������IO�źš�for����IO�źſ�����IO������ǰȫ������ֻ������һ�δ�������
			Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
		for(j=0;j<5;j++)//�ź�����IO��ʹ��
		{
			n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
			recvline[n] = 0;	/* null terminate */
			Fputs(recvline, stdout);
		}
	}
}
