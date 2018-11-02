cmake_minimum_required(VERSION 2.8.10.2)
project(read_qei)
set(CMAKE_VERBOSE_MAKEFILE FALSE)

find_package(
  catkin REQUIRED
  roscpp
  std_msgs
)
catkin_package(
  CATKIN_DEPENDS
  roscpp
  std_msgs
)

#pthread
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

#Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
FIND_PACKAGE(Qt5Widgets REQUIRED)
MESSAGE(STATUS "Qt5 --> ${Qt5Widgets_VERSION_STRING}")

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${CATKIN_INCLUDE_DIRS}
  ${CMAKE_PREFIX_PATH}/include
)

add_executable(qei_node
  main.cpp
)


target_link_libraries(qei_node
 ${catkin_LIBRARIES}
)
QT5_USE_MODULES(qei_node Widgets)
