#ifndef _COMM_
#define _COMM_

#include <termios.h>

class rawSerialport
{
private:
    int fd = -1;
    struct termios tio_old, tio;
	char buf[256];
public:
    rawSerialport(char* deviceName);
    ~rawSerialport();
    
    int readChar(char * character)
    bool isErr;
    int ErrNo;
};

#endif