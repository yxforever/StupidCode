#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#define FIFO_SERVER "/tmp/fifos_server "
main(int argc，char** argv) 
{
	char r_buf[4096*2];
	int  fd;
	int  r_size;
	int  ret_size;
	r_size=atoi(argv[1]);
	printf("input real read bytes %d\n"，r_size);
	memset(r_buf，0，sizeof(r_buf));
	fd=open(FIFO_SERVER，	O_RDONLY|O_NONBLOCK，0);
	/* fd=open(FIFO_SERVER，O_RDONLY，0); 在此	处可以把读程序编译成两个
		不同版本：阻塞版本及非阻塞版本 */
	if(fd==-1)
	{
			printf("open %s for read error\n");
			exit();	
	}

	while(1)
	{
		memset(r_buf，0，sizeof(r_buf));
		ret_size=read(fd，r_buf，r_size);
		if(ret_size==-1)
			if(errno==EAGAIN)
				printf("no data avlaible\n");
		printf("real read bytes %d\n"，ret_size);
	  sleep(1);
	 }	
	pause();
	unlink(FIFO_SERVER);//删除FIFO文件
}