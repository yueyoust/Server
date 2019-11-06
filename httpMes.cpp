#include "httpMes.h"
#include "Server.h"
httpMes::httpMes(EventLoop *loop,int connfd)
	:loop_(loop),
	 fd_(connfd),
	 channel_(new Channel(loop,connfd))
{
	channel_->setReadCallback(std::bind(&httpMes::handleRead,this));
	//channel_->setWriteCallback(std::bind(&httpMes::handleWrite,this));
	channel_->enableReading();
}

void httpMes::handleRead()
{
	/*
	_uint32_t &events=channel_->events();
	do
	{
		
	}while(false);*/
	int fd =fd_;
	char fbuffer[4096];
	int num=read(fd,fbuffer,4096);
	
	std::cout<<"\nfile descriptor\t"<<fd<<"\tbuffersize\t"<<num<<"\n\n"<<fbuffer<<std::this_thread::get_id()<<std::endl;
	if(num==0)
	{
		channel_->remove();
		std::cout<<"channel has been removed"<<std::endl;
		return ;
	}
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
}

