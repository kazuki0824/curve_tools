/*
 * main.cpp
 *
 *  Created on: 2018/11/20
 *      Author: maleicacid
 */

#include <ros/ros.h>
#include <iostream>
#include <thread>
#include "comm.h"

#include "base_control/myTwistacc.h"
#include "control_server/reference.h"

using namespace std;

const double a = 308.432;
const double b = 308.432;
const double radius = 0 ;

#define rows 4
#define cols 1
void refMsgCallback(const base_control::myTwistacc::ConstPtr accel)
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
    //Scilab ref

    /* critical section start */

    /* end */
}

int main(int argc, char** argv)
{
    vector<rawSerialport> motors;

    if(argc<=4)
    {
        cout << "fail" << endl;
    }
    for(size_t i = 1; i <= 4; i++)
    {
        rawSerialport a(argv[i]);
        motors.push_back(a);
    }

    //threads
    
    //ROS
	ros::init(argc, argv, "base_control");
	ros::NodeHandle nh("~");
	ros::Subscriber s_twist = nh.subscribe("v_accel", 100, refMsgCallback);
    while(true){
        /* code */
        ros::spinOnce();
    }
    
}
