

#include "data_global.h"
#include "uart.h"

extern int uart_fd;
extern char led_cmd;
extern pthread_mutex_t mutex_client_request,
        		mutex_refresh,
        		mutex_sqlite,
	        	mutex_transfer,
	        	mutex_analysis,
	        	mutex_sms,
	        	mutex_buzzer,
	         	mutex_led,
	         	mutex_camera;

extern pthread_cond_t  cond_client_request,
        		cond_refresh,
        		cond_sqlite,
	        	cond_transfer,
	        	cond_analysis,
	        	cond_sms,
	        	cond_buzzer,
	         	cond_led,
	         	cond_camera;

//:A9LED模块线程.
void *pthread_led(void *arg)
{
	char led_on = 0x41;
	char led_off = 0x42;

	printf("pthread_led\n");

	while(1){
		pthread_mutex_lock(&mutex_led);
		pthread_cond_wait(&cond_led,&mutex_led);

		if(led_cmd==1){
			write(uart_fd,&led_on,1);
		}else if(led_cmd==0){
			write(uart_fd,&led_off,1);
		}
		pthread_mutex_unlock(&mutex_led);

	}



}

