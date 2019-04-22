#include"EventLoopThreadPoll.h"
`

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseloop,const string &nameArg)
	:baseLoop(baseloop),
	 name(nameArg),
	 numThreads(0)
{
}

void EventLoopThreadPool(const ThreadInitCacllback)
{
	for(int i=0;i<numThreads;i++)
	{
		char buff(name.size()+32];
		snprintf(buff,sizeof buff, "%s%d",name.c_str(),i);
		EventLoopThread *t=new EventLoopThread(cb,buff);
		threads.push_back(std::unique_ptr<EventLoopThread>(t));
	}
	if(numThreads==0&&cb)
	{
		cb(baseLoop);
	}
}
