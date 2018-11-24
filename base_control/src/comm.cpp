#include <vector>
#include "comm.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

uint8_t ComposePackatFromMatrix(
  const float* matrix, uint8_t row, uint8_t column, uint8_t id,
  uint8_t* packet)
{
  // signature  : 1byte
  // size       : 2byte
  // id         : 1byte
  // row        : 1byte
  // column     : 1byte
  // data       : (row * column) * 4byte
  uint16_t size = 6 + sizeof(float) * row * column;
  int offset = 0;

#ifdef USE_SPRINTF

  sprintf((char*)packet, "%f,%f,%f\n", matrix[0], matrix[1], matrix[2]);
  return strlen((char*)packet);

#else

  // Write header
  packet[0] = 0xFF;
  packet[1] = (size >> 0) & 0xFF;
  packet[2] = (size >> 8) & 0xFF;
  packet[3] = id;
  packet[4] = row;
  packet[5] = column;
  offset += 6;

  // Write data
  for(int i = 0; i < row * column; i++)
  {
    union {
      float real;
      uint8_t base[4];
    } u_data;
    u_data.real = packet[i];
    packet[offset + 0] = u_data.base[0];
    packet[offset + 1] = u_data.base[1];
    packet[offset + 2] = u_data.base[2];
    packet[offset + 3] = u_data.base[3];
    offset += sizeof(packet[i]);
  }
  return offset;
#endif
}

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