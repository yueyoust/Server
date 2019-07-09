#include <iostream.>
#include "Epoll.h"

const int EVENTSNUM=4096;
const int EPOLLWAIT_TIME=10000;

Epoll::Epoll()
	:epollFd_(epoll_create1(EPOLL_CLOSEXEC)),
	 events(EVENTSNUM)
{
}


Epoll:: ~Epoll()
{
}


void Epoll::epoll_add(SP_Channel request,int timeout)
{
	int fd=request->fd();
	if(timeout>0)
	{
		
	}
	struct epoll_event event;
	event.data.fd=fd;
	event.events=request->getEvents();

	fdChannels_[fd]=request;

	if(epoll_ctl(epollFd_,EPOLL_CTL_ADD,fd&event)<0)
	{
		perror("epoll_add error");
	}
}


void Epoll::epoll_mod(SP_Channel request,int timeout)
{
	if(timeout>0)
	;
	int fd= request->fd();
	
	struct epoll_event event;
	event.data.fd=fd;
	
	event.events=request->getEvents();

	if(epoll_ctl(epollFd_,EPOLL_CTL_MOD,fd,&event)<0)
	{
		perror("epoll_mod error");
		fdChannels_[fd].reset();
	}
}

void Epoll::epoll_del(SP_Channel request)
{
	int fd=request->fd();
	struct epoll_event event;
	event.data.fd=fd;
	event.events=request->getEvents();
	
	if(epoll_ctl(epollFd_,EPOLL_CTL_DEL,fd,&event)<0)
	{
		perror("epoll_del error");
		//fdChannels_[fd].reset();
	}
}
std::vector<SP_Channel> Epoll::poll()
{
	while(true)
	{
		int event_count=epoll_wait(epollFd_,&*events_.begin(),events_.size(),EPOLLWAIT_TIME);
		if(event_count<0)
		{
			perror("epoll_wait error");
		}
		
		std::vector<SP_Channel> req_data=getEventsRequests(event_count);
		if(req_data.size()>0)
		return req_data;
	}
}


std::vector<SP_Channel> Epoll::getEventsRequest(int events_num)
{
	std::vector<SP_Channel> req_data;
	for(int i=0;i<events_num;++i)
	{
		int fd=events_[i].data.fd;
		SP_Channel cur_req=fdChannels_[fd];
		
		if(cur_req)
		{
			cur_req->setRevents(events_[i].events)
			cur_req->setEvents(0);
			req_data.push_back(cur_req);
		}
		else
		{
		}
	}
	return req_data;
}


void Epoll::fillActiveChannels(int numEvents,ChannelList *activeChannels) const
{
	for(int i=0; i<numEvents;++i)
	{
		Channel *channel=static_cast<Channel*>(events_[i].data.ptr);
		
		int fd=Channel->fd();
		ChannelMap::const_iterator it=Channels_find(fd);
		Channel->set_revents(events_[i].events);
		activeChannels->push_back(channel);
	}
}









