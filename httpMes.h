#ifndef _HTTPMES_H_
#define _HTTPMES_H_
#include <string>
#include <unordered_map>
#include <memory>
#include <unistd.h>
#include "EventLoop.h"
#include "Channel.h"

class httpMes
{
public:
	httpMes(EventLoop *loop,int connfd);

	~httpMes() {close(fd_);}


private:
	const static int BufferSize=4096;	

	EventLoop *loop_;

	int fd_;

	std::shared_ptr<Channel> channel_;

	int  posBuffer_; 
	
	char Buffer_[BufferSize];	
		
	void handleRead();
	
	void handleWrite();
	
	void handleConn();

};

#endif
