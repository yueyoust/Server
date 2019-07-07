#ifndef _SERVER_H_
#define _SERVER_H_
#include <memory>
#include "EventLoop.h"
#include "Channel.h"
#include "EventLoopThreadPool.h"
#include "Util.h"

class  Server{
public:
	Server(EventLoop *loop,int threadNum,int port);
	
	~Server();
	
	EventLoop* getLoop()const{ return loop_;}

	void start();
	
	void handNewConn();

	void handThisConn() { loop_->updateChannel(acceptChannel_); }

private:
	EventLoop *loop_;
	int threadNum_;
	std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool_;
	bool started_;
	int listenFd_;
	int port_;
	std::shared_ptr<Channel>acceptChannel_;
	static const int MAXFDS=10000;
};























#endif
