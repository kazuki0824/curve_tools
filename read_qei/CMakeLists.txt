cmake_minimum_required(VERSION 2.8.10.2)
project(read_qei)
set(CMAKE_VERBOSE_MAKEFILE FALSE)

add_compile_options(-std=c++11)

find_package(
  catkin REQUIRED
  roscpp
  std_msgs
  geometry_msgs
)
catkin_package(
  CATKIN_DEPENDS
)

#pthread
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

#Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
FIND_PACKAGE(Qt5Widgets REQUIRED)
MESSAGE(STATUS "Qt5 --> ${Qt5Widgets_VERSION_STRING}")

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${CATKIN_INCLUDE_DIRS}
  ${CMAKE_PREFIX_PATH}/include
  /usr/include/qwt
)

file(GLOB_RECURSE CPP_FILES src/*.cpp)
file(GLOB_RECURSE UI_FILES src/*.ui)
file(GLOB_RECURSE QRC_FILES res/*.qrc)

add_executable(qei_node
    ${CPP_FILES}
    ${UI_FILES}
    ${QRC_FILES}
)


target_link_libraries(qei_node
 ${catkin_LIBRARIES}
 qwt-qt5
)
QT5_USE_MODULES(qei_node Widgets)
