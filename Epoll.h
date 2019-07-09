#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <vector>
#include <memory>
#include <unordered_map>
#include <sys/epoll.h>
#include <map>
#include "Channel.h"

class Epoll
{
public:
	typedef std::vector<Channel*> ChannelList;
	Epoll(EventLoop* loop);	
	~EPoll();
	
	void epoll_add(SP_Channel request,int timeout);
		
	void epoll_mod(SP_Channel request,int timeout);
	
	void epoll_del(SP_Channel request,int timeout);

	std::vector<std::shared_ptr<Channel>> poll();

	std::vector<std::shared_ptr<Channel>>getEventsRequest(int events_num);

	int getEpollFd()
	{
		return epollFd_;
	}

	void handExpired();

	void updateChannel(Channel* channel);

	void poll(int timeoutMs,ChannelList *activeChannels);

private:
	typedef std::map<int Channel*> ChannelMap;
	ChannelMap channels_;
	
	EventLoop *ownerLoop;

	void fillActiveChannels(int numEvents,ChannelList *activeChannels) const;
	//typedef std::shared_ptr<Channel> SP_Channel;

	static const int MAXFDS=10000;
	
	int epollFd_;
	
	
	std::vector<epoll_event> events_;

	std::shared_ptr<Channel> fdChannels_[MAXFDS];
};

#endif


