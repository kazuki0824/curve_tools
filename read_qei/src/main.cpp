#include "mainwindow.h"
#include <QApplication>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>


void* th_rx(void* pParam); //受信用スレッド

static bool isROS =false;
ros::Publisher encoder_pub;

int tty_fd = -1;

unsigned char c = 'D';

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int main(int argc, char** argv){
    struct termios tio;
    struct termios stdio;
    struct termios old_stdio;
    char buf[256];
	
    speed_t baudRate = B115200;
	  pthread_t  tid_rx; // Thread IDs
    char default_device[256] = "/dev/ttyACM0";
    char * device = NULL;

    tcgetattr(STDOUT_FILENO, &old_stdio);
	if(argc < 2)
	{
        printf("Open warning  : A device should be specified. Assume '/dev/ttyACM0'\n");
        device = default_device;
	}
  else
  {
      device = argv[1];
  }

    memset(&stdio, 0, sizeof(stdio));
    stdio.c_iflag = 0;
    stdio.c_oflag = 0;
    stdio.c_cflag = 0;
    stdio.c_lflag = ISIG;
    stdio.c_cc[VMIN] = 1;
    stdio.c_cc[VTIME] = 0;
    stdio.c_cc[VINTR] = 0x03;

    tcsetattr(STDOUT_FILENO, TCSANOW, &stdio);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    memset(&tio, 0, sizeof(tio));
    tio.c_iflag = 0;
    tio.c_oflag = 0;
    tio.c_cflag = CS8 | CREAD | CLOCAL;
    tio.c_lflag = 0;
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 5;

//Device Open
    tty_fd = open(device, O_RDWR | O_NONBLOCK);
	if(tty_fd < 0){
        printf("Open error : %s \n", device);
        tcsetattr(STDOUT_FILENO, TCSANOW, &old_stdio);
        return tty_fd;
	}

    cfsetispeed( &tio, baudRate );
    cfsetospeed( &tio, baudRate );

    //tcflush(tty_fd, TCIFLUSH);
    tcsetattr( tty_fd, TCSANOW, &tio );     // デバイスに設定を行う


//Create threads
	pthread_create(&tid_rx, NULL, th_rx, NULL);

//User check
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    int ui_exitcode = app.exec();
    //return app.exec();

//ROS Init
	ros::init(argc, argv, "read_qei");
	ros::NodeHandle nh("~");
    encoder_pub = nh.advertise<geometry_msgs::Twist>("robot_encoder", 100);
    isROS = true;

//Wait for threads end
    struct timespec ts;
    ts.tv_nsec += 1000000;
    do
    {
        ros::spinOnce();
    }while(pthread_timedjoin_np(tid_rx, NULL, &ts)!=0);


	  pthread_mutex_destroy(&m); 

    close(tty_fd);
    tcsetattr(STDOUT_FILENO, TCSANOW, &old_stdio);
}


float wheels[4] ={0.0f};
geometry_msgs::Twist msg;
//自己速度float(3)+誤差混入度q15_t(1)+エンコーダのfloatデータ(4) = 3*4+1*2+4*4=30bytes
void* th_rx(void* pParam){

	printf("Rx thread starts\n");

  int size;
  int state = 0;
  
  typedef union __rawfloat
  {
    unsigned char raw[4];
    float data;
  }rawfloat;
  typedef union __rawshort
  {
    unsigned char raw[2];
    short data;
  }rawshort;

  rawfloat v[3];
  rawshort err;
  rawfloat enc[4];
  int offset = 0;
  printf("size of float : %d\r\n", sizeof(float));
  printf("size of short : %d\r\n", sizeof(short));
  while(true)
  {
    if(size = read(tty_fd, &c, 1) > 0)
    {
      //printf("c = %02x / state = %d\r\n", c, state);
      switch(state)
      {
        case 0:
        case 1:
          if(c == '#')state++;
          else
          {
            state = 0;
            offset = 0;
          }
          break;
        case 32:
          //printf("|\r\nvel : %f, %f, %f\r\nerr : %d\r\nenc : %f, %f, %f, %f\r\n\n", v[0].data, v[1].data, v[2].data, err.data, enc[0].data, enc[1].data, enc[2].data, enc[3].data);
          msg.linear.x = v[0].data;
          msg.linear.y = v[1].data;
          msg.angular.z = v[2].data;
          for (int k = 0; k<4;k++)
          {
            wheels[k] = enc[k].data;
          }
          if (isROS)
          {
            encoder_pub.publish(msg);
          }
          if(c == '#')state = 1;
          else 
          {
            state = 0;
            offset = 0;
          }
          break;
        case 2:
        case 6:
        case 10:
        case 14:
        case 16:
        case 20:
        case 24:
        case 28:
          //printf("| ");
        default:
          //printf("%02x ", c);
          if(state < 14)
          {
            offset = 2;
            v[(state - offset) / 4].raw[(state - offset) % 4] = c;
          }
          else if(state < 16)
          {
            offset = 14;
            err.raw[state - offset] = c;
          }
          else
          {
            offset = 16;
            enc[(state - offset) / 4].raw[(state - offset) % 4] = c;
          }
          state++;
          break;
      }
    }
    if(read(STDIN_FILENO, &c, 1) > 0)
      write(tty_fd, &c, 1);
  }

	printf("Rx thread ends\n");	
}
