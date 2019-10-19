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
		_exit(0);			/* �����̽��� */

	/* �ӽ���1��������һ�����ǽ�������鳤*/

	if (setsid() < 0)			/*�����Ự����Ϊ�Ự���׽��̣��µĽ�������鳤*/
		return (-1);

	Signal(SIGHUP, SIG_IGN);
	//���Թ����źţ����Ự���׽��̽���ʱ���Ự�����н��̶��յ����ź�
	if ( (pid = Fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* �ӽ���1���� */

	/* �ӽ���2�����ӽ��̣������������ǻỰ���׽��̣����������д�һ�������ն�Ҳ�����Ϊ���ƽ���*/

	daemon_proc = 1;			/* ������Ϣ���ӱ�׼�������������ͨ��syslog�������erro.c */

	chdir("/");				/*�޸ĵ�ǰ����·��Ϊ��Ŀ¼*/
	umask(0);	//����ļ���ʽ����������
	/* �ر��ļ������� */
	for (i = 0; i < MAXFD; i++)
		close(i);

	/* �ض��� stdin, stdout, and stderr �� /dev/null �*/
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	openlog(pname, LOG_PID, facility);//ʹ��syslog�������

	return (0);				/* success */
}
