#include <vector>
#include "comm.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int rawSerialport::tryReadMsg(char character[MD_Msg_Size])
{
    return read(fd, character, MD_Msg_Size);
}

int rawSerialport::WriteMsg(char * data, size_t size)
{
	return write(fd, data, size);
}

rawSerialport::rawSerialport(const char* device)
{
    speed_t baudRate = B115200;
//Device Open
    fd = open(device, O_RDWR | O_NOCTTY);
	if(fd < 0){
        printf("Open error : %s \n", device);
        ErrNo = fd; isErr =true;
	}

    cfmakeraw(&tio);                    // RAWモード
    tcgetattr(fd, &tio_old);
    tio.c_cflag += CREAD;               // 受信有効
    tio.c_cflag += CLOCAL;              // ローカルライン（モデム制御なし）
    tio.c_cflag += CS8;                 // データビット:8bit
    tio.c_cflag += 0;                   // ストップビット:1bit
    tio.c_cflag += 0;                   // パリティ:None

    cfsetispeed( &tio, baudRate );
    cfsetospeed( &tio, baudRate );
    tio.c_cc[VTIME] = 1;
    tio.c_cc[VMIN] = MD_Msg_Size;

    //tcflush(fd, TCIFLUSH);
    tcsetattr( fd, TCSANOW, &tio );     // デバイスに設定を行う

}

rawSerialport::~rawSerialport()
{
	tcsetattr(fd, TCSANOW, &tio_old);
	close(fd);
}