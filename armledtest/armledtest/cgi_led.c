/*************************************************************************
	#	 FileName	: test.c
	#	 Author		: fengjunhui 
	#	 Email		: 18883765905@163.com 
	#	 Created	: 2017年07月03日 星期一 15时48分02秒
 ************************************************************************/



#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/limits.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "chardev.h"


#define LED_DEVICE "/sys/class/leds/sys-led/brightness"
#define MAX_BUFFER_SIZE PIPE_BUF

int main(int argc, const char *argv[])
{
	int i = 0,j = 3;
	int nread;
	int led_control,led_state;
	int led_fd,fifo_fd;
	led_desc_t led;
	char *data;

	led_fd = open(LED_DEVICE,O_RDWR);
	if(led_fd < 0){
		printf("open failed !\n");
	}
	printf("open device success! led_fd: %d\n",led_fd);

    printf("Content-type: text/html;charset=utf-8\n\n");
    printf("<html>\n");
    printf("<head><title>cgi control led web</title></head>\n");
    printf("<body>\n");
    printf("<p>led is setted successful! you can watch the led's change</p>\n");
    //printf("<p><a herf=http://192.168.1.100/led.html>go back</a></p>\n");
	printf("<a href=\"/led.html\">go back led control page </a>");
	printf("</body>\n");

    data = getenv("QUERY_STRING");   //getenv()读取环境变量的当前值的函数 

    if(sscanf(data,"led_control=%d&led_state=%d",&led_control,&led_state)!=2)
    {   //利用sscnaf（）函数的特点将环境变量分别提取出led_control和led_state这两个值
        printf("<p>please input right"); 
        printf("</p>");
    } 
    printf("<p>led_control = %d,led_state =  %d</p>", led_control, led_state);
    if(led_control < 2 || led_control > 5) { 
        printf("<p>Please input 2<=led_control<=5!"); 
        printf("</p>");
    } 
    if(led_state>1) {
        printf("<p>Please input 0<=led_state<=1!"); 
        printf("</p>"); 
    }

	led.led_num   = led_control;
	led.led_state = led_state;
	
	if(led.led_state == 0){
		write(led_fd,"0",1);
	}else if(led.led_state == 1){
		write(led_fd,"1",1);
	}

	close(led_fd);
    printf("</html>\n");

	return 0;
}
