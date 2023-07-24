/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : uart_test.c
 作者 : 邓涛
 版本 : V1.0
 描述 : 串口在原始模式下进行数据传输--应用程序示例代码
 其他 : 无
 论坛 : www.openedv.com
 日志 : 初版 V1.0 2021/7/20 邓涛创建
 ***************************************************************/

#define _GNU_SOURCE     //在源文件开头定义_GNU_SOURCE宏
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <termios.h>


typedef struct uart_hardware_cfg {
    unsigned int baudrate;      /* 波特率 */
    unsigned char dbit;         /* 数据位 */
    char parity;                /* 奇偶校验 */
    unsigned char sbit;         /* 停止位 */
} uart_cfg_t;

struct sensor{
    char temp;
    char humi;
    float MQ2;
}__attribute__((packed));

static struct termios old_cfg;  //用于保存终端的配置参数
static int fd;      //串口终端对应的文件描述符
static struct sensor datas;

int recvDats(int fd,unsigned char *pBuf,int datalen)
{   
	int curlen= 0;   
	int reallen= 0;   
	while(curlen < datalen)    
	{        
		reallen = read(fd,pBuf+curlen, datalen-curlen);          
		if(reallen >= 0){           
			curlen+=reallen;        
		}else{          
			//printf("%s %d:%s\n",__FUNCTION__,__LINE__,"uart recv err");       
			return -1;       
		}  
	}  
	return 0;
}



/**
 ** 串口初始化操作
 ** 参数device表示串口终端的设备节点
 **/
