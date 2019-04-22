#ifndef _EVENTLOOPTHREADPOLL_H_
#define _EVENTLOOPTHREADPOLL_H_

#include"EventLoopThread"

class EventLoop;
class EventLoopThread;



class EventLoopThreadPool{
	
public:
	typedef std::function<void(EventLoop*)>ThreadInitCallback;
	EventLoopThreadPool(EventLoop *baseloop,const string &nameArg);
	~EventLoopThreadPool();

	void start(const ThreadInitCallback &cb);
private:
	EventLoop *baseLoop;
	std::string name;
	int numThreads;
	std::vector<std::unique_ptr<EventLoopThread>>threads;
	std::vector<EventLoop*>loops;
	
}















