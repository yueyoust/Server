#include <functional>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Server.h"


Server::server(EventLoop *loop,int threadNum,int port)
	:loop_(loop),
	 threadNum_(threadNum),
	 eventLoopThreadPool_(new EventLoopThreadPool(loop_,threadNum)),
	 started_(false),
	 acceptChannel_(new Channel(loop_)),
	 port_(port),
	 listenFd_(socket_bind_listen(port_))
{
	acceptChannel_->setFd(listenFd_);

	if(setSocketNonBlocking(listenFd_)<0)
	{
		perror("set socket nonblock failed");
		abort();
	}
}


void Server::start()
{
	eventLoopThreadPool_->start();
	
	started_=true;
}

void Server::handNewConn()
{
	struct sockaddr_in client_addr;
	memset(&client_addr,0, sizeof(struct sockaddr_in));
	socklen_t client_addr_len =sizeof(client_addr);
	int accept_fd=0;
	while((accept_fd=aceept(listenFd_ (struct sockaddr*) &client_addr,&client_addr_len))>0)
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
		
		setSocketNodelay(accept_fd);
	}
}
