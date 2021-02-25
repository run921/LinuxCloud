#include<unistd.h>		//read()、write()
#include<sys/types.h>		//是Unix/Linux基本系统数据类型，含有size_t，time_t，pid_t等类型
#include<sys/socket.h>		//socket
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>

#include<netinet/in.h>
#include<arpa/inet.h>

#include<string.h>		//memset
#include<netdb.h>		//gethostname
#define ERR_EXIT(m)\
	do\ 
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)
//获取本机IP
int getlocalip(char *ip)
{
	char host[100] = {0};
	if(gethostname(host, sizeof(host)) < 0)
		return -1;
	struct hostent *hp;
	if((hp = gethostbyname(host)) == NULL)
		return -1;
	strcpy(ip, inet_ntoa(*(struct in_addr*)hp->h_addr));
	return 0;
}

int main(void)
{
	char host[100] = {0};
	if(gethostname(host, sizeof(host)) < 0)
		ERR_EXIT("gethostname");
	struct hostent *hp;
	if((hp = gethostbyname(host)) == NULL)
		ERR_EXIT("gethostbyname");
	int i ;
	while(hp->h_addr_list[i] != NULL)
	{
		printf("%s\n",inet_ntoa(*(struct in_addr*)hp->h_addr_list[i]));
		i++;
	}
	char ip[16] = {0};
	getlocalip(ip);
	printf("localip = %s\n",ip);
	return 0;
}
