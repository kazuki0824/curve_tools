/*#include "mainwindow.h"
#include "ui_mainwindow.h"*/

#include <ros/ros.h>

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>


void* th_rx(void* pParam); //受信用スレッド
void* th_tx(void* pParam); //送信用スレッド


 pthread_mutex_t mutex; //2つのスレッド間で変数の保護を行う
//　スレッド内で，   pthread_mutex_lock(&mutex);　　
//    　           pthread_mutex_unlock(&mutex);
// の間で排他処理をさせる

int fd = -1;

int main(){
	struct termios tio_old, tio;
	int ret;
	char buf[256];
	pthread_t tid_tx, tid_rx; // Thread IDs

//Device Open
	fd = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY);
	if(fd < 0){
		printf("Open error : /dev/ttyUSB1\n");
		return -1;
	}

	tcgetattr(fd, &tio_old);
	bzero(&tio, sizeof(tio));

	tio.c_cflag = B1000000 | CS8 | CLOCAL | CREAD;
	//tio[0].c_oflag = 0;
	//tio[0].c_lflag = ICANON;

	tio.c_iflag = IGNPAR;	//ignore parity error
	tio.c_oflag = 0;
	tio.c_lflag = 0;

	tio.c_cc[VTIME] = 0;
	tio.c_cc[VMIN] = 1;

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &tio);
//Device Open (finish)

//Create threads
	pthread_create(&tid_tx, NULL, th_tx, NULL);
	pthread_create(&tid_rx, NULL, th_rx, NULL);
  
//Wait for threads end
	pthread_join(tid_tx,NULL);

	pthread_cancel( tid_rx );
	pthread_join(tid_rx,NULL);


	pthread_mutex_destroy(&mutex); 

	tcsetattr(fd, TCSANOW, &tio_old);
	close(fd);
}




void* th_rx(void* pParam){
	int ret;
	char buf[64];

	printf("Rx thread starts\n");

	while(1){
		ret = read(fd, buf, 1);
		if(ret > 0){
			printf("RX:%02x\n",(unsigned char)buf[0]);
		}
	}

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
