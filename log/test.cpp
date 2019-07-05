#include<iostream>
#include"Logging.h"
#include<poll.h>

int main()
{
//	FixedBuffer<4096> test;
//	test.append("yueyou",6);
//	test.append("hello world",11);
//	std::cout<<test.data()<<std::endl;
	int i=1000000;
	while(i--)
	{
		LOG<<"yueyou, this is your fisrt log file ,youmust learn to maintain it and make it better\t"<<i;

		LOG<<"hello world\t"<<i;
	}
	::poll(NULL,NULL,2000);
//	while(1);
	return 0;
}
