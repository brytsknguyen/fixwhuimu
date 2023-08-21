#include <ros/ros.h>
#include "sensor_msgs/Imu.h"

typedef sensor_msgs::Imu rosImuMsg;
typedef sensor_msgs::Imu::ConstPtr rosImuMsgPtr;

ros::Publisher imu_mod_pub;

void imuCB(const rosImuMsgPtr &msg)
{
    rosImuMsg imu_mod;
    
    imu_mod = *msg;

    imu_mod.angular_velocity.x = -M_PI/180.0*msg->angular_velocity.y;
    imu_mod.angular_velocity.y = -M_PI/180.0*msg->angular_velocity.z;
    imu_mod.angular_velocity.z =  M_PI/180.0*msg->angular_velocity.x;

    imu_mod.linear_acceleration.x = -msg->linear_acceleration.y;
    imu_mod.linear_acceleration.y = -msg->linear_acceleration.z;
    imu_mod.linear_acceleration.z =  msg->linear_acceleration.x;

    imu_mod_pub.publish(imu_mod);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "fixwhuimu");
    ros::NodeHandle nh("~");

    ROS_INFO("----> Imu Odometry Started.");

    // Subscribe to the IMU topic
    ros::Subscriber imu_sub = nh.subscribe("/imu0", 10, imuCB);

    // Advertise the modify imu
    imu_mod_pub = nh.advertise<sensor_msgs::Imu>("/imu0_mod", 10);

    ros::MultiThreadedSpinner spinner(0);
    spinner.spin();

    return 0;
}
