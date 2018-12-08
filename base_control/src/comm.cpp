#include <vector>
#include "comm.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <ros/ros.h>

uint8_t ComposePackatFromMatrix(
    const float *matrix, uint8_t row, uint8_t column, uint8_t id,
    uint8_t *packet)
{
  // signature  : 1byte
  // size       : 2byte
  // id         : 1byte
  // row        : 1byte
  // column     : 1byte
  // data       : (row * column) * 4byte
  uint16_t size = 6 + sizeof(float) * row * column;
  int offset = 0;

  // Write header
  packet[0] = 0xFF;
  packet[1] = (size >> 0) & 0xFF;
  packet[2] = (size >> 8) & 0xFF;
  packet[3] = id;
  packet[4] = row;
  packet[5] = column;
  offset += 6;

  // Write data
  for (int i = 0; i < row * column; i++)
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
}

int rawSerialport::tryReadMsg(char *character)
{
  if (isOpen)
    return read(fd, character, 1);
}

int rawSerialport::WriteMsg(char *data, size_t size)
{
  if (isOpen)
    return write(fd, data, size);
}

rawSerialport::rawSerialport(const char *device)
{
  isOpen = false;
  speed_t baudRate = B115200;
  //Device Open
  while ((fd = open(device, O_RDWR | O_NOCTTY)) < 0)
  {
    ROS_WARN("Open error : %s not found\n", device);
    ErrNo = fd;
    isErr = true;
    sleep(5U);
  }
  tcgetattr(fd, &tio_old);

  memset(&tio, 0, sizeof(tio));
  tio.c_iflag = 0;
  tio.c_oflag = 0;
  tio.c_cflag = CS8 | CREAD | CLOCAL;
  tio.c_lflag = 0;
  tio.c_cc[VMIN] = 1;
  tio.c_cc[VTIME] = 5;

  cfsetispeed(&tio, baudRate);
  cfsetospeed(&tio, baudRate);

  //tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &tio); // デバイスに設定を行う
}

rawSerialport::~rawSerialport()
{
  if (!isErr)
  {
    tcsetattr(fd, TCSANOW, &tio_old);
    close(fd);
  }
}