main()
{
	char parstr[]={"the datas from parent.\n"}；
	char chistr[]={"the datas from child.\n"}；
	int chpipe1[2]， chpipe2[2]；
  char buff[100]； 
	if (pipe(chpipe1)<0)
  	err_sys("pipe1 creat error")；
  if (pipe(chpipe2)<0)
           err_sys("pipe2 creat error")；
  if (fork())
  {
	  /*在父进程中 */
	  close(chpipe1[0])；
	  close(chpipe2[1])；
		if (write(chpipe1[1]，parstr， sizeof(parstr))!=sizeof(parstr))
	  	err_sys ("send error")；
	  close (chpipe1[1])；
	  if (read(chpipe2[0]，buff，100)<=0)
	  	err_sys("receive error")；
	  else
	  	printf("parent process：%s\n"，buff)；
  } /* fork */
	else { 
		close(chpipe1[1])；      /* 在子进程中 */
    close(chpipe2[0])；
    if (read(chpipe1[0]，buff，100)<=0)
    	err_sys("receive error")；
    else
    	printf("child process：%s\n"，buff)；
    if (write(chpipe2[1]，chistr，sizeof(chistr))!=sizeof(chistr))
    	err_sys("send error")；
    close (chpipe2[1])；
    }
} 