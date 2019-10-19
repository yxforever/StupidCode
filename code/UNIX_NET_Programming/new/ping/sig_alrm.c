#include	"ping.h"

void
sig_alrm(int signo)
{ 
	(*pr->fsend)();//每隔一秒发送一次数据

	alarm(1);
	return;
}
