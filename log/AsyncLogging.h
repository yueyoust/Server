#ifndef _ASYNCLOGGING_H_
#define _ASYNCLOGGING_H_

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "Logging.h"



class AsyncLogging{
public:
	AsyncLogging(const std::string baseName,int flushInterval=2);
	~AsyncLogging()
	{
	}


	void append(const char *logline,int len);


private:
	void threadFunc();

	typedef FixedBuffer<kLargeBuffer> Buffer;

	typedef std::vector<std::shared_ptr<Buffer>> BufferVector;

	typedef std::shared_ptr<Buffer> BufferPtr;

	const int flushInterval_;
	std::string baseName_;
	std::thread thread_;
	

	std::mutex mutex_;	
	std::unique_lock<std::mutex> uniqueMutex_;
	std::condition_variable cond_;

	BufferPtr currentBuffer_;
	BufferPtr nextBuffer_;
	BufferVector buffers_;
	

};











#endif
