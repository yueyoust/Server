#include <functional>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include "Server.h"
#include "Util.h"

Server::Server(EventLoop *loop,int threadNum,int port)
	:loop_(loop),
	 threadNum_(threadNum),
	 eventLoopThreadPool_(new EventLoopThreadPool(loop_,threadNum)),
	 started_(false),
	 port_(port),
	 listenFd_(socket_bind_listen(port_)),
	 acceptChannel_(new Channel(loop_,listenFd_)) //add file descriptor to channel
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
	eventLoopThreadPool_->start(NULL);
//	acceptChannel_->setEvents();
	
	acceptChannel_->setReadCallback(std::bind(&Server::handNewConn,this));
	acceptChannel_->enableReading();
	//loop_->updateChannel(acceptChannel_);
	started_=true;
}

void rcallback(Channel* channel)
{
	int fd =channel->fd();
	char fbuffer[4096];
	int num=read(fd,fbuffer,4096);
	
	std::cout<<"\nfile descriptor\t"<<fd<<"\tbuffersize\t"<<num<<"\n\n"<<fbuffer<<std::this_thread::get_id()<<std::endl;
	if(num==0)
	{
		channel->remove();
		return ;
	}
	std::string header_buff,body_buff; 
	body_buff += "<html><title>not</title>";
    	body_buff += "<body bgcolor=\"ffffdd\">";
    	body_buff += "404 Not Found";
    	body_buff += "<hr><em> Yueyou's Web Server</em>\n</body></html>";
	
	header_buff += "HTTP/1.1 " + std::to_string(404) + "Not Found" + "\r\n";
    	header_buff += "Content-Type: text/html\r\n";
    	header_buff += "Connection: Close\r\n";
    	header_buff += "Content-Length: " + std::to_string(body_buff.size()) + "\r\n";
    	header_buff += "Server: yueyou's Web Server\r\n";
    	header_buff += "\r\n";
	
	write(fd,header_buff.c_str(),header_buff.size());
	write(fd,body_buff.c_str(),body_buff.size());
	//while(1);

}

void wcallback(Channel* channel)
{
	int fd=channel->fd();

	std::cout<<"write callback is called"<<std::endl;
	//write(fd,"1",1);
	//char tbuffer[]="HTTP/1.0 404 Not Found \r\nContent-Type: text/html \r\n<HTML><HEAD><TITLE>Not Found lukas</TITLE></HEAD><BODY>Not Found</BODY></HTML>Sending file not found.";
	//write(fd,tbuffer,strlen(tbuffer));
	//close(fd);
	//while(1);
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
		//std::cout<<'\n'<<"socketac"<<accept_fd<<std::endl;
		Channel *chann= new Channel(loop,accept_fd);
		chann->setReadCallback(std::bind(&rcallback,chann));
		chann->setWriteCallback(std::bind(&wcallback,chann));
		chann->enableReading();
		
		chann->enableWriting();
	}
}

