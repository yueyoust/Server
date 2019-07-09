#include <iostream>
#include "EventLoopThreadPool.h"


EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseloop,const int numthreads)
	:baseLoop(baseloop),
	 numThreads(numthreads),
	 next(0)
{
}


EventLoopThreadPool::~EventLoopThreadPool()
{
}


void EventLoopThreadPool::start(const ThreadInitCallback &cb)
{
	for(int i=0;i<numThreads;i++)
	{
//		std::cout<<"thread pool start"<<std::endl;
		EventLoopThread *t=new EventLoopThread(cb);
		threads.push_back(std::unique_ptr<EventLoopThread>(t));
		loops.push_back(t->getLoop());
	}

	if(numThreads==0&&cb)
	{
		cb(baseLoop);
	}
}


EventLoop *EventLoopThreadPool::getNextLoop()
{
	baseLoop->assertInLoopThread();
	//std::cout<<"pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp";
	EventLoop *loop=baseLoop;
	if(!loops.empty())
	{
		loop=loops[next];
		++next;
		if(next>(loops.size()))   {next=0;}
	}
	return loop;
}
