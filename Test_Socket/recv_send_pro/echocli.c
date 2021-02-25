#include<unistd.h>		//read|write
#include<sys/types.h>		//是Unix/Linux基本系统数据类型，含有size_t，time_t，pid_t等类型
#include<sys/socket.h>		//socket
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>

#include<netinet/in.h>
#include<arpa/inet.h>

#include<string.h>		//memset

#define ERR_EXIT(m)\
	do\ 
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)
//自定义一个包
//struct packet
//{
//        int len;
//        char buf[1024];
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

void echo_cli(int sock)
{
	//通信细节
	char sendbuf[1024] = {0};
	char recvbuf[1024] = {0};
	while(fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
	{
		writen(sock, sendbuf, strlen(sendbuf));
		int ret = readline(sock, recvbuf, sizeof(recvbuf));
                if(ret == -1)
                        ERR_EXIT("readline\n");
                else if(ret ==  0)
                {       
                        printf("client close\n");
                        break;
                }       

		fputs(recvbuf, stdout);
		//每一次都清空缓冲区
		memset(sendbuf, 0, sizeof(sendbuf));
		memset(recvbuf, 0, sizeof(recvbuf));
	}
	close(sock);
}

//客户端程序
int main(void)
{
	//socket
	int sock;
	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket ");
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	//地址族
	servaddr.sin_family = AF_INET;
	//端口号
	servaddr.sin_port = htons(5188);	//占两个字节用htons
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");	//服务器端的地址
	
	//connect
	if(connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)	
		ERR_EXIT("connect");
	
	//getsockname使用
	struct sockaddr_in localaddr;
	socklen_t addrlen = sizeof(localaddr);	//必须有初始值
	if(getsockname(sock, (struct sockaddr*)&localaddr, &addrlen) < 0)
		ERR_EXIT("getsockname");
	printf("ip=%s port=%d\n", inet_ntoa(localaddr.sin_addr), ntohs(localaddr.sin_port));

	echo_cli(sock);
	
	return 0;
}
