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
	Fcntl(sockfd, F_SETFL, val | O_NONBLOCK);//套接口设置为非阻塞

	val = Fcntl(STDIN_FILENO, F_GETFL, 0);
	Fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);//标准输入设置为非阻塞

	val = Fcntl(STDOUT_FILENO, F_GETFL, 0);
	Fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);//标准输出设置为非阻塞

	toiptr = tooptr = to;	/* initialize buffer pointers */
	friptr = froptr = fr;
	stdineof = 0;//标准输入结束标志

	maxfdp1 = max(max(STDIN_FILENO, STDOUT_FILENO), sockfd) + 1;
	for ( ; ; ) {
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		if (stdineof == 0 && toiptr < &to[MAXLINE])//标准输入未结束且有空间从标准输入读入
			FD_SET(STDIN_FILENO, &rset);	/* 标准输入可读 */
		if (friptr < &fr[MAXLINE])//fr缓冲有空间
			FD_SET(sockfd, &rset);			/* 从套接口读入 */
		if (tooptr != toiptr)//to缓冲有数据可写
			FD_SET(sockfd, &wset);			/* 套接口可写 */
		if (froptr != friptr)//fr有数据可写
			FD_SET(STDOUT_FILENO, &wset);	/* 标准输出可写 */

		Select(maxfdp1, &rset, &wset, NULL, NULL);
/* end nonb1 */
/* include nonb2 */
		if (FD_ISSET(STDIN_FILENO, &rset)) {//标准输入可读
			if ( (n = read(STDIN_FILENO, toiptr, &to[MAXLINE] - toiptr)) < 0) {
				if (errno != EWOULDBLOCK)
					err_sys("read error on stdin");
			} else if (n == 0) {
				stdineof = 1;			/* 标准输入结束了*/
				if (tooptr == toiptr)//若数据已经发完
					Shutdown(sockfd, SHUT_WR);/* 则关闭套接口的写 */
			} else {//正常读取
				toiptr += n;			/*读了n自己，缓冲区指针变化*/
				FD_SET(sockfd, &wset);	/* 立即本循环内尝试写套接口，已经有数据了*/
			}
		}//标准输入可读结束

		if (FD_ISSET(sockfd, &rset)) {//套接口可读
			if ( (n = read(sockfd, friptr, &fr[MAXLINE] - friptr)) < 0) {
				if (errno != EWOULDBLOCK)
					err_sys("read error on socket");

			} else if (n == 0) {//读到结束符
				if (stdineof)
					return;		/*正常结束 */
				else
					err_quit("str_cli: server terminated prematurely");
			} else {//正常读数据
				friptr += n;		/* 修改缓冲区指针*/
				FD_SET(STDOUT_FILENO, &wset);	/* 尝试立即写标准输出*/
			}
		}
/* end nonb2 */
/* include nonb3 */
		if (FD_ISSET(STDOUT_FILENO, &wset) && ( (n = friptr - froptr) > 0)) {//标准输入可写
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
				if (tooptr == toiptr) {//数据发完了，初始化缓冲区
					toiptr = tooptr = to;	/* back to beginning of buffer */
					if (stdineof)//标准输入也结束了，关闭套接口写
						Shutdown(sockfd, SHUT_WR);	/* send FIN */
				}
			}
		}
	}
}
/* end nonb3 */
