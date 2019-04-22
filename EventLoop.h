#ifndef _EventLoop_H
#define _EventLoop_H

#include<thread>
#include<functional>
#include<vector>
#include<poll.h>
#include<memory>
#include"Channel.h"
#include"Poller.h"

class Poller;
class Channel;
class EventLoop{
public:
	typedef std::function<void()> EventCallBack;
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

private:
	

	typedef std::vector<Channel*> ChannelList;

	void abortNotInLoopThread();

	bool looping;
	const std::thread::id threadId;
	
	std::unique_ptr<Poller> poller;

	ChannelList  activeChannels;
};




#endif
