#ifndef _TIMER_H_
#define _TIMER_H_
#include <string>
#include <sys/time.h>
#include <vector>
#include "httpMes.h"

class httpMes;

class Timer{
	
public
	Timer();
	
	Timer(std::shared_ptr<httpMes> request,int64_t timeout);
	
	void refresh();
	~Timer();
private:
		
	int64_t *latestRefreshTime_;
	
	std::shared_ptr<httpMes> httpRequest_;
	static TimerQueue timerQueue
};


class TimerQueue
{
public:
	TimerQueue();
	
	handleExpireTimer();

	void push();
private:
	const static int TimerQueueSize=100;
	typedef std::vector<Timer> TimeQueue;
	
	TimeQueue timerQueue_[TimerQueueSize];
	
	int nowTimerPos_;
}














class Timestamp{
public:


	Timestamp();
	
 	Timestamp(int64_t microSecondsSinceEpoch);
	std::string toString();
	
	static Timestamp now();
private:
	
	const static int kMicroSecondsPerSecond=1000*1000;
	
	int64_t microSecondsSinceEpoch_;
};
















#endif
