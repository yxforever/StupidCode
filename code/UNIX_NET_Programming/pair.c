#define DATA1 "test string 1"
#define DATA2 "test string 2"
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<netinet/in.h>
#include <errno.h>
main()
{
  int sockets[2], child;
  char buf[1024];
  /* Get the socket pair */
  if (socketpair(AF_UNIX, SOCK_DGRAM,0, sockets) < 0) {
    printf("error %d on socketpair ", errno);
    exit(1);
  }
  /* create child process */
if ((child = fork()) == -1) {
    printf("fork error %d ", errno);
    exit(1);
  }
  if (child != 0) { /* this is the parent */
    /* close child's end of socket */
    close(sockets[0]);
    /* read message from child */
    if (read(sockets[1], buf, sizeof(buf)) < 0) {
      printf("error %d reading socket ", errno);
      exit(1);
    }
    printf("parent:-->%s \n", buf);
    /* write message to child */
    if (write(sockets[1], DATA1, sizeof(DATA1)) < 0) {
      printf("error %d writing socket ", errno);
      exit(1);
    }
    /* finished */
    close(sockets[1]);
 } else { /* the child */
    /* close parent's end of socket */
    close(sockets[1]);
    /* send message to parent */
    if (write(sockets[0], DATA2, sizeof(DATA2)) < 0) {
      printf("error %d writing socket ", errno);
      exit(1);
    }
    /* get message from parent */
    if (read(sockets[0], buf, sizeof(buf)) < 0) {
      printf("error %d reading socket ", errno);
      exit(1);
    }
    printf("child: -->%s\n ", buf);
    /* finished */
    close(sockets[0]);
  }



}
