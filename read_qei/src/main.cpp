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
void* th_tx(void* pParam); //送信用スレッド

static bool isROS =false;
ros::Publisher encoder_pub;

pthread_mutex_t mutex; //2つのスレッド間で変数の保護を行う
//　スレッド内で，   pthread_mutex_lock(&mutex);　　
//    　           pthread_mutex_unlock(&mutex);
// の間で排他処理をさせる

int fd = -1;

int main(int argc, char** argv){
    struct termios tio_old, tio;
    speed_t baudRate = B115200;
	int ret;
	char buf[256];
	pthread_t tid_tx, tid_rx; // Thread IDs
    char default_device[256] = "/dev/ttyACM0";
    char * device = NULL;
	if(argc < 2)
	{
        printf("Open warning  : A device should be specified. Assume '/dev/ttyACM0'\n");
        device = default_device;
	}
    else
    {
        device = argv[1];
    }

//Device Open
    fd = open(device, O_RDWR | O_NOCTTY);
	if(fd < 0){
        printf("Open error : %s \n", device);
        return fd;
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
    tio.c_cc[VMIN] = 32;
    //tio.c_cc[VMIN] = 1;

    //tcflush(fd, TCIFLUSH);
    tcsetattr( fd, TCSANOW, &tio );     // デバイスに設定を行う

//Device Open (finish)

//Create threads
	pthread_create(&tid_tx, NULL, th_tx, NULL);
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
    }while(pthread_timedjoin_np(tid_tx, NULL, &ts)!=0);

	pthread_cancel( tid_rx );
	pthread_join(tid_rx,NULL);


	pthread_mutex_destroy(&mutex); 

	tcsetattr(fd, TCSANOW, &tio_old);
	close(fd);
}


pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
float wheels[4] ={0.1};
geometry_msgs::Twist msg;
const int msgbodysize = 8; //自己速度float(3)+誤差混入度q15_t(1)+エンコーダのfloatデータ(4) = 3*4+1*2+4*4=30bytes
void* th_rx(void* pParam){
	int ret;
    char buf[64] ={0};

	printf("Rx thread starts\n");

	unsigned int reading_flag = 0;
	struct mystruct
	{
		float velo[3];
		short int e;
		float wheels[4];
	};
	union
	{
		char Byte[30];
		struct mystruct Body;
	} u;
    while(1){
        usleep(1000);
        memset(buf,0,64);
        ret = read(fd, buf, 32);
        if(ret == 32)
        {
            //1文字読み取れた
            char* buf_ptr = (char*)memchr(buf, '#', 32);
            if (buf_ptr ==NULL)
            {
                continue;
            }
            else if (buf_ptr[1] == '#')
            {
                int rest = buf_ptr-buf;
                if(rest!=0)
                {
                    do
                    {
                        ret = read(fd, &buf[32], rest);
                    } while (ret <= 0);
                }
                memcpy(u.Byte,&buf_ptr[2],30);
                msg.linear.x = u.Body.velo[0];
                msg.linear.y = u.Body.velo[1];
                msg.angular.z = u.Body.velo[2];
                for (int k = 0; k<4;k++)
                {
                    wheels[k]=u.Body.wheels[k];
                }
                if(isROS)
                {
                    encoder_pub.publish(msg);
                }
            }
        }
        else
            continue;
    }

    /*
    while(1){
        ret = read(fd, buf, 32);
		if(ret > 0){
			//1文字読み取れた
			char r = buf[0];
			switch (reading_flag)
			{
				case 0:
				{
                    if(r == '#')
					{
						reading_flag++;
					}
					else
					{
						reading_flag=0;
					}
					break;
				}
				case 1:
				{
					if(r == '#')
					{
						reading_flag++;
					}
					else
					{
						reading_flag=0;
					}
					break;
				}
				default:
				{
					if(reading_flag < 2 + msgbodysize)
					{
						//Body読み取り
						u.Byte[reading_flag - 2] = r;
						reading_flag++;
					}
					else
					{
                        if(reading_flag == 2 + msgbodysize)
                        {
                            msg.linear.x = u.Body.velo[0];
                            msg.linear.y = u.Body.velo[1];
                            msg.angular.z = u.Body.velo[2];
                            if(isROS)
                            {
                                encoder_pub.publish(msg);
                            }
						}
						reading_flag=0;
					}
				}
			}
		}
		else
            continue;
	}
    */

//Never comes here!
	printf("Rx thread ends\n");	
}



void* th_tx(void* pParam){
	int ret;
	char buf[64];
	int val;
	
	printf("Tx thread starts\n");
	printf("(Type 'e' for exit)\n");

	while(1){
		fgets(buf, 64, stdin);
		if(buf[0] == 'e' || buf[0] == 'E')
			break;

		val = strtol(buf, NULL, 0);
		buf[0] = (char)(val & 0xff);
		printf("TX:%02x\n", (unsigned char)buf[0]);
		ret = write(fd, buf, 1);
	}

	printf("TX thread ends\n");
}
