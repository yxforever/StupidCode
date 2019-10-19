/* include nonb1 */
#include	"my.h"

void 
str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1, val, stdineof;
	ssize_t		n, nwritten;
	fd_set		rset, wset;
	char		to[MAXLINE], fr[MAXLINE];
	char		*toiptr, *tooptr, *friptr, *froptr;

	val = Fcntl(sockfd, F_GETFL, 0);
	Fcntl(sockfd, F_SETFL, val | O_NONBLOCK);//�׽ӿ�����Ϊ������

	val = Fcntl(STDIN_FILENO, F_GETFL, 0);
	Fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);//��׼��������Ϊ������

	val = Fcntl(STDOUT_FILENO, F_GETFL, 0);
	Fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);//��׼�������Ϊ������

	toiptr = tooptr = to;	/* initialize buffer pointers */
	friptr = froptr = fr;
	stdineof = 0;//��׼���������־

	maxfdp1 = max(max(STDIN_FILENO, STDOUT_FILENO), sockfd) + 1;
	for ( ; ; ) {
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		if (stdineof == 0 && toiptr < &to[MAXLINE])//��׼����δ�������пռ�ӱ�׼�������
			FD_SET(STDIN_FILENO, &rset);	/* ��׼����ɶ� */
		if (friptr < &fr[MAXLINE])//fr�����пռ�
			FD_SET(sockfd, &rset);			/* ���׽ӿڶ��� */
		if (tooptr != toiptr)//to���������ݿ�д
			FD_SET(sockfd, &wset);			/* �׽ӿڿ�д */
		if (froptr != friptr)//fr�����ݿ�д
			FD_SET(STDOUT_FILENO, &wset);	/* ��׼�����д */

		Select(maxfdp1, &rset, &wset, NULL, NULL);
/* end nonb1 */
/* include nonb2 */
		if (FD_ISSET(STDIN_FILENO, &rset)) {//��׼����ɶ�
			if ( (n = read(STDIN_FILENO, toiptr, &to[MAXLINE] - toiptr)) < 0) {
				if (errno != EWOULDBLOCK)
					err_sys("read error on stdin");
			} else if (n == 0) {
				stdineof = 1;			/* ��׼���������*/
				if (tooptr == toiptr)//�������Ѿ�����
					Shutdown(sockfd, SHUT_WR);/* ��ر��׽ӿڵ�д */
			} else {//������ȡ
				toiptr += n;			/*����n�Լ���������ָ��仯*/
				FD_SET(sockfd, &wset);	/* ������ѭ���ڳ���д�׽ӿڣ��Ѿ���������*/
			}
		}//��׼����ɶ�����

		if (FD_ISSET(sockfd, &rset)) {//�׽ӿڿɶ�
			if ( (n = read(sockfd, friptr, &fr[MAXLINE] - friptr)) < 0) {
				if (errno != EWOULDBLOCK)
					err_sys("read error on socket");

			} else if (n == 0) {//����������
				if (stdineof)
					return;		/*�������� */
				else
					err_quit("str_cli: server terminated prematurely");
			} else {//����������
				friptr += n;		/* �޸Ļ�����ָ��*/
				FD_SET(STDOUT_FILENO, &wset);	/* ��������д��׼���*/
			}
		}
/* end nonb2 */
/* include nonb3 */
		if (FD_ISSET(STDOUT_FILENO, &wset) && ( (n = friptr - froptr) > 0)) {//��׼�����д
			if ( (nwritten = write(STDOUT_FILENO, froptr, n)) < 0) {
				if (errno != EWOULDBLOCK)
					err_sys("write error to stdout");
			} else {
				froptr += nwritten;		/* # just written */
				if (froptr == friptr)
				froptr = friptr = fr;	/* back to beginning of buffer */
			}
		}

		if (FD_ISSET(sockfd, &wset) && ( (n = toiptr - tooptr) > 0)) {
			if ( (nwritten = write(sockfd, tooptr, n)) < 0) {
				if (errno != EWOULDBLOCK)
					err_sys("write error to socket");

			} else {
				tooptr += nwritten;	/* # just written */
				if (tooptr == toiptr) {//���ݷ����ˣ���ʼ��������
					toiptr = tooptr = to;	/* back to beginning of buffer */
					if (stdineof)//��׼����Ҳ�����ˣ��ر��׽ӿ�д
						Shutdown(sockfd, SHUT_WR);	/* send FIN */
				}
			}
		}
	}
}
/* end nonb3 */
