#ifndef _MYTIMER_H_
#define _MYTIMER_H_
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
	
	Timer(httpMes *request,TimerQueue *tmq,int64_t timeoutSec);
	
	void refresh();

	int64_t getInternalTime();
	
	//static void handleExpireEvent();
	~Timer();
private:
		
	int64_t *latestRefreshTime_;//to prevent the same timer being pushed into same slot more than twice
	
	int64_t timeoutSec_;	

	httpMes *httpRequest_;

	TimerQueue *timerQueue_;
	
	int *nowTimerQueuePos_;

};


class TimerQueue
{
public:
	TimerQueue(EventLoop *loop,int TimerQueueSize);
	
	~TimerQueue();
	
	int index(){return nowTimerQueuePos_;}

	int64_t time(){return nowTime_;}

	void push(Timer &tim);

	std::mutex Mutex_;

private:
	int TimerQueueSize;

	int64_t nowTime_;
	
	typedef std::vector<std::vector<Timer>> TimeQueue;
	
	TimeQueue timerQueue_;
	
	EventLoop *loop_;
	
	Channel *channel_;

	int nowTimerQueuePos_;

	void handleExpireTimer();	//100ms per slot;

	void refreshTime();
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
