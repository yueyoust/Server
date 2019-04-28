#ifndef _FILEUTIL_H_
#define _FILEUTIL_H_
#include <string>
class AppendFile{
public:
	explicit AppendFile(std::string fileName);

	~AppendFile();

	void append(const char *logline ,const size_t len);
	void flush();

private:

	size_t write(const char *logline,size_t len);
	FILE *fp_;
	char buffer_[64*1024];
};


#endif
