#include "httpMes.h"

httpMes::httpMes(EventLoop *loop.int connfd)
	:loop_(loop),
	 fd_(connfd)
	 channel_(new Channel(loop,connfd));
{
	channel_->setReadCallback(std::bind(&httpMes::handleRead,this));
	channel_->setWriteCallback(std::bind(&httpMes::handleWrite,this));
	//channel_->
}

void httpMes::handleRead()
{
	_uint32_t &events=channel_->events();
	do
	{
		
	}while(false);
}

