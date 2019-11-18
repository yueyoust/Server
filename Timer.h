#ifndef __TIMER_H_
#define __TIMER_H_
#include <string>
#include <sys/time.h>
#include <vector>
#include <mutex>
#include "httpMes.h"



class httpMes;

class TimerQueue;

class Timer{
	
public:
	Timer();
	
	Timer(std::shared_ptr<httpMes> request,int64_t timeoutSec);
	
	void refresh();

	int64_t getInternalTime();
	
	void handleExpireEvent();

	~Timer();
private:
		
	int64_t *latestRefreshTime_;
	
	int64_t timeoutSec_;	

	std::shared_ptr<httpMes> httpRequest_;

	TimerQueue *timerQueue_;
};


class TimerQueue
{
public:
	TimerQueue(int TimerQueueSize);
	
	~TimerQueue();
	void handleExpireTimer();//100ms per slot;

	void push(Timer &tim);

private:
	int TimerQueueSize;
	
	static std::mutex Mutex_;

	typedef std::vector<std::vector<Timer>> TimeQueue;
	
	TimeQueue timerQueue_;
	
	int nowTimerQueuePos_;
};

//std::mutex TimerQueue::Mutex_;


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
