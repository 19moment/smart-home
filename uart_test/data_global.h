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

#define STORAGE_NUM 5
#define GOODS_NUM   10
#define QUEUE_MSG_LEN 32
 

/*********************************************************
	全局的外部线程函数声明
*********************************************************/

extern void *pthread_client_request (void *arg);//接收CGI，QT请求
extern void *pthread_refresh(void *arg);
extern void *pthread_sqlite(void *arg);
extern void *pthread_transfer(void *arg);
extern void *pthread_sms(void *arg);
 
extern int send_msg_queue(long type,unsigned char text);

/*********************************************************
	全局的结构体信息声明
*********************************************************/

//仓库
//仓库中的状态
//仓库中的货物信息
//怎么打包这些数据

//仓库货物信息
struct storage_goods_info
{
	unsigned char goods_type;//货物类型，用数字表示
	unsigned int  goods_count;//货物数量
};


//某个仓库的全部信息
struct storage_info
{
	unsigned char storage_status;  //存储状态
	unsigned char led_status;      //led状态  1 亮 - 0 灭
	unsigned char buzzer_status;   //蜂鸣器状态 1 报警 - 0 不报警
	unsigned char fan_status;      //风扇状态  1 转 - 0 不转
	unsigned char seg_status;     //???
	signed char x;       //x轴
	signed char y;       //y轴
	signed char z;       //z轴
	float temperature;   //温度
	float temperatureMIN;//温度下限
	float temperatureMAX;//温度上限
	float humidity;      //湿度
	float humidityMIN;   //湿度下限
	float humidityMAX;   //湿度上限
	float illumination;  //照明
	float illuminationMIN;//光照最小值
	float illuminationMAX;//光照最大值
	float battery;      //电池
	float adc;          //
	struct storage_goods_info goods_info[GOODS_NUM];  //仓库货物信息
};

//所有仓库的信息结构体
struct env_info_clien_addr
{
	struct storage_info storage_no[STORAGE_NUM];	//数组，编号仓库号 - 仓库一 仓库二
};

struct env_info_clien_addr all_info_RT;
/*********************************************************
	全局的消息队列传递的结构体声明
*********************************************************/

#endif 


