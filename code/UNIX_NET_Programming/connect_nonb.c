 
int connect_nonb(int sockfd, const SA *saptr, socklen_t salen, int nsec)
{
	int				flags, n, error;
	socklen_t		len;
	fd_set			rset, wset;
	struct timeval	tval;

	flags = Fcntl(sockfd, F_GETFL, 0);
	Fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

	error = 0;
	if ( (n = connect(sockfd, saptr, salen)) < 0)
		if (errno != EINPROGRESS)//期望的错误是EINPROGRESS，表示连接建立已经启动，尚未完成
			return(-1);

	/*此处可以跌合其他处理，等待连接建立完成期间做任何我们想做的事情 */

	if (n == 0)
		goto done;	/* connect completed immediately服务器与客户机同主机时可能发生 */

	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	wset = rset;
	tval.tv_sec = nsec;
	tval.tv_usec = 0;

	if ( (n = Select(sockfd+1, &rset, &wset, NULL,
					 nsec ? &tval : NULL)) == 0) {//通过select阻塞等待非阻塞connect完成
		//select第四个参数为NULL表示一直等待不超时，0表示不等待。若nsec为0则等待时间为tval
		close(sockfd);		/* timeout */
		errno = ETIMEDOUT;
		return(-1);
	}

	if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
		len = sizeof(error);
		if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
			//获取套接口待处理错误
			return(-1);			/* 解决Solaris 错误 */
	} else
		err_quit("select error: sockfd not set");

done:
	Fcntl(sockfd, F_SETFL, flags);	/* 恢复套接口属性 */

	if (error) {
		close(sockfd);		/* 解决Berkeley系统的错误 */
		errno = error;
		return(-1);
	}
	return(0);
}
