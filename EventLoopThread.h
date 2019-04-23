#ifndef _EVENTLOOPTHREAD_H_
#define _EVENTLOOPTHREAD_H_

#include<mutex>
#include<functional>
#include"EventLoop.h"

class EventLoop;

class EventLoopThread
{
public:
	typedef std::function<void(EventLoop*)>  ThreadInitCallback;

	EventLoopThread(const ThreadInitCallback cb= ThreadInitCallback());

	~EventLoopThread();

	EventLoop *getLoop();

private:

	void threadFunc();
	
	EventLoop *loop_;	

	std::thread thread_;

	ThreadInitCallback callback;
	
	std::mutex Mutex;

};


#endif
