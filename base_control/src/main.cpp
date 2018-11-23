/*
 * main.cpp
 *
 *  Created on: 2018/11/20
 *      Author: maleicacid
 */

#include <omp.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <signal.h>

#include "comm.h"

#include <ros/ros.h>
#include "base_control/myTwistacc.h"
#include "control_server/reference.h"

using namespace std;

const double a = 178.2;
const double b = 290.0;
const double radius = 50.8 ;

#define rows 4
#define cols 1

std::mutex motor_refs_mutex;
bool abort_flag = false;

void MotorDriverHandler(const char* device)
{
    rawSerialport * a = new rawSerialport(device);
    if(!a->isErr)
    {
        while(!abort_flag){    
        //wait for mutex
    
        //Scilab ref
    
        //send ref
        }
    }
    delete a;
}

void trajMsgCallback(const base_control::myTwistacc::ConstPtr accel)
{
    //Matrix mul
    double trans[rows][3] = {
        {-1, 1, (a+b)},
        {1 , 1, -(a+b)},
        {-1, 1, -(a+b)},
        {1,  1, (a+b)}
    };
    double twist_x[1][3] = {{0,0,0}};
    double result[rows][cols];
#ifdef _OPENMP
    cout << "OpenMP Enabled" << endl;
#else
    cout << "OpenMP Disabled" << endl;
#endif
    #pragma omp parallel for
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++) 
        {
            double cc = 0.0;
            for (int k = 0; k < 3; k++) {
                cc += trans[i][k] * twist_x[j][k];
            }
            result[i][j] = cc;
        }
    }

    /* critical section start */
    //set motor angular velocities & angular accels

    /* end */
}
void mySigintHandler(int sig)
{
    abort_flag =true;
}

int main(int argc, char** argv)
{
    vector<rawSerialport> motors;
    vector<thread> motorThreads;

    if(argc<=4)
    {
        cout << "fail" << endl;
    }
    for(size_t i = 1; i <= 4; i++)
    {
        motorThreads.push_back(std::thread(MotorDriverHandler, argv[i]));
    }

    //ROS
	ros::init(argc, argv, "base_control");
	ros::NodeHandle nh("~");
	ros::Subscriber reference_generation = nh.subscribe("v_accel", 100, trajMsgCallback);
    
	signal(SIGINT, mySigintHandler);
    while(!abort_flag){
        /* code */
        
        ros::spinOnce();
    }
    ros::shutdown();
}
