#include<unistd.h>
#include"Poller.h"
#include"Channel.h"





Poller::Poller(EventLoop*loop)
	: ownerLoop(loop)
{

}

void Poller::poll(int timeoutMs,ChannelList *activeChannels)
{
	int numEvents=::poll(&*pollFds.begin(),pollFds.size(),timeoutMs);

//	std::cout<<"nums of file\t"<<pollFds.size()<<'\t';
	if(numEvents>0)
	{
		fillActiveChannels(numEvents,activeChannels);
//		std::cout<<"something happened"<<std::endl;
	}else if(numEvents==0){
//		std::cout<<"nothing happended"<<std::endl;
	}else{
//		std::cout<<"Poll ererror"<<std::endl;
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
		//	std::cout<<pfd->fd<<'\t';
			if(ch->second==NULL)
				continue;
			Channel *channel=ch->second;
			channel->set_revents(pfd->revents);	
			activeChannels->push_back(channel);
		}
	}
}

void Poller::updateChannel(Channel*channel)
{
	if(channel->index()<0)		
	{
		struct pollfd pfd;
		pfd.fd=channel->fd();
		pfd.events=channel->events();
		pfd.revents=0;
		pollFds.push_back(pfd);
		channels[pfd.fd]=channel;//add to channel map;
		int idx=static_cast<int>(pollFds.size()-1);
		channel->set_index(idx);
		std::cout<<"update channel"<<std::endl;
	}
	else
	{
		int idx=channel->index();
		struct pollfd &pfd=pollFds[idx];
		pfd.fd=channel->fd();
		pfd.events=static_cast<short>(channel->events());
		pfd.revents=0;
		std::cout<<"update existing channel"<<std::endl;
	}
}

void Poller::removeChannel(Channel *channel)
{
	int idx= channel->index();
	const struct pollfd &pfd=pollFds[idx];
	int fd=channel->fd();
	size_t n=channels.erase(channel->fd());
	if(idx==pollFds.size()-1)
	{
		pollFds.pop_back();
	}
	else
	{
		int channelAtEnd=pollFds.back().fd;
		iter_swap(pollFds.begin(),pollFds.end()-1);
		if(channelAtEnd<0)
		{
			channelAtEnd=-channelAtEnd-1;
		}
		if(channels[channelAtEnd]==NULL)
			return ;
		channels[channelAtEnd]->set_index(idx);
		pollFds.pop_back();
	}
	close(fd);

}

