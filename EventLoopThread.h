#ifndef _EVENTLOOPTHREAD_H_
#define _EVENTLOOPTHREAD_H_

#inlucde<mutex>
#include"EventLoop.h"

class EventLoopThread
{
public:
	typedef std::function<void(EventLoop*)> ThreadInitCallback;
	EventLoopThead(const ThreadInitCallback &cb,const std::string &name=string());
	~EventLoopThread();


Private:
	void threadFunc();
	
	EventLoop *loop_;	
	std::thread thread_;

	ThreadInitCallback callback;
	
	std::mutex Mutex;

};




























#endif
