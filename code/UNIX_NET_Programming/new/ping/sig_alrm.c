#include	"ping.h"

void
sig_alrm(int signo)
{ 
	(*pr->fsend)();//ÿ��һ�뷢��һ������

	alarm(1);
	return;
}
