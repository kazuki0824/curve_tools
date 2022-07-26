cmake_minimum_required(VERSION 2.8.3)
project(control_server)

set(CMAKE_VERBOSE_MAKEFILE FALSE)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp message_generation std_msgs)
find_package(PkgConfig REQUIRED)
pkg_check_modules(scilab REQUIRED scilab)

## Generate services in the 'srv' folder
add_service_files(
   FILES
   reference.srv
   C1.srv
)
 generate_messages(DEPENDENCIES std_msgs)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  CATKIN_DEPENDS
  roscpp
  message_runtime
  std_msgs
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  ${PROJECT_SOURCE_DIR}
  ${CATKIN_INCLUDE_DIRS}
  ${scilab_INCLUDE_DIRS}
  ${CMAKE_PREFIX_PATH}/include
)

set(CMAKE_INSTALL_RPATH "/usr/lib/scilab") 
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) 

#string (REPLACE ";" " " LD_FLAGS_STR "-L/usr/lib/scilab;-lscilab-cli;-lscicall_scilab")
#Message("${LD_FLAGS_STR}")
#set(CMAKE_EXE_LINKER_FLAGS "${LD_FLAGS_STR}")

add_executable(${PROJECT_NAME}_node src/control_server_node.cpp src/sci_bridge.c)
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS}) #necessary for message generation


## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}_node
   ${catkin_LIBRARIES}
   /usr/lib/scilab/libscicall_scilab.so
   /usr/lib/scilab/libscilab-cli.so
)


#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
install(PROGRAMS
  synth_core.sce
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark executables and/or libraries for installation
# install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_control_server.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
