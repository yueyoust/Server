#include"EventLoop.h"
#include<poll.h>
#include<iostream>

__thread EventLoop *t_loopInThisThread=0;

EventLoop::EventLoop()
	:looping(false),
	 threadId(std::this_thread::get_id()),
	 poller_(new Poller(this))
{
	if(t_loopInThisThread)
	{
		std::cout<<"annother EventLoop	exists inthe same thread"<<std::endl;	
	}
	else
	{
		t_loopInThisThread=this;
	}
}


EventLoop::~EventLoop()
{
	t_loopInThisThread=this;
}


void EventLoop::loop()
{
	assertInLoopThread();
	looping=true;
	while(true)
	{
		activeChannels.clear();
		//::poll(NULL,0,1000);
		poller_->poll(1000,&activeChannels);
//		std::cout<<activeChannels.size()<<'\t';	
		for(auto it =activeChannels.begin();it!=activeChannels.end();it++)
		{
			(*it)->handleEvent();
		}
	//	std::cout<<"looping\t"<<std::this_thread::get_id()<<'\t'<<activeChannels.size()<<std::endl;
	}
	looping=false;
}

void EventLoop::abortNotInLoopThread()
{
	std::cout<<" NOT IN THIS THREAD"<<std::endl;
}


void EventLoop::updateChannel(Channel *channel)
{

	poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
	poller_->removeChannel(channel);
}





