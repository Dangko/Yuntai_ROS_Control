#include "../include/UART_solve.hpp"


float angle[2];
int16_t vel1,vel2;
int16_t vel1_feed,vel2_feed;
float pitch,yaw;
uint8_t RxBuffer[20];
uint8_t TxBuffer[8];
bool flag_decode=0;
sensor_msgs::Imu pose_feedback;

void RxBuffer_Decode(uint8_t* RxBuffer)
{
    for(int i=0;i<20;i++)
    {
        if(RxBuffer[i]==0x55 && RxBuffer[i+1]==0x20)
        {
            int index=i+2;
            angle[0]=((int16_t)(RxBuffer[index]<<8|RxBuffer[index+1]))/100.0;
            angle[1]=((int16_t)(RxBuffer[index+2]<<8|RxBuffer[index+3]))/100.0;
            pose_feedback.orientation.y = angle[0];
            pose_feedback.orientation.z = angle[1];
            flag_decode = true;
            ROS_INFO_STREAM("Reading from serial port");
            ROS_INFO("pitch:%f",angle[0]);
            ROS_INFO("yaw:%f\n",angle[1]);
            break;
        }
    }
}

void RxBuffer_Decode_vel(uint8_t* RxBuffer)
{
    for(int i=0;i<20;i++)
    {
        if(RxBuffer[i]==0x55 && RxBuffer[i+1]==0x20)
        {
            int index=i+2;
            vel1_feed=(int16_t)(RxBuffer[index]<<8|RxBuffer[index+1]);
            vel2_feed=(int16_t)(RxBuffer[index+2]<<8|RxBuffer[index+3]);
            flag_decode = true;
            ROS_INFO_STREAM("Reading from serial port");
            ROS_INFO("vel1 feed back:%d",vel1_feed);
            ROS_INFO("vel2 feed back:%d\n",vel2_feed);
            break;
        }
    }
}

void TxBuffer_Package_vel(uint8_t* TxBuffer,int16_t vel1,int16_t vel2)
{
    TxBuffer[0] = 0x55;
    TxBuffer[1] = 0x10;
    TxBuffer[2] = vel1>>8;
    TxBuffer[3] = vel1;
    TxBuffer[4] = vel2>>8;
    TxBuffer[5] = vel2;
    TxBuffer[6] = 0;
    TxBuffer[7] = 0;
}

void TxBuffer_Package_pos(uint8_t* TxBuffer,float pitch,float yaw)
{
    int16_t pitch_int = (int16_t)(pitch*100);
    int16_t yaw_int = (int16_t)(yaw*100);
    TxBuffer[0] = 0x55;
    TxBuffer[1] = 0x20;
    TxBuffer[2] = pitch_int>>8;
    TxBuffer[3] = pitch_int;
    TxBuffer[4] = yaw_int>>8;
    TxBuffer[5] = yaw_int;
    TxBuffer[6] = 0;
    TxBuffer[7] = 0;
}
