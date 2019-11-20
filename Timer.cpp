#include "Timer.h"
#include <time.h>
#include <stdio.h>
#include <sys/timerfd.h>
Timer::Timer()
{
;
}

Timer::Timer(httpMes *request,TimerQueue *tmq,int64_t timeoutSec=10)
	:httpRequest_(request),
	 timeoutSec_(timeoutSec),
	 latestRefreshTime_(new int64_t(0)),
	 timerQueue_(tmq),
	 nowTimerQueuePos_(new int(timerQueue_->index()))
{
/*	if(timerQueue_==NULL)
	{
		timerQueue_= new TimerQueue(20);;//(static_cast<int>10*timeoutSec);
	}*/
	//struct timeval now;

	//gettimeofday(&now,NULL);
	
	//*latestRefreshTime_=now.tv_sec*1000*1000+now.tv_usec;

	timerQueue_->push(*this);
	//refresh();
}

void Timer::refresh()
{

	if(*nowTimerQueuePos_==timerQueue_->index())
		return;
	*nowTimerQueuePos_=timerQueue_->index();

	struct timeval now;
	gettimeofday(&now,NULL);

	*latestRefreshTime_=now.tv_sec*1000*1000+now.tv_usec;

	timerQueue_->push(*this);
}

/*static void Timer::handleExpireEvent()
{
	timerQueue_->handleExpireTimer();
}*/
int64_t Timer::getInternalTime()
{
	int64_t tim=*latestRefreshTime_;
	return tim;
}
Timer::~Timer()
{
	std::cout<<"yueyou******************************************************************"<<std::endl;
/*	struct timeval now;

	gettimeofday(&now,NULL);

	int64_t timeNow=now.tv_sec*1000*1000+now.tv_usec;
*/
//	if((*latestRefreshTime_)/1000/1000+timeoutSec_<=timerQueue_->time()/1000/1000)
	if(*nowTimerQueuePos_-1==timerQueue_->index())
	{
		std::cout<<"timer ###################################################################### expired\t"<<__FILE__<<":"<<__LINE__<<std::endl;
		if(httpRequest_->isValid())	
		httpRequest_->handleClose();
		delete latestRefreshTime_;
		delete nowTimerQueuePos_;
	}	
}

TimerQueue::TimerQueue(EventLoop *loop,int timerQueueSize)
	:timerQueue_(timerQueueSize,std::vector<Timer>()),
	 TimerQueueSize(timerQueueSize),
	 loop_(loop),
	 nowTimerQueuePos_(0),
	 channel_(NULL)
{
	int tfd=timerfd_create(CLOCK_REALTIME,0);

        struct itimerspec newvalue;

        struct timespec now;

        clock_gettime(CLOCK_REALTIME,&now);

        newvalue.it_value.tv_sec=now.tv_sec+3;
        newvalue.it_value.tv_nsec=now.tv_nsec;

        newvalue.it_interval.tv_sec=0;

        newvalue.it_interval.tv_nsec=100*1000*1000;

        timerfd_settime(tfd,TFD_TIMER_ABSTIME,&newvalue,NULL);
	
	channel_=new Channel(loop_,tfd);

	channel_->setReadCallback(std::bind(&TimerQueue::handleExpireTimer,this));

	channel_->enableReading();
	
	refreshTime();

	std::cout<<"******************************************************"<<std::endl;

}



TimerQueue::~TimerQueue()
{
}


void TimerQueue::handleExpireTimer()
{

	if(channel_==NULL)
		return ;
	int fd=channel_->fd();
	int buff[10];
	read(fd,buff,10); //prevent the timer to be triggered immediately again

	refreshTime();

	std::lock_guard<std::mutex> lock(Mutex_);
	timerQueue_[nowTimerQueuePos_].clear();


	nowTimerQueuePos_++;
	if(nowTimerQueuePos_>=TimerQueueSize)
		nowTimerQueuePos_=0;
	std::cout<<"handle Expire Timer\t\t"<<nowTimerQueuePos_<<std::endl;
	
}

void TimerQueue::refreshTime()
{
	struct timeval now;

	gettimeofday(&now,NULL);

	nowTime_=now.tv_sec*1000*1000+now.tv_usec;	
}

void TimerQueue::push(Timer &tim)
{	
	std::lock_guard<std::mutex> lock(Mutex_);

	int pos=nowTimerQueuePos_-1;
	if(pos<0)			//queue behind nowTimerQueuePos_
		pos=TimerQueueSize-1;
	timerQueue_[pos].push_back(tim);
	std::cout<<"NowTimerQueuePos\t\t"<<nowTimerQueuePos_<<std::endl;
}



Timestamp::Timestamp()
	:microSecondsSinceEpoch_(0)
{
}

Timestamp::Timestamp(int64_t microSecondsSinceEpoch)
	:microSecondsSinceEpoch_(microSecondsSinceEpoch)
{
}

std::string Timestamp::toString()
{
	char buf[64]={0};
	time_t seconds=static_cast<time_t>(microSecondsSinceEpoch_/kMicroSecondsPerSecond);
	
	struct tm *tmp;
	
	tmp=localtime(&seconds);
	
	snprintf(buf,sizeof(buf),"%4d/%02d/%02d %02d:%02d:%02d",tmp->tm_year+1900,tmp->tm_mon+1,tmp->tm_mday,tmp->tm_hour,tmp->tm_min,tmp->tm_sec);
	
	return buf;
	
}
Timestamp Timestamp::now()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	int64_t seconds=tv.tv_sec;
	return Timestamp(seconds*kMicroSecondsPerSecond+tv.tv_usec);	
}
