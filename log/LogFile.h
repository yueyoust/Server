#ifndef _LOGFILE_H_
#define _LOGFILE_H_
#include <string>
#include <mutex>
#include <memory>
#include "FileUtil.h"
class LogFile{
public:
	LogFile(const std::string &baseName,int flushEveryN=1024);
	~LogFile();
	void append(const char *logline,int len);
	void flush();

private:
	void append_unlocked(const char *logline,int len);

	const std::string baseName_;
	
	const int flushEveryN_;

	int count_;
	
	std::unique_ptr<std::mutex> mutex_;
	
	std::unique_ptr<AppendFile>file_;

};

#endif
