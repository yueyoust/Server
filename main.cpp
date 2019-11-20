#include <iostream>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <sys/timerfd.h>
#include "EventLoop.h"
#include "Channel.h"
#include "EventLoopThreadPool.h"
#include "Server.h"
#include "Timer.h"
#include <signal.h>
//EventLoop *g_loop;
void  timeout()
{	
	std::cout<<"timeout"<<std::endl;
}	
int main()
{
	//Timer tmn(NULL,);
	
/*	int tfd=timerfd_create(CLOCK_REALTIME,0);
	struct itimerspec newvalue;
	struct timespec now;
	clock_gettime(CLOCK_REALTIME,&now);
	
	newvalue.it_value.tv_sec=now.tv_sec+3;
	newvalue.it_value.tv_nsec=now.tv_nsec;

	newvalue.it_interval.tv_sec=0;
	newvalue.it_interval.tv_nsec=100*1000;

	timerfd_settime(tfd,TFD_TIMER_ABSTIME,&newvalue,NULL);

	struct pollfd pfd;
	pfd.fd=tfd;
	pfd.events=POLLIN||POLLPRI||POLLRDNORM;
	while(true)
	{
		::poll(&pfd,1,10000);
		char buff[90];
		read(tfd,buff,90);
		std::cout<<"timer was triggered\n";
	}
*/	
	Timestamp time=Timestamp::now();
	std::cout<<time.toString()<<std::endl;;
	signal(SIGPIPE,SIG_IGN);//ignore SIGPIPE;
	std::function<void()>callback=timeout;
	EventLoop loop;
	//g_loop=&loop;
	Server ser(&loop,3,8080);
	ser.start();
//	EventLoopThreadPool th(&loop,3);
		
//	th.start(NULL);
	
/*	int timefd=::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
	std::cout<<timefd;
	Channel channel(&loop,timefd);
	channel.setReadCallback(timeout);
	channel.enableReading();
	
	
	//struct pollfd time;
	struct itimerspec howlong;
	bzero(&howlong,sizeof howlong);
	howlong.it_value.tv_sec=5;
	::timerfd_settime(timefd,0,&howlong,NULL);
*/	loop.loop();
	
	
	
	std::cout<<"main loop"<<std::endl;
//	::close(timefd);
}	
