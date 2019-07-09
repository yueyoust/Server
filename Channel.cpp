#include "Channel.h"

const int Channel::kreadEvent=POLLIN;//|POLLPRI;
const int Channel::kwriteEvent=POLLOUT;



Channel::Channel(EventLoop *loop,int fda)
	:loop_(loop),
	 fd_(fda),
	 events_(0),
	 revents_(0)
	 {}


void Channel::update()
{
	loop_->updateChannel(this);
}

/*void Channel::setFd(int fd)
{
	fd_ = fd;
}*/

void Channel::handleEvent()
{
	
	if(revents_&(POLLIN|POLLPRI|POLLRDHUP))
	{
		if(readCallBack)
			readCallBack();
	}
	if(revents_&POLLOUT)
	{
		if(writeCallBack)
			writeCallBack();
	}
}







































































