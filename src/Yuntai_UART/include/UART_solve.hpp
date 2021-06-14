#ifndef UART_SOLVE_H
#define UART_SOLVE_H

#include "ros/ros.h"

void RxBuffer_Decode(uint8_t* RxBuffer);
void TxBuffer_Package(uint8_t* TxBuffer,int vel1,int vel2);
void RxBuffer_Decode_vel(uint8_t* RxBuffer);

extern float angle[2];
extern int16_t vel1;
extern int16_t vel2;
#endif