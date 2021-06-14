#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Joy.h>
#include "../include/UART_solve.hpp"

void joy_callback(const sensor_msgs::Joy::ConstPtr& joy)
{
    vel1 = joy->axes[0]*300*(-1);
    vel2 = joy->axes[1]*300;
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "yuntai_serial");
    ros::NodeHandle n;

    ////声明串口对象
    serial::Serial yuntai_port;

    uint8_t RxBuffer[20];
    uint8_t TxBuffer[8];

    ros::Subscriber sub_joy = n.subscribe<sensor_msgs::Joy>("/joy",1,joy_callback);


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
        }
        TxBuffer_Package(TxBuffer,vel1,vel2);
        yuntai_port.write(TxBuffer,8);
       // ROS_INFO("vel1:%d",vel1);
       // ROS_INFO("vel2:%d\n",vel2);




        /*
        int16_t v = -50;
        uint8_t vel_buffer[2];
        int16_t v_decode;


        vel_buffer[0] = v>>8;
        vel_buffer[1] = v;
        v_decode = (int)(vel_buffer[0]<<8|vel_buffer[1]);
        count+=1;
        if(count>50)
        {
            ROS_INFO("vel decode :%d",v_decode);
            ROS_INFO("vel decode :%d\n",v);
            count=0;
        }
         */

        ros::spinOnce();
        loop_rate.sleep();

    }

    yuntai_port.close();

    return 0;
}
