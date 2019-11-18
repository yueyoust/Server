#include "Timer.h"
#include <time.h>
#include <stdio.h>

Timer::Timer()
{
;
}

Timer::Timer(std::shared_ptr<httpMes> request,int64_t timeoutSec=20)
	:httpRequest_(request),
	 timeoutSec_(timeoutSec),
	 latestRefreshTime_(new int64_t(0))
	// timerQueue_(static_cast<int>10*timeoutSec)
{
	if(timerQueue_==NULL)
	{
		timerQueue_= new TimerQueue(20);;//(static_cast<int>10*timeoutSec);
	}
	struct timeval now;

	gettimeofday(&now,NULL);
	
	*latestRefreshTime_=now.tv_sec*1000*1000+now.tv_usec;
}

void Timer::refresh()
{
	struct timeval now;

	gettimeofday(&now,NULL);
	
	*latestRefreshTime_=now.tv_sec*1000*1000+now.tv_usec;

	timerQueue_->push(*this);
}

int64_t Timer::getInternalTime()
{
	int64_t tim=*latestRefreshTime_;
	return tim;
}
Timer::~Timer()
{
	struct timeval now;

	gettimeofday(&now,NULL);

	int64_t timeNow=now.tv_sec*1000*1000+now.tv_usec;

	if(*latestRefreshTime_/1000/1000+timeoutSec_<=timeNow)
	{
		std::cout<<"timer expired"<<std::endl;
		delete latestRefreshTime_;
	}	

}

TimerQueue::TimerQueue(int timerQueueSize)
	:timerQueue_(timerQueueSize,std::vector<Timer>()),
	 TimerQueueSize(timerQueueSize),
	 nowTimerQueuePos_(0),
	 Mutex_()
{
	
}



TimerQueue::~TimerQueue()
{
}


void TimerQueue::handleExpireTimer()
{
	timerQueue_[nowTimerQueuePos_].clear();
	std::lock_guard<std::mutex> lock(Mutex_);

	nowTimerQueuePos_++;

	if(nowTimerQueuePos_>=TimerQueueSize)
	nowTimerQueuePos_++;
}


void TimerQueue::push(Timer &tim)
{	
	std::lock_guard<std::mutex> lock(Mutex_);
	
	int pos=nowTimerQueuePos_-1;
	if(pos<0)			//queue behind nowTimerQueuePos_
		pos=TimerQueueSize-1;
	timerQueue_[pos].push_back(tim);
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
