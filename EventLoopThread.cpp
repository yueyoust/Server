#include"EventLoopThread.h"


EventLoopThread::EventLoopThread(const ThreadInitCallback cb)
	:loop_(NULL),
	 thread_(std::bind(&EventLoopThread::threadFunc,this)),
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


EventLoop* EventLoopThread::getLoop()
{
	return loop_;
}


void EventLoopThread::threadFunc()
{
	EventLoop loop;
	std::cout<<"looping111111111111111111111111111 \t"<<std::this_thread::get_id()<<std::endl;
	if(callback)
	{
		callback(&loop);
	}

	{
		std::lock_guard<std::mutex> lock(Mutex);
		loop_=&loop;
	}
	
	loop.loop();
}



