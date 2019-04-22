#include"EventLoopThread"


EventLoopThread::EventLoopThread()(const ThreadInitCallback &cb,
				const std::string &name)
	:loop_(NULL)
	 thread_(std::bind(&EventLoopThread::threadFunc,this),name),
	 Mutex(),
	 callback(cb)
{
}

EventLoopThread::~EventLoopThread()
{
	if(loop_!=NULL)
	{
		thread_.join();
	}
}



void EventLoopThread::threadFunc()
{
	EventLoop loop;
	if(callback)
	{
		callback(&loop);
	}
	{
		lock_guard lock(Mutex);
		loop_=&loop;
	}
	
	loop.loop();
}



