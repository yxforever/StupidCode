#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/type.h> 
#include <sys/un.h>
#include <sys/socket.h>
#define BACKLOG 5
#define SOCK_PATH "/tmp/exmp_sock"
main()
{
	int len， sin_size，n;
	int sock_fd， new_fd ;	 //s2 ;
	struct sockaddr_un my_addr， remote_addr;
	char buf[512];

if (sock_fd = socket(AF_UNIX， SOCK_STREAM， 0)) == -1)
{
   /* 输出错误提示并退出*/
	perror("socket error");
	exit(1);
 	}
	my_addr.sun_family = AF_UNIX;
	strcpy(my_addr.sun_path，SOCK_PATH);
/* 将要绑定的套接字地址对应的文件删除 */
	unlink(my_addr.sun_path);
	len = strlen(my_addr.sun_path)+sizeof(my_addr.sun_family);
	if (bind(sock_fd， (struct sockaddr *)&my_addr，len) < 0)
		{
  		 /* 如果调用bind()失败，则给出错误提示，退出*/
  		perror("bind error");
   		exit(1);
		}
	if(listen(sockf_d，BACKLOG) ==-1)
    {
      perror("listen");
      exit(1);
    }
	while (1)
	{
		sin_size = sizeof(struct sockaddr_un);
		if ((new_fd = accept(sock_fd，(struct sockaddr*)&remote_addr， &sin_size))==-1)
		{
			perror("accept");
      continue;
     }
     do {
					n =recv(new_fd，buf，512，0);
					if ( n<0)
						{  perror("recv error");
   						 done = 1;
						}
					if (n == 0)
   					done = 1;
					if (!done )
   					if (send(new_fd，buf，n ，0)<0)
  						{  	perror("recv error");
   								done = 1;
   								}
				} while (!done);
	close(new_fd);
} //while
 return 0;
}