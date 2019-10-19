#include	"my.h"
#include	<syslog.h>

#define	MAXFD	64

extern int	daemon_proc;	/* defined in error.c */

int 
daemon_init(const char *pname, int facility)
{
	int		i;
	pid_t	pid;

	if ( (pid = Fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* 父进程结束 */

	/* 子进程1继续，则一定不是进程组的组长*/

	if (setsid() < 0)			/*创建会话，成为会话的首进程，新的进程组的组长*/
		return (-1);

	Signal(SIGHUP, SIG_IGN);
	//忽略挂起信号，当会话的首进程结束时，会话中所有进程都收到该信号
	if ( (pid = Fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* 子进程1结束 */

	/* 子进程2（孙子进程）继续，不再是会话的首进程；即便无意中打开一个控制终端也不会成为控制进程*/

	daemon_proc = 1;			/* 错误信息不从标准错误输出，而是通过syslog输出。见erro.c */

	chdir("/");				/*修改当前工作路径为根目录*/
	umask(0);	//清空文件方式创建屏蔽字
	/* 关闭文件描述字 */
	for (i = 0; i < MAXFD; i++)
		close(i);

	/* 重定向 stdin, stdout, and stderr 到 /dev/null �*/
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	openlog(pname, LOG_PID, facility);//使用syslog处理错误

	return (0);				/* success */
}
