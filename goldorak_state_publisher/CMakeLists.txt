cmake_minimum_required(VERSION 2.8.3)
project(goldorak_state_publisher)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    rospy
    sensor_msgs
    std_msgs
    tf
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(
    joint_state_publisher
    src/joint_state_publisher.cpp
)
target_link_libraries(joint_state_publisher ${catkin_LIBRARIES})
