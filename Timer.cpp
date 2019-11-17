#include "Timer.h"
#include <time.h>
#include <stdio.h>

Timer::Time()
{
}

Timer::Timer(std::shared<httpMes> request,int64_t timeoutMs)
	:httpRequest_(request),
	 latestRefreshTime_(new int(0))
{
	struct timeval now;
	gettimeofday(&now,NULL);
	
	*latestRefreshTime_=now.tv_sec*1000*1000+now.tv_usec;
}

Timer::~Timer()
{
	struct timeval now;
	gettimeofday(&now,NULL);

	int64_t timeNow=now.tv_sec*1000*1000+now.tv_usec;
	if(latestRefreshTime+timeoutMs>timeNow)
	{
		cout<<"timer expired"<<std::endl;
	}	

}




TimeQueue::TimeQueue()

{}


















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
