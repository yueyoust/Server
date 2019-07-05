#include <iostream.>
#include "Epoll.h"



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
	:
}
