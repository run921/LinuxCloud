#include<unistd.h>		//read()、write()
#include<sys/types.h>		//是Unix/Linux基本系统数据类型，含有size_t，time_t，pid_t等类型
#include<sys/socket.h>		//socket
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<signal.h>

#include<netinet/in.h>
#include<arpa/inet.h>

#include<string.h>		//memset

#define ERR_EXIT(m)\
	do\ 
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)

//使用readline解决粘包
//自定义一个包
//struct packet
//{
//	int len;
//	char buf[1024];
//};

//read封装
ssize_t readn(int fd, void * buf, size_t count)
{
	size_t nleft = count;	
	ssize_t nread;
	char *bufp = (char*)buf;
	while(nleft > 0)
	{
		if((nread = read(fd, buf, nleft)) < 0)
		{
			if(errno == EINTR)
				continue;
			return -1;
		}
		else if(nread == 0)
			return count - nleft;
		bufp += nread;
		nleft -= nread;
	}
}

//write封装
ssize_t writen(int fd, void * buf, size_t count)
{
	size_t nleft = count;	
	ssize_t nwritten;
	char *bufp = (char*)buf;
	while(nleft > 0)
	{
		if((nwritten = write(fd, buf, nleft)) < 0)
		{
			if(errno == EINTR)
				continue;
			return -1;
		}
		else if(nwritten == 0)
			continue;
		bufp += nwritten;
		nleft -= nwritten;
	}
}

//recv封装
ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
	while(1)
	{
		int ret = recv(sockfd, buf, len, MSG_PEEK);
		if(ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}

//通过recv_peek实现readline
ssize_t readline(int sockfd, void *buf, size_t maxline)
{
	int ret;
	int nread;
	char *bufp = buf;
	int nleft = maxline;
	while(1)
	{
		ret = recv_peek(sockfd, bufp, nleft);
		if(ret < 0)
			return ret;
		else if(ret == 0)
			return ret;
		nread = ret;
		int i;
		for(i = 0; i < nread; i++)
		{
			if(bufp[i] == '\n')
			{
				ret == readn(sockfd, bufp, i+1);
				if(ret != i+1)
					exit(EXIT_FAILURE);
				return ret;
			}
		}
			
		if(nread > nleft)
			exit(EXIT_FAILURE);
		bufp += nread; 
	}
	
	return -1;
}

void echo_srv(int conn)
{
	char recvbuf[1024];
	int n;
	while(1)
	{	
		memset(recvbuf, 0, sizeof(recvbuf));
		int ret = readline(conn, recvbuf, 1024);
		if(ret == -1)
			ERR_EXIT("readine\n");
		if(ret == 0)
		{
			printf("client close\n");
			break;
		}
		
		fputs(recvbuf, stdout);
		writen(conn, recvbuf, strlen(recvbuf));
	}
}	
//实现简单的客户端请求，服务器响应直接回显
int main(void)
{
	signal(SIGCHLD, SIG_IGN);
	//一、socket,创建一个套接字
	int listenfd;
	if((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	//二者都可以
	//if((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		ERR_EXIT("socket ");
	
	//二、初始化地址
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	//地址族
	servaddr.sin_family = AF_INET;
	//端口号
	servaddr.sin_port = htons(5188);	//占两个字节用htons
	//地址（三种方法）
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	//1、INADDR_ANY表示本机的任意地址，可以省略
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");	//2、指定特定IP地址
	//inet_aton("127.0.0.1", &servaddr.sin_addr);	//3、将点分式地址转换为网络式地址
	
	//绑定之前开启地址重复利用REUSEADDR
	int on = 1;
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		ERR_EXIT("setsockopt");
		
	//三、bind绑定地址
	if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("bind");
	//四、listen监听套接字
	if(listen(listenfd, SOMAXCONN) < 0)	//添加参数后，被动套接字
		ERR_EXIT("listen");
	//五、accept接受连接，取已完成连接的队头，返回一个连接，得到新的套接字（主动套接字）
	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);	//必须初始化，否则accept会失败
	int conn;
	
	pid_t pid;	
	while(1)
	{
		if((conn = accept(listenfd, (struct sockaddr*)&peeraddr ,&peeraddr)) < 0)
		ERR_EXIT("accept");
		//打印对方的ip和端口
		printf("连接的网络");
		printf("ip=%s port=%s\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
		
		pid = fork();
		if(pid == -1)
			ERR_EXIT("fork");
		if(pid == 0)
		{
			close(listenfd);
			echo_srv(conn);
			exit(EXIT_SUCCESS);	
		}
		else
			close(conn);
	}
	return 0;
}
