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
		_exit(0);			/* ¸¸½ø³Ì½áÊø */

	/* ×Ó½ø³Ì1¼ÌÐø£¬ÔòÒ»¶¨²»ÊÇ½ø³Ì×éµÄ×é³¤*/

	if (setsid() < 0)			/*´´½¨»á»°£¬³ÉÎª»á»°µÄÊ×½ø³Ì£¬ÐÂµÄ½ø³Ì×éµÄ×é³¤*/
		return (-1);

	Signal(SIGHUP, SIG_IGN);
	//ºöÂÔ¹ÒÆðÐÅºÅ£¬µ±»á»°µÄÊ×½ø³Ì½áÊøÊ±£¬»á»°ÖÐËùÓÐ½ø³Ì¶¼ÊÕµ½¸ÃÐÅºÅ
	if ( (pid = Fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* ×Ó½ø³Ì1½áÊø */

	/* ×Ó½ø³Ì2£¨Ëï×Ó½ø³Ì£©¼ÌÐø£¬²»ÔÙÊÇ»á»°µÄÊ×½ø³Ì£»¼´±ãÎÞÒâÖÐ´ò¿ªÒ»¸ö¿ØÖÆÖÕ¶ËÒ²²»»á³ÉÎª¿ØÖÆ½ø³Ì*/

	daemon_proc = 1;			/* ´íÎóÐÅÏ¢²»´Ó±ê×¼´íÎóÊä³ö£¬¶øÊÇÍ¨¹ýsyslogÊä³ö¡£¼ûerro.c */

	chdir("/");				/*ÐÞ¸Äµ±Ç°¹¤×÷Â·¾¶Îª¸ùÄ¿Â¼*/
	umask(0);	//Çå¿ÕÎÄ¼þ·½Ê½´´½¨ÆÁ±Î×Ö
	/* ¹Ø±ÕÎÄ¼þÃèÊö×Ö */
	for (i = 0; i < MAXFD; i++)
		close(i);

	/* ÖØ¶¨Ïò stdin, stdout, and stderr µ½ /dev/null Ø*/
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	openlog(pname, LOG_PID, facility);//Ê¹ÓÃsyslog´¦Àí´íÎó

	return (0);				/* success */
}
