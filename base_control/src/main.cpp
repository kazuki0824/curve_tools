/*
 * main.cpp
 *
 *  Created on: 2018/11/20
 *      Author: maleicacid
 */

#include <ros/ros.h>
#include <iostream>
#include "comm.h"

using namespace std;


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

}
