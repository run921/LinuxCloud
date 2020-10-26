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
struct packet
{
        int len;
        char buf[1024];
};

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
	//通信细节
	struct packet sendbuf;
	struct packet recvbuf;
	memset(&sendbuf, 0, sizeof(sendbuf));
	memset(&recvbuf, 0, sizeof(recvbuf));
	int n;
	//char sendbuf[1024] = {0};
	//char recvbuf[1024] = {0};
	while(fgets(sendbuf.buf, sizeof(sendbuf.buf), stdin) != NULL)
	{
		n = strlen(sendbuf.buf);
		sendbuf.len = htonl(n);
		writen(sock, &sendbuf, 4+n);
		int ret = readn(sock, recvbuf.len, 4));
                if(ret == -1)
                        ERR_EXIT("read\n");
                else if(ret < 4)
                {       
                        printf("client close\n");
                        break;
                }       
                n = ntohl(recvbuf.len);
                ret = readn(sock, recvbuf.buf, n);
                if(ret == -1)
                        ERR_EXIT("read\n");
                else if(ret < n)
                {
                        printf("client close\n");
                        break;
                }

		fputs(recvbuf.buf, stdout);
		//每一次都清空缓冲区
		memset(&sendbuf, 0, sizeof(sendbuf));
		memset(&recvbuf, 0, sizeof(recvbuf));
	}
	close(sock);
	
	return 0;
}
