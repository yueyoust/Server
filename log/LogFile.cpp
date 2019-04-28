#include "LogFile.h"
#include<iostream>

LogFile::LogFile(const std::string &baseName,int flushEveryN)
	:baseName_(baseName),
	 flushEveryN_(flushEveryN),
	 count_(0),
	 mutex_(new std::mutex)
{
	//std::cout<<baseName<<"filename";
	file_.reset(new AppendFile(baseName));
}

LogFile::~LogFile()
{
}

void LogFile::append(const char *logline,int len)
{
	std::lock_guard<std::mutex> lock(*mutex_);
	append_unlocked(logline,len);
}

void LogFile::flush()
{
	std::lock_guard<std::mutex> lock(*mutex_);
	file_->flush();
}
void LogFile::append_unlocked(const char *logline,int len)
{
	//for(int i=0;i<len;i++)
	//std::cout<<logline[i];
	file_->append(logline,len);
	++count_;
	if(count_>flushEveryN_)
	{
		count_=0;
		file_->flush();
	}
}
