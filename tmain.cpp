#include<iostream>
#include<string.h>
#include<unistd.h>
#include<thread>
#include<sys/timerfd.h>
#include"EventLoop.h"
#include"Channel.h"
#include"EventLoopThreadPool.h"
//EventLoop *g_loop;
void  timeout()
{
	std::cout<<"timeout"<<std::endl;
}

void tfunction()
{
	EventLoop ll;
	Channel *cha=new Channel(&ll,3);
	cha->setReadCallback(timeout);
	cha->enableReading();
	while(1);
}
int main()
{
	
	EventLoop loop;
	
	EventLoopThread tl;
	/*EventLoop *rl=tl.getLoop();
	do
	{
		rl=tl.getLoop();
		std::cout<<"\\"<<std::endl;
	}while(rl==NULL);
	std::cout<<"kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk"<<std::endl;	
*/	Channel  ch(tl.getLoop(),2);
	ch.setReadCallback(timeout);
	ch.enableReading();
	while(1);
/*	EventLoopThreadPool th(&loop,3);
	
	th.start(NULL);

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
	loop.loop();



	std::cout<<"main loop"<<std::endl;
	::close(timefd);*/
}
