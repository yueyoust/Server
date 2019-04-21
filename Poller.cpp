#include"Poller.h"
#include"Channel.h"
Poller::Poller(EventLoop*loop)
	: ownerLoop(loop){}



void Poller::poll(int timeoutMs,ChannelList *activeChannels)
{
	int numEvents=::poll(&*pollFds.begin(),pollFds.size(),timeoutMs);

	//std::cout<<"nums of file\t"<<pollFds.size()<<'\t'<<pollFds.begin()->fd<<std::endl;
	if(numEvents>0)
	{
		std::cout<<"something happened"<<std::endl;
		fillActiveChannels(numEvents,activeChannels);
	}else if(numEvents==0){
		std::cout<<"nothing happended"<<std::endl;
	}else{
		std::cout<<"Poll ererror"<<std::endl;
	}
}


void Poller::fillActiveChannels(int numEvents,ChannelList *activeChannels)const
{
	for(auto pfd=pollFds.begin(); pfd!=pollFds.end()&&numEvents>0;pfd++)
	{
		if(pfd->revents>0)
		{
			--numEvents;
			auto ch=channels.find(pfd->fd);
			Channel *channel=ch->second;
			channel->set_revents(pfd->revents);	
			activeChannels->push_back(channel);
		}
	}
}

void Poller::updateChannel(Channel*channel)
{
	struct pollfd pfd;
	pfd.fd=channel->fd();
	pfd.events=channel->events();
	pfd.revents=0;
	pollFds.push_back(pfd);
	channels[pfd.fd]=channel;//add to channel map;
	std::cout<<"update channel"<<std::endl;
	
}

