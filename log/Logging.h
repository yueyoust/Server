#ifndef _LOGGING_H_
#define _LOGGING_H_
#include <stdio.h>
#include <assert.h>
#include "LogStream.h"
#include "AsyncLogging.h"

class AsyncLogging;

class Logger
{
public:	
	Logger(const char *fileName,int line);
	~Logger();
	LogStream& stream(){return impl_.stream_;}

	static void setLogFileName(std::string fileName)
	{
		LogFileName_=fileName;
	}
	static std::string getLogFileName()
	{
		return LogFileName_;
	}
private:
	class Impl{
	public:
		Impl(const char *filename,int line);
		void formatTime();
		
		LogStream stream_;
		int line_;
		std::string basename_;
	};
	Impl impl_;
	static std::string LogFileName_;
};


#define LOG Logger(__FILE__,__LINE__).stream()
















#endif
