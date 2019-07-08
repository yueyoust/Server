#ifndef _CHANNEL_H_
#define _CHANNEL_H_
#include<vector>
#include<functional>
#include"EventLoop.h"
#include<iostream>
class EventLoop;
class Channel{
public:
	typedef std::function<void()> EventCallBack;
	Channel(EventLoop *loop,int fd);

	void handleEvent();

	void setReadCallback(EventCallBack callback){readCallBack=callback;}

	void setWriteCallback(EventCallBack &callback){writeCallBack=callback;}

	void enableReading(){events_|=kreadEvent; update();}

	void enableWriting(){events_|=kwriteEvent; update();}
	
	int fd(){return fd_;}
	
	//void setFd(int fd);
	
	int events(){return events_;}

	void set_revents(int revt){revents_=revt;}


private:
	static const int kreadEvent;
	static const int kwriteEvent;
	
	void update();
	int events_;
	int revents_;
	const int fd_;
	const EventLoop *loop;
	
	EventCallBack readCallBack;
	EventCallBack writeCallBack;
};



#endif
