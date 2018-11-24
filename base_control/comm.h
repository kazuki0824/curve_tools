#ifndef _COMM_
#define _COMM_

#include <termios.h>

#define MD_Msg_Size 22 //TODO: ここをMDに合わせる

class rawSerialport
{
private:
    int fd = -1;
    struct termios tio_old, tio;
	char buf[256];
public:
    rawSerialport(const char* deviceName);
    ~rawSerialport();
    
    int WriteMsg(char * data, size_t size);
    int tryReadMsg(char character[MD_Msg_Size]);
    bool isErr;
    int ErrNo;
};

#endif