	#include <stdlio.h>
pro_server(rfd， wfd)
	int rfd；
  int wfd； 
 {
 	int n；
  char buff[1024]， errbuff[64]；
  int n， fd；
	/* 从 IPC 描述符中读文件名 */
  if ((n=read(rfd，buff，1024))<=0)
  	err_sys("server：file read has error")；
	buff[n]='\0'；
  if ((fd=open(buff，0))<0)
	{ 
		strcpy(errbuf，"file can't open.\n")；
    strcat(buff，errbuff)；
    n=strlen(buff)；
    if(write(wfd，buff，n)!=n)
    	err_sys("server：message send error")
   }
	else {
		while ((n=read(fd，buff，1024))>0)
    	if(wirte(wfd，buff，n)!=n)
      	err_sys("server：message send error")；
       if (n<0)
       err_sys("server：read error")；
       }
}