#ifndef _EventLoop_H
#define _EventLoop_H

#include<thread>
#include<functional>
#include<vector>
#include<mutex>
#include<poll.h>
#include<memory>
#include"Channel.h"
#include"Poller.h"

class Poller;
class Channel;
class EventLoop{
public:
	typedef std::function<void()> Functor;
	EventLoop();
	~EventLoop();
	void loop();

	
	void assertInLoopThread()
	{
		if(!isInLoopThread())
		{
			abortNotInLoopThread();
		}	
	}
	bool isInLoopThread()const {return threadId==std::this_thread::get_id();}

	void updateChannel(Channel *channel);
		
	void removeChannel(Channel *channel);

	void queueInLoop(Functor &&cb);

	
private:
	typedef std::vector<Channel*> ChannelList;
	
	std::vector<Functor> pendingFunctors_;
	
	std::mutex Mutex_;

	void abortNotInLoopThread();

	bool looping;

	const std::thread::id threadId;
	
	std::unique_ptr<Poller> poller_;
	//Poller* poller_;

	ChannelList  activeChannels;
	
	void doPendingFunctors();
};




#endif
