#include <iostream>
#include <fstream>
#include "AsyncLogging.h"


AsyncLogging::AsyncLogging(std::string logFileName,int flushInterval)
	:flushInterval_(flushInterval),
	 baseName_(logFileName),
	 thread_(std::bind(&AsyncLogging::threadFunc,this)),
	 mutex_(),
//	 uniqueMutex_(mutex_),
//	 cond_(),
	 currentBuffer_(new Buffer),
	 nextBuffer_(new Buffer),
	 buffers_()
{	
	//std::cout<<baseName_;
	std::cout<<"init AsyncLogging class"<<std::endl;
	currentBuffer_->bzero();
	nextBuffer_->bzero();
	buffers_.reserve(16);
} 


void AsyncLogging::append(const char *logline,int len)
{
	std::lock_guard<std::mutex>lock(mutex_);
	//std::cout<<"AsyncLogging append message"<<std::endl;
	if(currentBuffer_->avail()>len)
		currentBuffer_->append(logline,len);
	else
	{
		buffers_.push_back(currentBuffer_);
		currentBuffer_.reset();
		if(nextBuffer_)
		{
			currentBuffer_=std::move(nextBuffer_);
		}
		else
		{
			currentBuffer_.reset(new Buffer);
		}
		currentBuffer_->append(logline,len);
	}
}


void AsyncLogging::threadFunc()
{
	std::ofstream file("text.log");
	LogFile output(baseName_);
	file<<"yueyou";
	//file.close();
	BufferPtr newBuffer1(new Buffer);
	BufferPtr newBuffer2(new Buffer);
	newBuffer2->bzero();	
	newBuffer1->bzero();
	//std::cout<<"thread function\n"<<std::endl;
	BufferVector buffersToWrite;
	buffersToWrite.reserve(16);
	while(true)
	{
	
		{	
			std::lock_guard<std::mutex> lock(mutex_);	
	
			buffers_.push_back(currentBuffer_);
			currentBuffer_.reset();
	
			currentBuffer_=std::move(newBuffer1);
			buffersToWrite.swap(buffers_);
			if(!nextBuffer_)
			{
				nextBuffer_=std::move(newBuffer2);
			}
		}	
		
		for(auto const &str:buffersToWrite)
		{
			output.append(str->data(),str->length());
			for(int i=0;i<str->length();i++)
			{
				file<<str->data()[i];
			}
			file<<std::flush;
		}

		if(buffersToWrite.size()>2)
		{
			buffersToWrite.resize(2);
		}

		if(!newBuffer1)
		{
			newBuffer1=buffersToWrite.back();
			buffersToWrite.pop_back();
			newBuffer1->reset();
		}
		if(!newBuffer2)
		{
			newBuffer2=buffersToWrite.back();
			buffersToWrite.pop_back();
			newBuffer2->reset();
		}
		buffersToWrite.clear();
		output.flush();
	}
	output.flush();
}











