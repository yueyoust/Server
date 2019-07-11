#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
int main()
{
	int fd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in ser;
	ser.sin_family=AF_INET;
	ser.sin_port=htons(8080);
	ser.sin_addr.s_addr=inet_addr("127.0.0.1");
	int flag=connect(fd,(struct sockaddr*)&ser,sizeof(ser));	
	
	if(flag==0)
	std::cout<<"success"<<std::endl;
	char buffer[4096]="sunday morning";
	write(fd,buffer,3);
	read(fd,buffer,4096);
	close(fd);
	std::cout<<buffer<<std::endl;
	return 0;
}
