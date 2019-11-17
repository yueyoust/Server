#ifndef _TIMER_H_
#define _TIMER_H_
#include <string>
#include <sys/time.h>


class Timer{
	
	Timer();

};




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
