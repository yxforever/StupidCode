#include <stdlio.h>
pro_client(rfd£¬ wfd)
	int rfd£»
	int wfd£»
{ 
      char parstr[]={"parfile.dat"}£»
      int n£»
      char buff[100]£»
      n=strlen(parstr)£»
      if (write(wfd£¬parstr£¬n)!=n)
      	err_sys("client£º send error")£»
      if (read(rfd£¬buff£¬100)<=0)
      	err_sys("client£º receive  error")£»
      else
       printf("parent process£º%s\n"£¬buff)£»
}