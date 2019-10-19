 
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
		if (errno != EINPROGRESS)//�����Ĵ�����EINPROGRESS����ʾ���ӽ����Ѿ���������δ���
			return(-1);

	/*�˴����Ե������������ȴ����ӽ�������ڼ����κ��������������� */

	if (n == 0)
		goto done;	/* connect completed immediately��������ͻ���ͬ����ʱ���ܷ��� */

	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	wset = rset;
	tval.tv_sec = nsec;
	tval.tv_usec = 0;

	if ( (n = Select(sockfd+1, &rset, &wset, NULL,
					 nsec ? &tval : NULL)) == 0) {//ͨ��select�����ȴ�������connect���
		//select���ĸ�����ΪNULL��ʾһֱ�ȴ�����ʱ��0��ʾ���ȴ�����nsecΪ0��ȴ�ʱ��Ϊtval
		close(sockfd);		/* timeout */
		errno = ETIMEDOUT;
		return(-1);
	}

	if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
		len = sizeof(error);
		if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
			//��ȡ�׽ӿڴ��������
			return(-1);			/* ���Solaris ���� */
	} else
		err_quit("select error: sockfd not set");

done:
	Fcntl(sockfd, F_SETFL, flags);	/* �ָ��׽ӿ����� */

	if (error) {
		close(sockfd);		/* ���Berkeleyϵͳ�Ĵ��� */
		errno = error;
		return(-1);
	}
	return(0);
}
