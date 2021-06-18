[toc]

# 一、stm32接收协议

- 速度控制接受模式：

  0x55|0x10|电机 1 速度高8位|电机 1 速度低8位|电机 2 速度高8位|电机 2 速度低8位|0|0

  ​    1   |   2   |                  3         |                    4        |                    5       |                         6  |7|8    

- 位置控制接受模式

  0x55|0x20|电机 1 位置高8位|电机 1 位置低8位|电机 2 位置高8位|电机 2 位置低8位|0|0

  ​    1   |   2   |                  3         |                    4        |                    5       |                         6  |7|8    

- 停止模式

  0x55|0x30|x|x|x|x|x|x



# 二、stm32发送协议

仅发送角度信息

- 位置控制接受模式

  0x55|0x20|电机 1 位置高8位|电机 1 位置低8位|电机 2 位置高8位|电机 2 位置低8位|0|0

  ​    1   |   2   |                  3         |                    4        |                    5       |                         6  |7|8   

 

单片机Tx、Rx引脚：

- UART

  PA9：TX

  PA10：RX

- CAN

  PA11：RX

  PA12：TX

   

# 三、手柄控制

解锁手柄权限：

`sudo chomod a+rw /dev/input/js0`

运行joy节点：

`rosrun  joy  joy_node`



# 四、ROS接口

拟定根据接收话题的类型，决定速度或位置控制模式，或是在接收停止指令后进行急停

运行ros程序前，需要解锁USB权限，USB2TTL模块才能被打开：

`sudo chomod 666 /dev/ttyUSB0`

运行可执行文件：

`rosrun  Yuntai_UART  yuntai_uart`



## 4.1订阅的话题

- $/vel$

  消息类型：`geometry_msgs::Twist`

  绕y轴速度：`twist.angular.y`

  绕z轴速度：`twist.angular.z`

  发布`/vel`话题，并给上述内容赋值，即可控制绕y、z轴的速度

- $/pose$

  消息类型：`sensor_msgs::Imu`

  $pitch$：`imu.orientation.y`

  $yaw$：`twist.orientation.z`

  发布`/pose`话题，并给上述内容赋值，即可控制云台的 $pitch$、$yaw$

- $/joy$

  消息类型：`sensor_msgs::Joy`

  仅用于手柄控制时使用





