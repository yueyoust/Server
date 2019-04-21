#ifndef _POLLER_H_
#define _POLLER_H_
#include<vector>
#include<poll.h>
#include<iostream>
#include<map>
#include"Channel.h"
#include"EventLoop.h"

class Channel;
class EventLoop;

class Poller{
public:
	typedef std::vector<Channel *> ChannelList;
	
	Poller(EventLoop *loop);


	void updateChannel(Channel *channel);

	void poll(int timeouMs,ChannelList *activeChannels);


private:
	typedef std::map<int,Channel *> ChannelMap;
	typedef std::vector<struct pollfd> PollFdList;

	void fillActiveChannels(int numEvents,ChannelList *activeChannels) const;

	EventLoop *ownerLoop;
	PollFdList pollFds;
	ChannelMap channels;



};

#endif
