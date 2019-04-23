#ifndef _EVENTLOOPTHREADPOLL_H_
#define _EVENTLOOPTHREADPOLL_H_

#include"EventLoopThread.h"

class EventLoop;
class EventLoopThread;



class EventLoopThreadPool{
	
public:
	typedef std::function<void(EventLoop*)>ThreadInitCallback;
	
	EventLoopThreadPool(EventLoop *baseloop,const int numthreads);

	~EventLoopThreadPool();

	void start(const ThreadInitCallback &cb);
	
	EventLoop* getNextLoop();
	
private:

	EventLoop *baseLoop;

	int numThreads;
	
	int next;

	std::vector<std::unique_ptr<EventLoopThread>>threads;
	std::vector<EventLoop*>loops;
	
};





#endif
