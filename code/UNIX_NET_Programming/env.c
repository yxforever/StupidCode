/*�����Դ������������Ӧ�ó�����ơ��α������²��֣�
P18 ��Ԫ�������� 
P20 ���̱�ʶ��
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[],char* envp[])
{
	int i;
	printf("argc: %d \n",argc);
	for (i=0;i<argc;i++)
		printf("argv[%d]: %s \n",i,argv[i]);
	
	//���������ʾ1 ����������������Ϣ
	int n; 
	printf(" \nenvironment parameters: \n");
	for (n=0;envp[n]!=(char *)0;n++)
		printf("%s\n",envp[n]);
	
	//���������ʾ2 �ⲿ����������Ϣ
	extern char ** environ;
	printf(" \nenvironment parameters2: \n");
	for (n=0;environ[n]!=(char *)0;n++)
		printf("%s\n",environ[n]);
	
	
	//���������ʾ3 �⺯��������Ϣ
	printf(" \nenvironment parameters3: \n");
	char * str;
	if((str=getenv("SHELL"))==(char*)0)
		printf(" SHELL is not defined.\n");
	else
		printf(" SHELL is =%s\n",str);
		
	//��ȡ���̱�ʶ�� �⺯����ȡ
	printf("\nprocess pid =%d , father process pid =%d\n",getpid(),getppid());
	//�ں��еķ�ʽ printk("\n pid:%d ppid:%d",current->pid,current->parent->pid);
	return(0);

}

