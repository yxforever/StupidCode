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
	key_t key;
	for(j=0;j<10;j++)
	{
		key=ftok("1.txt",j);
		
		msqid=msgget(key,IPC_CREAT);//����һ����Ϣ����
		printf("msqid=%d key:%d\n",msqid,(int )key);
		//msgctl(msqid,IPC_RMID,msq);//ɾ���ö���
}
exit(0);

}