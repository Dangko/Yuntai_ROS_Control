#ifndef UART_SOLVE_H
#define UART_SOLVE_H

#include "ros/ros.h"
#include "sensor_msgs/Imu.h"

void RxBuffer_Decode(uint8_t* RxBuffer);
void TxBuffer_Package_vel(uint8_t* TxBuffer,int16_t vel1,int16_t vel2);
void TxBuffer_Package_pos(uint8_t* TxBuffer,float pitch,float yaw);
void RxBuffer_Decode_vel(uint8_t* RxBuffer);

extern float angle[2];
extern int16_t vel1;
extern int16_t vel2;
extern float pitch;
extern  float yaw;
extern uint8_t RxBuffer[20];
extern uint8_t TxBuffer[8];
extern bool flag_decode;
extern sensor_msgs::Imu pose_feedback;
#endif