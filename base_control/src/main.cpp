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



void refMsgCallback(const base_control::myTwistacc::ConstPtr accel)
{
    
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
	ros::Subscriber s_twist = nh.subscribe("v_accel",100, refMsgCallback);
    while(true){
        /* code */
        ros::spinOnce();
    }
    
}
