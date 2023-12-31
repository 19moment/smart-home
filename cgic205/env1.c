#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>
#include <unistd.h>
#include "sem.h"
#include "cgic.h"
#include "data_global.h"

#define N 1024

#define STO_NO 1

char status[2][6] = {"Close", "Open"};
char fan_status[4][6] = {"Close", "One", "Two", "Three"};

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
 struct shm_addr
 {
    char shm_status;   //shm_status可以等于home_id，用来区分共享内存数据
    struct env_info  sm_all_env_info;
};

int cgiMain()
{
	key_t key;
	int shmid,semid;
	struct shm_addr *shm_buf;
	
	
	if((key = ftok("/tmp",'i')) <0)
	{
		perror("ftok");
		exit(1);
	}

	if((semid  = semget(key, 1, 0666)) < 0)
	{
		perror("semget");
		exit(1);
	}

	if((shmid = shmget(key, N, 0666 )) == -1)
	{
			perror("shmget");
			exit(1);
	}

	if((shm_buf = (struct shm_addr*)shmat(shmid, NULL, 0)) == (void*)-1 )
	{
		perror("shmat");
		exit(1);
	}

	sem_p(semid,0);

	cgiHeaderContentType("text/html\n\n");
	fprintf(cgiOut, "<head><meta http-equiv=\"refresh\" content=\"1\"><style><!--body{line-height:50%}--></style> </head>");
	fprintf(cgiOut, "<HTML>\n");
	fprintf(cgiOut, "<BODY bgcolor=\"#666666\">\n");
	if (shm_buf->shm_status == 1)
	{
		fprintf(cgiOut, "<h2><font face=\"Broadway\"><font color=\"#FFFAF0\">Real-time information environment</font></font></H2>\n ");	
		fprintf(cgiOut, "<h4>Temperature:\t%d</H4>\n ", shm_buf->sm_all_env_info.temp);
		fprintf(cgiOut, "<h4>Humidity:\t%d</H4>\n ", shm_buf->sm_all_env_info.humi);
		fprintf(cgiOut, "<h4>MQ2:\t%0.2f</H4>\n ", shm_buf->sm_all_env_info.MQ2);
		fprintf(cgiOut, "<h4>aX:\t%0.2f</H4>\n ", shm_buf->sm_all_env_info.accelx);
		fprintf(cgiOut, "<h4>aY:\t%0.2f</H4>\n ", shm_buf->sm_all_env_info.accely);
		fprintf(cgiOut, "<h4>aZ:\t%0.2f</H4>\n ", shm_buf->sm_all_env_info.accelz);
		fprintf(cgiOut, "<h4>gX:\t%0.2f</H4>\n ", shm_buf->sm_all_env_info.gyrox);
		fprintf(cgiOut, "<h4>gY:\t%0.2f</H4>\n ", shm_buf->sm_all_env_info.gyroy);
		fprintf(cgiOut, "<h4>gZ:\t%0.2f</H4>\n ", shm_buf->sm_all_env_info.gyroz);
		//fprintf(cgiOut, "<h4>Buzzer:\t%s</H4>\n ", status[shm_buf->sm_all_env_info.storage_no[STO_NO].buzzer_status]);
		//fprintf(cgiOut, "<h4>Led:\t%s</H4>\n ", status[shm_buf->sm_all_env_info.storage_no[STO_NO].led_status]);
		//fprintf(cgiOut, "<h4>Fan:\t%s</H4>\n ", fan_status[shm_buf->sm_all_env_info.storage_no[STO_NO].fan_status]);
		//fprintf(cgiOut, "<h4>Temperature\nMAX: %0.0f MIN: %0.0f</H4>\n ", shm_buf->sm_all_env_info.storage_no[STO_NO].temperatureMAX, shm_buf->sm_all_env_info.storage_no[STO_NO].temperatureMIN);
		//fprintf(cgiOut, "<h4>Humidity\nMAX: %0.0f MIN: %0.0f</H4>\n ", shm_buf->sm_all_env_info.storage_no[STO_NO].humidityMAX, shm_buf->sm_all_env_info.storage_no[STO_NO].humidityMIN);
		//fprintf(cgiOut, "<h4>Illumination\nMAX: %0.0f MIN: %0.0f</H4>\n ", shm_buf->sm_all_env_info.storage_no[STO_NO].illuminationMAX, shm_buf->sm_all_env_info.storage_no[STO_NO].illuminationMIN);

	}
	else
	{
		fprintf(cgiOut, "<h2><font face=\"Broadway\"><font color=\"#FFFAF0\">Close!</font></font></H2>\n ");	
	}
//	fprintf(cgiOut, "<h3>:</H3>\n ");	
	fprintf(cgiOut, "</BODY></HTML>\n");	
	sem_v (semid, 0);
	return 0;
}
