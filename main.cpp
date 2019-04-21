#include<iostream>
#include<string.h>
#include<unistd.h>
#include<sys/timerfd.h>
#include"EventLoop.h"
#include"Channel.h"

EventLoop *g_loop;
void  timeout()
{
	std::cout<<"timeout"<<std::endl;
	return 0;
}
int main()
{
	std::function<void()>callback=timeout;
	EventLoop loop;
	g_loop=&loop;
	int timefd=::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
	std::cout<<timefd;
	Channel channel(&loop,timefd);
	channel.setReadCallback(timeout);
	channel.enableReading();
	

	//struct pollfd time;
	struct itimerspec howlong;
	bzero(&howlong,sizeof howlong);
	howlong.it_value.tv_sec=5;
	::timerfd_settime(timefd,0,&howlong,NULL);
	//poll(&timefd,1,1000000);
	loop.loop();
	std::cout<<"main loop"<<std::endl;
	::close(timefd);
}
