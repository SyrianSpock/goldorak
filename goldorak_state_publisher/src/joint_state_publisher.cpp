#include <string>
#include <math.h>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <std_msgs/Float32.h>

ros::Publisher *joint_pub;

float right_wheel_pos, left_wheel_pos;
int right_wheel_direction, left_wheel_direction, external_to_internal_wheelbase_encoder_direction;

void right_wheel_cb(const std_msgs::Float32ConstPtr& msg)
{
    right_wheel_pos = msg->data * right_wheel_direction
                        * external_to_internal_wheelbase_encoder_direction;
    ROS_DEBUG("Got right wheel %f", right_wheel_pos);
}

void left_wheel_cb(const std_msgs::Float32ConstPtr& msg)
{
    left_wheel_pos = msg->data * left_wheel_direction
                        * external_to_internal_wheelbase_encoder_direction;
    ROS_DEBUG("Got left wheel %f", left_wheel_pos);

    sensor_msgs::JointState joint_state;
    const double degree = M_PI / 180;

    // Update joint_state
    joint_state.header.stamp = ros::Time::now();

    joint_state.name.resize(2);
    joint_state.position.resize(2);
    joint_state.name[0] ="right_wheel_joint";
    joint_state.position[0] = right_wheel_pos;
    joint_state.name[1] ="left_wheel_joint";
    joint_state.position[1] = left_wheel_pos;

    //send the joint state and transform
    joint_pub->publish(joint_state);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "joint_state_publisher");

    ros::NodeHandle node;

    /* Get differential base parameters */
    node.param<int>("diffbase/right_wheel/direction", right_wheel_direction, 1);
    node.param<int>("diffbase/left_wheel/direction", left_wheel_direction, 1);
    node.param<int>("diffbase/external_to_internal_wheelbase_encoder_direction",
                    external_to_internal_wheelbase_encoder_direction, 1);

    ros::Subscriber right_wheel_sub = node.subscribe(
        "right_wheel/feedback/position", 1, right_wheel_cb);
    ros::Subscriber left_wheel_sub = node.subscribe(
        "left_wheel/feedback/position", 1, left_wheel_cb);

    ros::Publisher pub = node.advertise<sensor_msgs::JointState>("joint_states", 1);
    joint_pub = &pub;

    ROS_INFO("Joint state publisher node ready.");
    ros::spin();

    return 0;
}
