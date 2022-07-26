/*
 * main.cpp
 *
 *  Created on: 2018/11/07
 *      Author: kazuki
 */
#include "ros/ros.h"
#include "control_server/reference.h"
#include "control_server/C1.h"
extern "C" {
#include "sci_bridge.h"
}

bool Tuning(control_server::C1::Request &req, control_server::C1::Response &res)
{
  //res.sum = req.a + req.b;
  double kfparam[3]={req.Kf.data[0],req.Kf.data[1],req.Kf.data[2]};
  process_C1(kfparam);
  return true;
}

bool getRef(control_server::reference::Request &req, control_server::reference::Response &res)
{
	process_ref((double)req.velo, (double)req.accel, (double)req.duty, (double)req.Tf);

	int row,col;
	double * result = read_matrix("ref", &row, &col);
	
	//FIXME: risk of segfault
	res.ref_.data[0] = result[0];
	res.ref_.data[1] = result[1];
  return true;
}


int main(int argc, char **argv)
{
	if(argc<=1)
		return -1;

	ros::init(argc, argv, "control_server");
	ros::NodeHandle n;

	ROS_INFO("control_server: Initializing Scilab");

	init_sci(argv[1]);
	ROS_INFO("control_server: Success");

	/** Motor Info **/
	Init_MotorModel();

	ros::ServiceServer C1srv = n.advertiseService("control_server_C1", Tuning);
	ros::ServiceServer refsrv = n.advertiseService("control_server_reference", getRef);
	ROS_INFO("control_server: Ready");


	ros::spin();

	deinit_sci();
	return 0;
}
