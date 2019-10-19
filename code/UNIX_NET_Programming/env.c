/*本测试代码包括《网络应用程序设计》课本中以下部分：
P18 变元表，环境表 
P20 进程标识符
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
	
	//环境表的显示1 第三个参数传递信息
	int n; 
	printf(" \nenvironment parameters: \n");
	for (n=0;envp[n]!=(char *)0;n++)
		printf("%s\n",envp[n]);
	
	//环境表的显示2 外部变量传递信息
	extern char ** environ;
	printf(" \nenvironment parameters2: \n");
	for (n=0;environ[n]!=(char *)0;n++)
		printf("%s\n",environ[n]);
	
	
	//环境表的显示3 库函数传递信息
	printf(" \nenvironment parameters3: \n");
	char * str;
	if((str=getenv("SHELL"))==(char*)0)
		printf(" SHELL is not defined.\n");
	else
		printf(" SHELL is =%s\n",str);
		
	//获取进程标识符 库函数获取
	printf("\nprocess pid =%d , father process pid =%d\n",getpid(),getppid());
	//内核中的方式 printk("\n pid:%d ppid:%d",current->pid,current->parent->pid);
	return(0);

}

