cmake_minimum_required(VERSION 2.8.3)
project(trajectory)
set(CMAKE_VERBOSE_MAKEFILE FALSE)


find_package(
  catkin REQUIRED
  roscpp
  std_msgs
)


#OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Boost
find_package(Boost 1.54 REQUIRED)

option(ENABLE_CUDA "Enable the cuda build" ON)
if(ENABLE_CUDA)
  find_package(CUDA 9.0 REQUIRED)
endif()

catkin_package(
  CATKIN_DEPENDS
  roscpp
  std_msgs
)


include_directories(
  ${PROJECT_SOURCE_DIR}/inc
  ${OpenGL_INCLUDE_DIR}
  ${GLUT_INCLUDE_DIR}
  ${CATKIN_INCLUDE_DIRS}
  ${CUDA_INCLUDE_DIRS}
  ${CMAKE_PREFIX_PATH}/include
)

set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -gencode arch=compute_60,code=sm_60 -O3 --use_fast_math -std=c++11")
set(CUDA_SEPARABLE_COMPILATION OFF)
set(CUDA_PROPAGATE_HOST_FLAGS OFF)

#???
set_directory_properties( PROPERTIES COMPILE_DEFINITIONS "" )

cuda_add_executable(traj_node
  src/main.cpp
  src/path_core.cu
  src/spline_gpu.cu
  src/ferguson_coons_gpu.cu
  src/minimum_jerk.cu
  src/MPC.cu
)

cuda_add_cublas_to_target(traj_node)

target_link_libraries(traj_node
 ${OPENGL_LIBRARIES}
 ${GLUT_LIBRARY}
 ${catkin_LIBRARIES}
 ${CUDA_LIBRARIES}
 ${CUDA_cusparse_LIBRARY}
)


################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a exec_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a exec_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a exec_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )
