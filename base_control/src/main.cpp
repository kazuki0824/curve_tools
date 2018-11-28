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

//メートルで
const double a = 0.1782;
const double b = 0.29;
const double radius = 0.0508 ;

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
        typedef union __rawfloat
        {
            unsigned char raw[4];
            float data;
        }rawfloat;
        rawfloat x_recv[4];
        char c ='D';
        
        int size;
        int state = 0;
        int offset = 0;
        while(!abort_flag)
        {
            //read state_variable
            if(size = a->tryReadMsg(&c) > 0)
            {
                switch(state)
                {
                    case 0:
                        if (c == 0xff) state++;
                        else
                        {
                            state = 0;
                            offset = 0;
                        }
                        break;
                    case 1:
                        if (c == MD_Msg_Size & 0xff) state++;
                        else
                        {
                            state = 0;
                            offset = 0;
                        }
                        break;
                    case 2:
                        if (c == (MD_Msg_Size >> 8) & 0xff) state++;
                        else
                        {
                            state = 0;
                            offset = 0;
                        }
                        break;
                    case 3:
                        if (c == 'x') state++;
                        else
                        {
                            state = 0;
                            offset = 0;
                        }
                        break;
                    case 4:
                        if (c == 4) state++;
                        else
                        {
                            state = 0;
                            offset = 0;
                        }
                        break;
                    case 5:
                        if (c == 1) state++;
                        else
                        {
                            state = 0;
                            offset = 0;
                        }
                        break;
                    case MD_Msg_Size:
                        {
                            //read
                            for(int i = 0; i < 4; i++)
                            {
                                x_hat[i] = x_recv[i].data;
                            }
                            
                            if(c == 0xff)state = 1;
                            else 
                            {
                                state = 0;
                                offset = 0;
                            }
                            break;
                        }
                    default:
                        offset = 6;
                        x_recv[(state - offset) / 4].raw[(state-offset) % 4] = c;
                        break;
                }
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
        //send ref
            uint8_t msgs[6+8];
            size_t msgsize =ComposePackatFromMatrix(ref, 2,1, 'r', msgs);
            a->WriteMsg((char*)msgs, msgsize);
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
#warning "base_control: OpenMP Disabled"
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
