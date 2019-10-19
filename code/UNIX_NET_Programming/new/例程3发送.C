#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h> 
#define FIFO_SERVER "/tmp/fifos_server"
main(int argc，char** argv) 		
/* 参数为即将写入的字节数 */
{
	int fd，n;
	char w_buf[512];
	int write_num;
	n=100;
	memset(w_buf，0，512);
	strncpy(w_buf，argv[1]，n);
	if ((mkfifo(FIFO_SERVER，O_CREAT|O_EXCL)<0)&&(errno!=EEXIST))
		printf("can’t create fifos_server\n");
		/* 设置非阻塞标志 */
	fd=open(FIFO_SERVER，	O_WRONLY|O_NONBLOCK，0);
	write_num = write(fd，w_buf，n);
	if(write_wnum == -1)
	{
		if(errno == EAGAIN)
		printf("write to fifo error; try later\n");
	}
	else 
	printf("real write num is %d\n"， write_num);
}