//���Բ�λseqֵ
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
		msqid=msgget(IPC_PRIVATE,IPC_CREAT);//����һ����Ϣ����
		printf("msqid=%d \n",msqid);
		msgctl(msqid,IPC_RMID,msq);//ɾ���ö���
	}
exit(0);

}