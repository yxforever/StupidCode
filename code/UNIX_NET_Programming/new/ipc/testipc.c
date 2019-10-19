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
	key_t key;
	for(j=0;j<10;j++)
	{
		key=ftok("1.txt",j);
		
		msqid=msgget(key,IPC_CREAT);//创建一个消息队列
		printf("msqid=%d key:%d\n",msqid,(int )key);
		//msgctl(msqid,IPC_RMID,msq);//删除该队列
}
exit(0);

}