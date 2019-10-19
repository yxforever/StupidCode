//测试槽位seq值
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
 
int main(int argc,char **argv)
{
	int j,msqid;
	struct msqid_ds * msq;
	msq=NULL;
	for(j=0;j<10000;j++)
	{
		msqid=msgget(IPC_PRIVATE,IPC_CREAT);//创建一个消息队列
		printf("msqid=%d \n",msqid);
		msgctl(msqid,IPC_RMID,msq);//删除该队列
	}
exit(0);

}