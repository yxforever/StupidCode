#include <stdio.h>
#include <stdlib.h> 
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
int main (int argc, char *argv [ ])
{
	struct hostent *h ;
	int i;
	/* 检测命令行中的参数是否存在 */
	if (argc !=2)
	{
		/* 如果没有参数,则给出使用方法 */
		printf("usage:getip address \n");
		exit (1);
	}
	/* 取得主机信息 */
	if ((h=gethostbyname(argv[1]))==NULL)
	{
	/* 如果gethostbyname失败,则给出错误信息 */
		printf("gethostbyname error:%s\n",hstrerror(h_errno));

		/* 然后退出 */
		exit (1);
	}
	/* 列出程序取得的信息 */
	
	printf("h_name:%s\n",h->h_name);
	printf("h_length:%d\n",h->h_length);
	printf("h_addrtype:%d",h->h_addrtype);
	for(i=0;h->h_aliases[i] !=NULL;i++)
		printf("h_aliases%d:%s\n",i+1,h->h_aliases[i]); 
	for(i=0;h->h_addr_list[i]!=NULL;i++){
		struct in_addr *addr;
		addr=(struct in_addr *)h->h_addr_list[i];
		printf("ip%d:%s\n",(i+1),inet_ntoa(*addr));
	}

	/* 返回 */
	return 0;
}

/*[root@localhost root]# cd /home/NWP/
[root@localhost NWP]# ./DNS 
usage:getip address 
[root@localhost NWP]# ./DNS www.fzu.edu.cn
h_name:www.fzu.edu.cn
h_length:4
h_addrtype:2
ip1:219.229.132.10

[root@localhost NWP]# ./DNS www.sina.com.cn
h_name:ara.sina.com.cn
h_length:4
h_addrtype:2
h_aliases1:www.sina.com.cn
h_aliases2:jupiter.sina.com.cn
ip1:58.63.236.48
ip2:58.63.236.49
ip3:58.63.236.50
ip4:58.63.236.26
ip5:58.63.236.27
ip6:58.63.236.37
ip7:58.63.236.38
ip8:58.63.236.39
ip9:58.63.236.40
ip10:58.63.236.41
ip11:58.63.236.42
ip12:58.63.236.43
ip13:58.63.236.44
ip14:58.63.236.45
ip15:58.63.236.46
ip16:58.63.236.47

[root@localhost NWP]# ./DNS www.sina.com.cn
h_name:ara.sina.com.cn
h_length:4
h_addrtype:2
h_aliases1:www.sina.com.cn
h_aliases2:jupiter.sina.com.cn
ip1:58.63.236.26
ip2:58.63.236.27
ip3:58.63.236.37
ip4:58.63.236.38
ip5:58.63.236.39
ip6:58.63.236.40
ip7:58.63.236.41
ip8:58.63.236.42
ip9:58.63.236.43
ip10:58.63.236.44
ip11:58.63.236.45
ip12:58.63.236.46
ip13:58.63.236.47
ip14:58.63.236.48
ip15:58.63.236.49
ip16:58.63.236.50
[root@localhost NWP]# 
*/