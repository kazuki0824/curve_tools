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
#include <chrono>
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
#define cols 2
double wheel_reference[rows][cols] = {0};

std::timed_mutex motor_refs_mutex;
bool abort_flag = false;

void MotorDriverHandler(int wheel_number, const char* device, ros::ServiceClient& client)
{
    rawSerialport * a = new rawSerialport(device);
    if(!a->isErr)
    {
        float ref[3] = {0.0,0.0,0.0};
        float x_hat[4] = {0.0,0.0,0.0,0.0}; //V on the bottom.
        while(!abort_flag)
        {
        //read state_variable
            char msgs[MD_Msg_Size];
            if (a->tryReadMsg(msgs) == MD_Msg_Size)
            {
                //TODO
            }
            else
            {
                continue;
            }

        //wait for mutex
            if (!motor_refs_mutex.try_lock_for(std::chrono::milliseconds(50))) {
                cout << "Mutex Error: No Twistacc" << endl;
                //abort_flag = true;
                continue;
            }
        //read trajMsg
            float ref_velo = wheel_reference[wheel_number-1][0];
            float ref_accel = wheel_reference[wheel_number-1][1];
        //Scilab ref
            control_server::reference conversation;
            conversation.request.velo = ref_velo;
            conversation.request.accel = ref_accel;
            conversation.request.Tf = x_hat[2];
            conversation.request.duty = x_hat[3];
            if (client.call(conversation))
            {
                ref[0] = conversation.response.ref_.data[0];
                ref[1] = conversation.response.ref_.data[1];
            }
            motor_refs_mutex.unlock();
        //TODO: send ref

        }
    }
    delete a;
}

void trajMsgCallback(const base_control::myTwistacc& tw)
{
    double trans[rows][3] = {
        {-1, 1, (a+b)},
        {1 , 1, -(a+b)},
        {-1, 1, -(a+b)},
        {1,  1, (a+b)}
    };
    
    /* critical section start */
    //set motor angular velocities & angular accels
    std::lock_guard<std::timed_mutex> lock(motor_refs_mutex);
    double twist_accel_x[2][3] = {{tw.Twist.linear.x,tw.Twist.linear.y,tw.Twist.angular.z},{tw.Accel.linear.x,tw.Accel.linear.y,tw.Accel.angular.z}};
    #pragma omp parallel for
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++) 
        {
            double cc = 0.0;
            for (int k = 0; k < 3; k++) {
                cc += trans[i][k] * twist_accel_x[j][k];
            }
            wheel_reference[i][j] = cc;
        }
    }
    /* end */
}
void mySigintHandler(int sig)
{
    abort_flag = true;
}

int main(int argc, char** argv)
{
    
#ifdef _OPENMP
#pragma message "base_control: OpenMP Enabled"
    cout << "base_control: OpenMP Enabled" << endl;
#else
#pragma message "base_control: OpenMP Disabled"
    cout << "base_control: OpenMP Disabled" << endl;
#endif

    if(argc<=4)
    {
        cout << "fail" << endl;
        return -1;
    }
    //ROS
	ros::init(argc, argv, "base_control");
	ros::NodeHandle nh("~");
	ros::Subscriber reference_generation = nh.subscribe("v_accel", 100, trajMsgCallback);
    ros::ServiceClient client = nh.serviceClient<control_server::reference>("control_server_reference");

    vector<rawSerialport> motors;
    vector<thread> motorThreads;

    for(int i = 1; i <= 4; i++)
    {
        motorThreads.push_back(std::thread(MotorDriverHandler, i, argv[i], std::ref(client)));
    }
    
	signal(SIGINT, mySigintHandler);
    while(!abort_flag){
        /* code */
        ros::spinOnce();
    }
    ros::shutdown();
}
