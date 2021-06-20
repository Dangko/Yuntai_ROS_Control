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
  
- 转速设置模式(仅限于位置控制模式下的转速设置，速度模式的速度不会受到影响)

  0x55|0x40|电机 1 速度高8位|电机 1 速度低8位|电机 2 速度高8位|电机 2 速度低8位|0|0

  ​    1   |   2   |                  3         |                    4        |                    5       |                         6  |7|8    



# 二、stm32发送协议

仅发送角度信息

- 位置控制接受模式

  0x66|0x20|电机 1 位置高8位|电机 1 位置低8位|电机 2 位置高8位|电机 2 位置低8位|0|0

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

使用程序前注意路径与版本问题，将已经编译好的debug与devel文件夹删除

需要serial包，执行`sudo apt-get install ros-version-serial`下载

运行ros程序前，需要解锁USB权限，USB2TTL模块才能被打开：

`sudo chomod 666 /dev/ttyUSB0`

编译后运行可执行文件：

`rosrun  Yuntai_UART  yuntai_uart`

可能出现2个选项，选择1即可



## 4.1订阅的话题

- `/vel​`

  消息类型：`geometry_msgs::Twist`

  绕y轴速度：`twist.angular.y`

  绕z轴速度：`twist.angular.z`

  发布`/vel`话题，并给上述内容赋值，即可控制绕y、z轴的速度，单位：`rpm`

  推荐控制范围：300~1000 rpm

  若订阅成功，终端会输出以下信息：

  ```c++
  [ INFO] [xxx]: Subcribe topic /vel successful,your control command is:
  [ INFO] [xxx]: set pitch rpm : xxx
  [ INFO] [xxx]: set yaw rpm :xxx
  ```

- `/pose​`

  消息类型：`sensor_msgs::Imu`

  `pitch​`：`imu.orientation.y`

  `yaw​`：`twist.orientation.z`

  发布`/pose`话题，并给上述内容赋值，即可控制云台的 `pitch`、`yaw​`，单位为角度制

  推荐控制范围：-180° ~ 180°

  若订阅成功，终端会输出以下信息：

  ```
  [ INFO] [xxx]: Subcribe topic /pose successful,your control command is:
  [ INFO] [xxx]: pitch: xxx
  [ INFO] [xxx]: yaw:xxx
  ```

- `/vel_set`

  消息类型：`geometry_msgs::Twist`

  绕y轴速度：`twist.angular.y`

  绕z轴速度：`twist.angular.z`

  发布`/vel_set`话题，并给上述内容赋值，即可设置在位置控制模式下，绕y、z轴转速的最大值，改变位置控制模式下的转速，单位为：`rpm`

  推荐设置范围：300 ~ 1000 rpm

  ```c++
  [ INFO] [xxx]: Subcribe topic /vel_set  successful,your control command is:
  [ INFO] [xxx]: set pitch rpm  max : xxx
  [ INFO] [xxx]: set yaw rpm  max :xxx
  ```

  

- `/joy​`

  消息类型：`sensor_msgs::Joy`

  仅用于手柄控制时使用



## 4.2 发布的话题

- `/pose\_feedback​`

  `pitch`：`imu.orientation.y`

  `yaw​`：`twist.orientation.z`

  订阅`/pose_feedback`话题，并获取上述内容，即可获取云台当前的 `pitch`$、`yaw`



# 五、调试bug即解决方式

- 若在发送话题后收到ROSINFO，但云台无反应，可用笔尖按下控制板上Reset按钮

- 若在插上usb后电机发生不规律的抖动，建议关闭电源，拔出usb，重启所有程序

  该问题需要查看stm32底层是否有串口接受错误，使用stlink或jlink(默认stlink)连接stm32，在debug模式下查看变量窗口watch1，

  观察变量Yuntai(云台控制结构体)、ReceiveData(串口通讯接收数组)、TxBuffer(串口通讯发送数组)

  Yuntai变量中，包含Motor、Pos两个结构体，包含Pid控制、反馈信息等关键变量

  根据一中的串口通讯协议，查看ReceiveData接收到的内容是否ros发送的数据包一致，可先查看帧头是否正确(0x55、0x66、0x10等)

  