static int uart_init(const char *device)
{
    /* 打开串口终端 */
    fd = open(device, O_RDWR | O_NOCTTY);
    if (0 > fd) {
        fprintf(stderr, "open error: %s: %s\n", device, strerror(errno));
        return -1;
    }

    /* 获取串口当前的配置参数 */
    if (0 > tcgetattr(fd, &old_cfg)) {
        fprintf(stderr, "tcgetattr error: %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    return 0;
}

/**
 ** 串口配置
 ** 参数cfg指向一个uart_cfg_t结构体对象
 **/
static int uart_cfg(const uart_cfg_t *cfg)
{
    struct termios new_cfg = {0};   //将new_cfg对象清零
    speed_t speed;

    /* 设置为原始模式 */
    cfmakeraw(&new_cfg);

    /* 使能接收 */
    new_cfg.c_cflag |= CREAD;

    /* 设置波特率 */
    switch (cfg->baudrate) {
    case 1200: speed = B1200;
        break;
    case 1800: speed = B1800;
        break;
    case 2400: speed = B2400;
        break;
    case 4800: speed = B4800;
        break;
    case 9600: speed = B9600;
        break;
    case 19200: speed = B19200;
        break;
    case 38400: speed = B38400;
        break;
    case 57600: speed = B57600;
        break;
    case 115200: speed = B115200;
        break;
    case 230400: speed = B230400;
        break;
    case 460800: speed = B460800;
        break;
    case 500000: speed = B500000;
        break;
    default:    //默认配置为115200
        speed = B115200;
        printf("default baud rate: 115200\n");
        break;
    }

    if (0 > cfsetspeed(&new_cfg, speed)) {
        fprintf(stderr, "cfsetspeed error: %s\n", strerror(errno));
        return -1;
    }

    /* 设置数据位大小 */
    new_cfg.c_cflag &= ~CSIZE;  //将数据位相关的比特位清零
    switch (cfg->dbit) {
    case 5:
        new_cfg.c_cflag |= CS5;
        break;
    case 6:
        new_cfg.c_cflag |= CS6;
        break;
    case 7:
        new_cfg.c_cflag |= CS7;
        break;
    case 8:
        new_cfg.c_cflag |= CS8;
        break;
    default:    //默认数据位大小为8
        new_cfg.c_cflag |= CS8;
        printf("default data bit size: 8\n");
        break;
    }

    /* 设置奇偶校验 */
    switch (cfg->parity) {
    case 'N':       //无校验
        new_cfg.c_cflag &= ~PARENB;
        new_cfg.c_iflag &= ~INPCK;
        break;
    case 'O':       //奇校验
        new_cfg.c_cflag |= (PARODD | PARENB);
        new_cfg.c_iflag |= INPCK;
        break;
    case 'E':       //偶校验
        new_cfg.c_cflag |= PARENB;
        new_cfg.c_cflag &= ~PARODD; /* 清除PARODD标志，配置为偶校验 */
        new_cfg.c_iflag |= INPCK;
        break;
    default:    //默认配置为无校验
        new_cfg.c_cflag &= ~PARENB;
        new_cfg.c_iflag &= ~INPCK;
        printf("default parity: N\n");
        break;
    }

    /* 设置停止位 */
    switch (cfg->sbit) {
    case 1:     //1个停止位
        new_cfg.c_cflag &= ~CSTOPB;
        break;
    case 2:     //2个停止位
        new_cfg.c_cflag |= CSTOPB;
        break;
    default:    //默认配置为1个停止位
        new_cfg.c_cflag &= ~CSTOPB;
        printf("default stop bit size: 1\n");
        break;
    }

    /* 将MIN和TIME设置为0 */
    new_cfg.c_cc[VTIME] = 0;
    new_cfg.c_cc[VMIN] = 0;

    /* 清空缓冲区 */
    if (0 > tcflush(fd, TCIOFLUSH)) {
        fprintf(stderr, "tcflush error: %s\n", strerror(errno));
        return -1;
    }

    /* 写入配置、使配置生效 */
    if (0 > tcsetattr(fd, TCSANOW, &new_cfg)) {
        fprintf(stderr, "tcsetattr error: %s\n", strerror(errno));
        return -1;
    }

    /* 配置OK 退出 */
    return 0;
}

/***
--dev=/dev/ttymxc2
--brate=115200
--dbit=8
--parity=N
--sbit=1
--type=read
***/
/**
 ** 打印帮助信息
 **/
static void show_help(const char *app)
{
    printf("Usage: %s [选项]\n"
        "\n必选选项:\n"
        "  --dev=DEVICE     指定串口终端设备名称, 譬如--dev=/dev/ttymxc2\n"
        "  --type=TYPE      指定操作类型, 读串口还是写串口, 譬如--type=read(read表示读、write表示写、其它值无效)\n"
        "\n可选选项:\n"
        "  --brate=SPEED    指定串口波特率, 譬如--brate=115200\n"
        "  --dbit=SIZE      指定串口数据位个数, 譬如--dbit=8(可取值为: 5/6/7/8)\n"
        "  --parity=PARITY  指定串口奇偶校验方式, 譬如--parity=N(N表示无校验、O表示奇校验、E表示偶校验)\n"
        "  --sbit=SIZE      指定串口停止位个数, 譬如--sbit=1(可取值为: 1/2)\n"
        "  --help           查看本程序使用帮助信息\n\n", app);
}

/**
 ** 信号处理函数，当串口有数据可读时，会跳转到该函数执行
 **/
static void io_handler(int sig, siginfo_t *info, void *context)
{
    char buf[sizeof(struct sensor)+2] = {0};

    int ret;
    int n;

    if(SIGRTMIN != sig)
        return;

    /* 判断串口是否有数据可读 */
    if (POLL_IN == info->si_code) {

        recvDats(fd,&buf[0],1);
        if(buf[0]==0x55){
            recvDats(fd,&buf[1],1);
            if(buf[1] == 0xaa){
                recvDats(fd,&buf[2],sizeof(struct sensor));
                memcpy(&datas,buf+2,sizeof(struct sensor));
                printf("temp = %d\n",datas.temp);
                printf("humi = %d\n",datas.humi);
                printf("temp = %f\n",datas.MQ2);
            }

        }
        
    }
}

/**
 ** 异步I/O初始化函数
 **/
static void async_io_init(void)
{
    struct sigaction sigatn;
    int flag;

    /* 使能异步I/O */
    flag = fcntl(fd, F_GETFL);  //使能串口的异步I/O功能
    flag |= O_ASYNC;
    fcntl(fd, F_SETFL, flag);

    /* 设置异步I/O的所有者 */
    fcntl(fd, F_SETOWN, getpid());

    /* 指定实时信号SIGRTMIN作为异步I/O通知信号 */
    fcntl(fd, F_SETSIG, SIGRTMIN);

    /* 为实时信号SIGRTMIN注册信号处理函数 */
    sigatn.sa_sigaction = io_handler;   //当串口有数据可读时，会跳转到io_handler函数
    sigatn.sa_flags = SA_SIGINFO;
    sigemptyset(&sigatn.sa_mask);
    sigaction(SIGRTMIN, &sigatn, NULL);
}

int main(int argc, char *argv[])
{
    uart_cfg_t cfg = {0};
    char *device = "/dev/ttymxc2";
    int rw_flag = -1;
    unsigned char w_buf[2] = {0x41, 0x51};    //通过串口发送出去的数据
    int n;
    char cmd=0x51;
    char buf[sizeof(struct sensor)+2] = {0};


    /* 串口初始化 */
    if (uart_init(device))
        exit(EXIT_FAILURE);

    /* 串口配置 */
    if (uart_cfg(&cfg)) {
        tcsetattr(fd, TCSANOW, &old_cfg);   //恢复到之前的配置
        close(fd);
        exit(EXIT_FAILURE);
    }
    write(fd, w_buf, 2);
    sleep(1);
    write(fd, &cmd, 1);


    

   while( 1 ){
        memset(buf,0,sizeof(buf));

        recvDats(fd,&buf[0],1);
        if(buf[0]==0x55){
            recvDats(fd,&buf[1],1);
            if(buf[1] == 0xaa){
                recvDats(fd,&buf[2],sizeof(struct sensor));
                memcpy(&datas,buf+2,sizeof(struct sensor));
                printf("temp = %d\n",datas.temp);
                printf("humi = %d\n",datas.humi);
                printf("temp = %.2f\n",datas.MQ2);
            }

        }
        
    }
   }
            

    /* 读|写串口 */

