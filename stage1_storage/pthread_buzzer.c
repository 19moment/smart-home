#include "data_global.h"
#include "uart.h"

extern int uart_fd;
extern char beep_cmd;
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

//:A9蜂鸣器控制线程.
void *pthread_buzzer(void *arg)
{
	printf("pthread_buzzer\n");

	char beep_on = 0x51;
	char beep_off = 0x52;

	printf("pthread_led\n");

	while(1){
		pthread_mutex_lock(&mutex_buzzer);
		pthread_cond_wait(&cond_buzzer,&mutex_buzzer);

		if(beep_cmd==1){
			write(uart_fd,&beep_on,1);
		}else if(beep_cmd==0){
			write(uart_fd,&beep_off,1);
		}
		pthread_mutex_unlock(&mutex_buzzer);

	}
}



