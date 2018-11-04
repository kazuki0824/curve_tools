/*
 * path_core.cu
 *
 *  Created on: 2018/10/31
 *      Author: kazuki
 */
#include "path_core.hpp"
std::vector<pSpline_polynomial>& getReference(float2 point[32] ,int pointcount, cusparseHandle_t& cusparsehandle)
{

	pSpline_polynomial f[32];
	pSpline(point, f, pointcount, &cusparsehandle);
	std::vector<pSpline_polynomial> path;
	//TODO :個数は正しい?
	for (int i = 0; i < pointcount; ++i) {
		path.push_back(f[i]);
	}
}

float getMinimumTime(std::vector<pSpline_polynomial>& path, float maximum_velocity, float maximum_accel)
{
	float raw_time = 1.0 * path.size();

	float vtmp =0.0;
	float atmp = 0.0;

	for (int var = 0; var < path.size(); ++var) {
		for (float t = 0.0; t < 1.0; t += 0.05) {
			vtmp =fmax(length(path[var].speed(t)),vtmp);
			atmp =fmax(length(path[var].accel(t)),atmp);
		}
	}
    float scale = fmax( ( maximum_velocity / vtmp ) , ( maximum_accel / atmp ) );

    return raw_time * scale;
}


//float2.x=time; float2.y=angle
static std::vector<pSpline_polynomial> getReferenceAngle(float2 point[32], int pointcount, cusparseHandle_t& cusparsehandle)
{
//TODO:
}


void DispatchTrajectory(float3 pose, float3 twist, std::vector<pSpline_polynomial>& path , float whole_time_for_reference)
{

}

