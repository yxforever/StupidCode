#include <stdlio.h>
pro_client(rfd�� wfd)
	int rfd��
	int wfd��
{ 
      char parstr[]={"parfile.dat"}��
      int n��
      char buff[100]��
      n=strlen(parstr)��
      if (write(wfd��parstr��n)!=n)
      	err_sys("client�� send error")��
      if (read(rfd��buff��100)<=0)
      	err_sys("client�� receive  error")��
      else
       printf("parent process��%s\n"��buff)��
}