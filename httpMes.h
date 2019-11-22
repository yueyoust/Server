#ifndef _HTTPMES_H_
#define _HTTPMES_H_
#include <string>
#include <unordered_map>
#include <memory>
#include <unistd.h>
#include "EventLoop.h"
#include "Channel.h"
#include "Timer.h"

enum processState{
	STATE_REQUEST_LINE=1,
	STATE_REQUEST_HEADER,
	STATE_REQUEST_BODY,
	STATE_REQUEST_COMPLETED
};

class Timer;
class TimerQueue;
class httpMes
{
public:
	httpMes(EventLoop *loop,TimerQueue *timerQueue,int connfd);

	~httpMes() {close(fd_);}
	
	bool isValid(){return connectionState_;}

	void setHttpConnectionState(bool state){connectionState_=state;}

	void handleClose();	

private:
	const static int BufferSize=4096;	

	EventLoop *loop_;

	int fd_;
	
	bool connectionState_;
	processState state_;

	std::string unparsedStr;
	
	std::map<std::string,std::string> httpHeaders_;	
	
	std::string httpRequestBody_;

	std::shared_ptr<Channel> channel_;

	int  posBuffer_; 
	
	char fBuffer_[BufferSize];	

	std::mutex Mutex_;

	TimerQueue *timerQueue_;	

	Timer *timer_;	

	void handleRead();
	
	void handleWrite();
	
	void handleConn();


	processState parseRequestLine(std::istringstream &requestLine);
	processState parseRequestHeader(std::string &hstr);
	processState parseRequestBody();
	
	void onResponse();
};

#endif
