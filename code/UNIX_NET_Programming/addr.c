//��ַ����˵��:inet_addr;inet_aton;inet_ntoa;inet_ntop
#include<fcntl.h>
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
int main()
{
	char b[16]="192.168.5.10";
	char a[16]="255.255.255.255";
	char c[16]="192.168.1.259";
	char d[16]="192.168.1.14";
	long A,B,C;
	struct in_addr addr,addr2;
	struct in_addr addr3;
	printf("a:%s\nb:%s\nc:%s\nd:%s\n",a,b,c,d);
	B=inet_addr(b);//inet_addr���ص�������
	A=inet_addr(a);//�������ַ�ú������޷������,a��ַ�ǺϷ������������FFFFFFFF
	C=inet_addr(c);//�������ַ�ú������޷������,c��ַ�ǲ��Ϸ�,�������Ҳ��FFFFFFFF
	printf("inet_addr(a):%X\n",A);
	printf("inet_addr(c):%X\n",C);
	inet_aton(b,&addr);//inet_aton��д�ṹ��struct in_addr
	printf("inet_addr(b):%X\ninet_aton(b,&addr):%X\n",B,addr.s_addr);
	++addr.s_addr;//�Խṹ������ͺ����Ĳ���
	
	addr2.s_addr=addr.s_addr+5;
	printf("First time inet_ntoa(addr):%s \n",inet_ntoa(addr));
	printf("First time inet_ntoa(addr2):%s \n",inet_ntoa(addr2));
	printf("Two function together:\n1.inet_ntoa(addr):%s \n2.inet_ntoa(addr2):%s\n",inet_ntoa(addr),inet_ntoa(addr2));//֤��inet_ntoa�ǲ������뺯��	
	
	
	inet_pton(AF_INET,b,&addr3);//ʹ�õ���struct in_addr�ṹ��
	printf("inet_pton(b):%X\n",addr3.s_addr);
	
	char str[INET_ADDRSTRLEN];
	inet_aton(d,&addr3);	
	printf("inet_ntop(addr3):%s \n",inet_ntop(AF_INET,&addr3,str,sizeof(str)));
	return(0);
}

