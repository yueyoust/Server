#include "Timer.h"
#include <time.h>
#include <stdio.h>





















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
