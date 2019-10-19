//#include	"unp.h"
#include <stdio.h>
#include <sys/utsname.h>

int 
main(int argc, char **argv)
{
	union {//�����干���ڴ�ռ�
		short  s;
		char   c[sizeof(short)];
    		} un;
    	union {
		int  s;
		char   c[sizeof(short)];
    	} un1;
 	struct utsname utsn;
 	uname(&utsn);
	un.s = 0x0102;//01�Ǹ����ֽڣ�02�ǵ����ֽ�
	printf("%s::%s::%s::%s::%s:: ", utsn.sysname,utsn.nodename,utsn.release,utsn.version,utsn.machine);
	if (sizeof(short) == 2) {
		if (un.c[0] == 1 && un.c[1] == 2)
			printf("big-endian\n");
		else if (un.c[0] == 2 && un.c[1] == 1)
			printf("little-endian\n");
		else
			printf("unknown\n");
	} else
		printf("sizeof(short) = %d\n", sizeof(short));

	un1.s=0x01020304;
	printf("%d %d %d %d",un1.c[0],un1.c[1],un1.c[2],un1.c[3]);
	
	exit(0);
}
