cmake_minimum_required(VERSION 2.8.3)
project(read_qei)

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
