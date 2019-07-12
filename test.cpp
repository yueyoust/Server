#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

int main()
{
	
	int tfd=socket_bind_listen(8080);

	struct sockaddr_in client_addr;

	//memset(&client_addr,0, sizeof(struct sockaddr_in));

	socklen_t client_addr_len =sizeof(client_addr);

	int accept_fd;
	
	std::vector<struct pollfd> vecfd;
	

	accept_fd=accept(tfd,(struct sockaddr*) &client_addr,&client_addr_len);
	char tbuffer[4096];
	int nu=read(accept_fd,tbuffer,4096);
	std::cout<<nu<<'\t'<<tbuffer<<std::endl;
	char text[]="HTTP/1.0 404 Not Found\r\nContent-Type: text/html \r\n\r\n<HTML><HEAD><TITLE>Not Found lukas</TITLE></HEAD><BODY>Not Found</BODY></HTML>Sending file not found.\r\n\r\n";	
	std::string header_buff,body_buff; 
	body_buff += "<html><title>not</title>";
    	body_buff += "<body bgcolor=\"00ffdd\">";
    	body_buff += "404 Not Found";
    	body_buff += "<hr><em> Yueyou's Web Server</em>\n</body></html>";
	
	header_buff += "HTTP/1.1 " + std::to_string(404) + "Not Found" + "\r\n";
    	header_buff += "Content-Type: text/html\r\n";
    	header_buff += "Connection: Close\r\n";
    	header_buff += "Content-Length: " + std::to_string(body_buff.size()) + "\r\n";
    	header_buff += "Server: yueyou's Web Server\r\n";
    	header_buff += "\r\n";
	
	write(accept_fd,header_buff.c_str(),header_buff.size());
	write(accept_fd,body_buff.c_str(),body_buff.size());

	struct pollfd pf;
	pf.fd=accept_fd;

	pf.events=POLLIN;

	vecfd.push_back(pf);
		
	while(1)
	{
		int l=::poll(&*vecfd.begin(),vecfd.size(),1000);
		char buffer[4096];
		if(l>0)
		{	
			std::cout<<"something\t"<<vecfd.size()<<std::endl;
			for(auto i: vecfd)
			{
				
				if(i.revents>0)
					{
						if(i.revents&(POLLIN))
						std::cout<<"POLLIN"<<std::endl;
						if(i.revents&(POLLERR))
						std::cout<<"POLLERR"<<std::endl;
						if(i.revents&(POLLRDNORM))
						std::cout<<"POLLRDNORM"<<std::endl;
						if(i.revents&(POLLRDBAND))
						std::cout<<"POLLRDBAND"<<std::endl;
						if(i.revents&(POLLWRNORM))
						std::cout<<"POLLWRNORM"<<std::endl;
						if(i.revents&(POLLWRBAND))
						std::cout<<"POLLWRBAND"<<std::endl;
						if(i.revents&(POLLNVAL))
						std::cout<<"POLLNVAL"<<std::endl;
						if(i.revents&(POLLHUP))
						std::cout<<"POLLHUP"<<std::endl;

						int num=read(i.fd,buffer,4096);
						std::cout<<"num of word has been read\t"<<num<<std::endl;
						if(i.revents&(POLLOUT))
						std::cout<<"POLLOUT"<<std::endl;

						//write(i.fd,text,strlen(text));

					}
			}
		}
		std::cout<<"\\num of Eventloop queue in poll\t"<<vecfd.size()<<std::endl;
	}
	
}
