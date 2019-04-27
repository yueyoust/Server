#include <time.h>
#include <sys/time.h>
#include <iostream>
#include "Logging.h"

static AsyncLogging *AsyncLogger;

Logger::Impl::Impl(const char *fileName,int line)
	:stream_(),
	 line_(line),
	 basename_(fileName)
{
	formatTime();
}
void Logger::Impl::formatTime()
{
	struct timeval tv;
	time_t time;
	char str[26]={0};
	gettimeofday(&tv,NULL);
	time=tv.tv_sec;
	struct tm *p_time=localtime(&time);
	strftime(str,26,"%Y-%m-%d %H:%M:%S\n",p_time);
	
	stream_<<str;
}



Logger::Logger(const char *fileName,int line)
	:impl_(fileName,line)
{
}

Logger::~Logger()
{
	impl_.stream_ << " -- " << impl_.basename_ << ':' << impl_.line_ << '\n';
    	const LogStream::Buffer& buf(stream().buffer());
	
	AsyncLogger->append(buf.data(),buf.length());
	for(int i=0;i<buf.length();i++)
	{
		std::cout<<buf.data()[i];
	}
//	std::cout<<std::endl;
}