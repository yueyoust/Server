#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include "Util.h"

int socket_bind_listen(int port)
{
	if(port<0 || port>65535)
		return 0;
	int listen_fd=0;
	if((listen_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
		return -1;

	int optval=1;
	if(setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval))==-1)
		return -1;

	struct sockaddr_in server_addr;
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr("192.168.145.128");//&//htonl(INADDR_ANY);
	server_addr.sin_port=htons((unsigned short)port);

	if(bind(listen_fd,(struct sockaddr *)&server_addr,sizeof(server_addr))==-1)
		return -1;


	if(listen(listen_fd,2048)==-1)
		return -1;
	
	//std::cout<<"lllllllllllllllllllllllllllllliiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiisssssssssssssssssssssssssssssssttttttttttttttttttttttttt"<<std::endl;
	return listen_fd;
}

int setSocketNonBlocking(int fd)
{
	int flag=fcntl(fd,F_GETFL,0);
	if(flag==-1)
		return -1;

	flag |=O_NONBLOCK;
	if(fcntl(fd,F_SETFL,flag)==-1)
		return -1;
	return 0;
}

void setSocketNoDelay(int fd)
{
	int enable=1;
	setsockopt(fd,SOL_SOCKET,TCP_NODELAY,(void*)&enable,sizeof(enable));
}

