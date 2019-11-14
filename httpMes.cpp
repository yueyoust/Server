#include <sstream>
#include "httpMes.h"
#include "Server.h"
httpMes::httpMes(EventLoop *loop,int connfd)
	:loop_(loop),
	 fd_(connfd),
	 channel_(new Channel(loop,connfd)),
	 state_(STATE_REQUEST_LINE),
	 posBuffer_(0)
{
	channel_->setReadCallback(std::bind(&httpMes::handleRead,this));
	//channel_->setWriteCallback(std::bind(&httpMes::handleWrite,this));
	channel_->enableReading();
}

void httpMes::handleRead()
{

	int fd =fd_;	
	int num=read(fd,fBuffer_,4096);
	if(num==0)
	{
		channel_->remove();
		std::cout<<"channel has been removed"<<std::endl;
		return ;
	}
	
	if(state_==STATE_REQUEST_LINE)
	{
		for(int i=1;i<num;i++)
		{
			if(fBuffer_[i-1]=='\r'&&fBuffer_[i]=='\n')
			{
				posBuffer_=i+1;
				std::istringstream requestLine(std::string(fBuffer_,fBuffer_+i-1));
				state_=parseRequestLine(requestLine);
				break;
			}
		}
	}
	if(state_==STATE_REQUEST_HEADER)
	{
	//	while(state_=STATE_REQUEST_HEADER)
		{
			for(int i=posBuffer_;i<num;i++)
			{
				if(fBuffer_[i-1]=='\r'&&fBuffer_[i]=='\n')
				{
					std::string hstr(fBuffer_+posBuffer_,fBuffer_+i-1);
					std::cout<<"\n***********************-------------------\t"<<i<<"\t--------------------**************************"<<std::endl;
					state_=parseRequestHeader(hstr);
					posBuffer_=i+1;
				}
			}
			for(auto &i:httpHeaders_)
			std::cout<<i.first<<"\t\t\t\t\t\t"<<i.second<<'\t'<<i.second.size()<<std::endl;

		}
	}
	if(state_==STATE_REQUEST_BODY)
	{
		state_=parseRequestBody();
		std::cout<<"the state of parse http has been change from STATE_REQUEST_HEADER to STATE_REQUEST_BODY"<<std::endl;
		
	}
	if(state_==STATE_REQUEST_COMPLETED)
	{
		state_=STATE_REQUEST_LINE;	
		std::cout<<"\nfile descriptor\t"<<fd<<"\tbuffersize\t"<<num<<"\n\n"<<fBuffer_<<std::this_thread::get_id()<<std::endl;

		std::string header_buff,body_buff; 
		body_buff += "<html><title>not</title>";
	    	body_buff += "<body bgcolor=\"ffffdd\">";
    		body_buff += "404 Not Found";
    		body_buff += "<hr><em> Yueyou's Web Server</em>\n</body></html>";
	
		header_buff += "HTTP/1.1 " + std::to_string(404) + "Not Found" + "\r\n";
    		header_buff += "Content-Type: text/html\r\n";
    		header_buff += "Connection: Close\r\n";
    		header_buff += "Content-Length: " + std::to_string(body_buff.size()) + "\r\n";
    		header_buff += "Server: yueyou's Web Server\r\n";
    		header_buff += "\r\n";
	
		write(fd,header_buff.c_str(),header_buff.size());
		write(fd,body_buff.c_str(),body_buff.size());
		
		httpHeaders_.clear();
	}	
}

processState httpMes::parseRequestLine(std::istringstream &requestLine)
{
	std::string requestMethod,httpURL,httpVersion;
	requestLine>>requestMethod;
	requestLine>>httpURL;
	requestLine>>httpVersion;
	std::cout<<"\nHTTP REQUEST METHOD\t"<<requestMethod<<'\n'<<"HTTP URL\t\t"<<httpURL<<'\n'<<"HTTP VERSION\t\t"<<httpVersion<<std::endl;
	
	return STATE_REQUEST_HEADER;
}

processState httpMes::parseRequestHeader(std::string &hstr)
{
	if(hstr.empty())
		return STATE_REQUEST_BODY;
	//std::map<std::string,std::string> httpHeIader;
	std::string contenType,value;
	for(int i=0;i<hstr.size();i++)
	{
		if(hstr[i]==':')
			{
				contenType=hstr.substr(0,i);
				i+=1;
				while(hstr[i]==' ')i++;// delete space;
				value=hstr.substr(i);
				break;
			}
	}
	while(!contenType.empty()&&contenType.back()==' ') contenType.pop_back();
	while(!value.empty()&&value.back()==' ')value.pop_back();// delete space;
	
	
	httpHeaders_[std::move(contenType)]=std::move(value);	
	
	return STATE_REQUEST_HEADER;
}
processState httpMes::parseRequestBody()
{
	if(httpHeaders_.count("Content-Length"))
	{
		//for(int i=po
	}	
	return STATE_REQUEST_COMPLETED;	
}
void httpMes::onResponse()
{
	
}
