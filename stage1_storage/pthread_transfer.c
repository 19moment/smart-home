#include "data_global.h"
#include "uart.h"


struct sensor{
    char temp;
    char humi;
    float MQ2;
}__attribute__((packed));

extern int uart_fd;
int ic_fd;
extern struct env_info  sm_all_env_info;


extern pthread_cond_t cond_transfer;
extern pthread_mutex_t mutex_transfer;

void getdata_form_a9(struct env_info *data);
void getdata_form_stm32(struct env_info *data);


//接收ZigBee的数据和采集的A9平台的传感器数据

void *pthread_transfer(void *arg)
{	
	stm32_uart_int();
	
	ic_fd = open("/dev/icm20608",O_RDWR);
	if((ic_fd==-1)){
		printf("open device failed.\n");
	}
	while(1){
		pthread_mutex_lock(&mutex_transfer);
		printf("pthread_analysis and tranfer.\n");

		
		getdata_form_a9(&sm_all_env_info);
		getdata_form_stm32(&sm_all_env_info);

		pthread_mutex_unlock(&mutex_transfer);
		pthread_cond_signal(&cond_transfer);
	}
	close(ic_fd);
	
}

void getdata_form_stm32(struct env_info *data)
{
	struct sensor m3_data;
	char buf[8];
	char a;
	memset(buf,0,sizeof(buf));
	while(a!=0x55){
		recvDats(uart_fd,&buf[0],1);
		a=buf[0];
	}
	while(a!=0xaa){
		recvDats(uart_fd,&buf[1],1);
		a=buf[1];
	}
	recvDats(uart_fd,&buf[2],6);
	memcpy(&m3_data,buf+2,sizeof(struct sensor));

	data->temp = m3_data.temp;
	data->humi = m3_data.humi;
	data->MQ2 = m3_data.MQ2;

}

void getdata_form_a9(struct env_info *data)
{
	int ret;
	signed int databuf[7];
	memset(databuf,0,sizeof(databuf));

	ret=read(ic_fd,databuf,sizeof(databuf));
	
	if(ret==0){
		data->gyrox = (float)(databuf[0]) / 16.4;
		data->gyroy = (float)(databuf[1]) / 16.4;
		data->gyroz = (float)(databuf[2]) / 16.4;
		data->accelx = (float)(databuf[3]) / 2048;
		data->accely = (float)(databuf[4]) / 2048;
		data->accelz = (float)(databuf[5]) / 2048;
				}
	printf("ax= %.2f  ay = %.2f  az = %.2f\n",data->accelx\
			,data->accely,data->accelz);

	
}
