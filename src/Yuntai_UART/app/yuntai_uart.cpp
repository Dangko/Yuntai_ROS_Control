#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Joy.h>
#include "sensor_msgs/Imu.h"
#include "../include/UART_solve.hpp"

////声明串口对象
serial::Serial yuntai_port;

void joy_callback(const sensor_msgs::Joy::ConstPtr& joy)
{
    vel1 = joy->axes[2]*300*(-1);
    vel2 = joy->axes[3]*300;
    TxBuffer_Package_vel(TxBuffer,vel1,vel2);
    yuntai_port.write(TxBuffer,8);
}

void vel_callback(const geometry_msgs::Twist ::ConstPtr& twist)
{
    float vel1_f = twist->angular.y;
    float vel2_f = twist->angular.z;
    vel1 = (int16_t)vel1_f;
    vel2 = (int16_t)vel2_f;
    //ROS_INFO("pitch:%d",vel1);
    TxBuffer_Package_vel(TxBuffer,vel2,vel1);
    yuntai_port.write(TxBuffer,8);
}

void pos_callback(const sensor_msgs::Imu ::ConstPtr& imu)
{
    pitch =imu->orientation.y;
    yaw = imu->orientation.z;
    ROS_INFO("pitch:%f",pitch);
    ROS_INFO("yaw:%f\n",yaw);
    TxBuffer_Package_pos(TxBuffer,yaw,pitch);
    yuntai_port.write(TxBuffer,8);
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "yuntai_serial");
    ros::NodeHandle n;


    ros::Subscriber sub_joy = n.subscribe<sensor_msgs::Joy>("/joy",1,joy_callback);
    ros::Subscriber sub_vel = n.subscribe<geometry_msgs::Twist>("/vel",1,vel_callback);
    ros::Subscriber sub_pose = n.subscribe<sensor_msgs::Imu>("/pose",1,pos_callback);
    ros::Publisher  pub_pose = n.advertise<sensor_msgs::Imu>("/pose_feedback",1);

    try
    {
        ////设置串口属性，并打开串口
        yuntai_port.setPort("/dev/ttyUSB0");
        yuntai_port.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        yuntai_port.setTimeout(to);
        yuntai_port.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }

    ////检测串口是否已经打开，并给出提示信息
    if(yuntai_port.isOpen())
    {
        ROS_INFO_STREAM("Serial Port initialized");
    }
    else
    {
        return -1;
    }

    //指定循环的频率
    ros::Rate loop_rate(50);
    int count=0;
    while(ros::ok())
    {
        count+=1;
        if(yuntai_port.available()){
            std_msgs::String result;
            yuntai_port.read(RxBuffer,20);
            RxBuffer_Decode(RxBuffer);
            if(flag_decode==true)
            {
                pub_pose.publish(pose_feedback);
                flag_decode = false;
            }
        }

       // TxBuffer_Package(TxBuffer,vel1,vel2);
       // yuntai_port.write(TxBuffer,8);
       // ROS_INFO("vel1:%d",vel1);
       // ROS_INFO("vel2:%d\n",vel2);

        ros::spinOnce();
        loop_rate.sleep();

    }

    yuntai_port.close();

    return 0;
}
