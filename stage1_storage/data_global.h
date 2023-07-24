#ifndef __DATA_GLOBAL__H__
#define __DATA_GLOBAL__H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <termios.h>
#include <syscall.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <linux/input.h>

/*********************************************************
  data_global.h : 

  全局的宏定义#define
  全局的线程函数声明
  全局的设备节点声明
  全局的消息队列发送函数外部extern声明
  全局的消息队列传递的结构体信息声明

 *********************************************************/


/*********************************************************
  全局的宏定义
 *********************************************************/

#define MONITOR_NUM   1
#define QUEUE_MSG_LEN  32

#define		GPRS_DEV   			 "/dev/ttyUSB0"
#define		ZIGBEE_DEV 		 "/dev/ttyUSB1"
#define		BEEPER_DEV 		 "/dev/fsbeeper0"
#define		LED_DEV    			 "/dev/fsled0"


/*********************************************************
  全局的结构体声明
 *********************************************************/

typedef  unsigned char uint8_t;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t;

//考虑到内存对齐的问题
struct env_info{
	char temp; //温度
	char humi;  //湿度
	float MQ2;//温度下限
	float gyrox;
	float gyroy;
	float gyroz;
	float accelx;
	float accely;
	float accelz;

};






/*********************************************************
  全局的外部线程函数声明
 *********************************************************/

extern void *pthread_client_request (void *arg); //接收CGI 等的请求
extern void *pthread_refresh(void *arg);              //刷新共享内存数据线程
extern void *pthread_sqlite(void *arg);                 //数据库线程，保存数据库的数据
extern void *pthread_transfer(void *arg);           //接收ZigBee的数据并解析
extern void *pthread_sms(void *arg);                //发送短信线程
extern void *pthread_buzzer(void *arg);          //蜂鸣器控制线程
extern void *pthread_led(void *arg);                 //led灯控制线程


extern int send_msg_queue(long type,unsigned char text);


/*********************************************************
  全局的消息队列传递的结构体声明
 *********************************************************/

//消息队列结构体
struct msg
{
	long type;//从消息队列接收消息时用于判断的消息类型
	long msgtype;//具体的消息类型
	unsigned char text[QUEUE_MSG_LEN];//消息正文
};




#endif 


