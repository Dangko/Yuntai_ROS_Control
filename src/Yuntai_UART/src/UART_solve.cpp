#include "../include/UART_solve.hpp"

float angle[2];
int16_t vel1,vel2;
int16_t vel1_feed,vel2_feed;

void RxBuffer_Decode(uint8_t* RxBuffer)
{
    for(int i=0;i<20;i++)
    {
        if(RxBuffer[i]==0x55 && RxBuffer[i+1]==0x20)
        {
            int index=i+2;
            angle[0]=((int16_t)(RxBuffer[index]<<8|RxBuffer[index+1]))/100.0;
            angle[1]=((int16_t)(RxBuffer[index+2]<<8|RxBuffer[index+3]))/100.0;
            ROS_INFO_STREAM("Reading from serial port");
            ROS_INFO("angle1:%f",angle[0]);
            ROS_INFO("angle2:%f\n",angle[1]);
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
            ROS_INFO_STREAM("Reading from serial port");
            ROS_INFO("vel1 feed back:%d",vel1_feed);
            ROS_INFO("vel2 feed back:%d\n",vel2_feed);
            break;
        }
    }
}

void TxBuffer_Package(uint8_t* TxBuffer,int vel1,int vel2)
{
    TxBuffer[0] = 0x55;
    TxBuffer[1] = 0x10;
    TxBuffer[2] = vel1>>8;
    TxBuffer[3] = vel1;
    TxBuffer[4] = vel2>>8;
    TxBuffer[5] = vel2;
    TxBuffer[0] = 0;
    TxBuffer[0] = 0;
}