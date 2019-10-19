#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h> 
main(int argc，char **argv)
{
	int sock_fd，new_fd ;
	struct sockaddr_un my_addr1，my_addr2;
	if (argc!=2)
	{
		fprintf(stderr，"usage：UNIX bind pathname\n");
		exit(1);
	}
	if (sock_fd = socket(AF_UNIX， SOCK_STREAM， 0)) == -1)
		 {		
   		/* 输出错误提示并退出*/
  		 perror("socket");
  		 exit(1);
 			}
/*将要绑定的套接字地址对应的文件删除*/
	unlink(argv[1]);
	bzero(&(my_addr1)， sizeof(my_addr1));
	my_addr1.sun_family = AF_UNIX;
	strncpy(my_addr1.sun_path，argv[1]，sizeof(my_addr1.sun_path)-1)；
	if (bind(sock_fd， (struct sockaddr *)&my_addr1，sizeof(struct sockaddr)) < 0

	{
   /* 如果调用bind()失败，则给出错误提示，退出*/
   perror(“bind error”);
   exit(1);	
	}
	getsockname(sock_fd，(struct sockaddr *)&my_addr2，&sizeof(my_addr2));
	printf("binding to pathname：%s\n"， my_addr2.sun_path， &sizeof(my_addr2));
} 