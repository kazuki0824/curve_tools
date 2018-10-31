/*
 * path_core.hpp
 *
 *  Created on: 2018/10/31
 *      Author: kazuki
 */

#ifndef PATH_CORE_HPP_
#define PATH_CORE_HPP_


#include <vector>

#include "helper_math.h"
#include "spline_gpu.hcu"

#include <cusparse_v2.h>
#include <cublas_v2.h>

std::vector<pSpline_polynomial>& getReference(float2 point[32] ,int pointcount, cusparseHandle_t& cusparsehandle);
float getMinimumTime(std::vector<pSpline_polynomial>& path, float maximum_velocity, float maximum_accel);

void DispatchTrajectory(float3 pose, float3 twist, std::vector<pSpline_polynomial>& path , float whole_time_for_reference);

#endif /* PATH_CORE_HPP_ */
