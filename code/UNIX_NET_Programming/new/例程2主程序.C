main()
{
	int chpipe1[2]， chpipe2[2]；
　int pid；
  if (pipe(chpipe1)<0) 
  	err_sys("pipe1 creat error")；
  if (pipe(chpipe2)<0)
  	err_sys("pipe2 creat error")；
  if ((pid=fork())<0)
  	{ err_sys("can't fork")；}
	else if (pid)
{
	/* 在父进程中 */
  close(chpipe1[0])；
  close(chpipe2[1])；
  pro_client(chpipe2[0]，chpipe1[1])；
	while (wait()!=pid)；
  /* 等待原子进程运行返回 */
  close(chpipe1[1])；
  close(chpipe2[0])；
 }
 else { /* 在子进程中 */
 	close(chpipe1[1])；
  close(chpipe2[0])；
	pro_server(chpipe1[0]，chpipe2[1])；
  close(chpipe1[0])；
  close(chpipe2[1])；
  }
 	exit(0)；
 }