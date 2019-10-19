int main()
		{
		  int sv[2];
		  char buf;
		  if(socketpair(AF_UNIX，SOCK_STREAM，0，sv)<0) 
  		exit(1);

/* 父、子进程分别关掉一个多余的套接字，然后进行全双工通信 */
		  if(fork() == 0)
		  {/* 子进程 */
			close(sv[0]);   
			read(sv[1]，&buf，1);
			buf=toupper(buf);
			write(sv[1]，&buf，1);
			exit(0);
		  }

else
  {/* 父进程  */
	close(sv[1]);
	sleep(1);
	write(sv[0]，"a"，1);
	read(sv[0]，&buf，1);
    printf("answer char is \s"，buf);
	exit(0);
  }
}
