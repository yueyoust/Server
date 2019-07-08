#include <functional>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "Server.h"
#include "Util.h"

Server::Server(EventLoop *loop,int threadNum,int port)
	:loop_(loop),
	 threadNum_(threadNum),
	 eventLoopThreadPool_(new EventLoopThreadPool(loop_,threadNum)),
	 started_(false),
	 port_(port),
	 listenFd_(socket_bind_listen(port_)),
	 acceptChannel_(new Channel(loop_,listenFd_))
{
	//acceptChannel_->setFd(listenFd_);
	if(setSocketNonBlocking(listenFd_)<0)
	{
		perror("set socket nonblock failed");
		abort();
	}
}


void Server::start()
{
//	std::function<void(EventLoop*)> func;
	eventLoopThreadPool_->start(NULL);
//	acceptChannel_->setEvents();
	
	acceptChannel_->setReadCallback(std::bind(&Server::handNewConn,this));
		
	loop_->updateChannel(acceptChannel_);
	started_=true;
}

void Server::handNewConn()
{
	struct sockaddr_in client_addr;
	memset(&client_addr,0, sizeof(struct sockaddr_in));
	socklen_t client_addr_len =sizeof(client_addr);
	int accept_fd=0;
	while((accept_fd=accept(listenFd_ ,(struct sockaddr*) &client_addr,&client_addr_len))>0)
	{
		EventLoop *loop=eventLoopThreadPool_->getNextLoop();
		
		if(accept_fd>=MAXFDS)
		{
			close(accept_fd);
			continue;
		}
		
		if(setSocketNonBlocking(accept_fd)<0)
		{
			std::cout<<"nonblock set failed\n";
			return;
		}
		
		setSocketNoDelay(accept_fd);
		//shared_ptr<httpMes> req_info(new (httpMes(loop,accept_fd)));
		//req_info->
		std::cout<<"new connection come from fd\n";
//		Channel chann(new Channel(*loop,accept_fd));
//		loop_->updateChannel(chann);
	}
}
