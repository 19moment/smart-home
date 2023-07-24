#include "data_global.h"
#include "sem.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>

#define N 1024  //for share memory

extern int shmid;
extern int msgid;
extern int semid;

extern key_t shm_key;
extern key_t sem_key;
extern key_t key; //msg_key

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


extern struct env_info  sm_all_env_info;

 struct shm_addr
 {
    char shm_status;   //shm_status可以等于home_id，用来区分共享内存数据
    struct env_info  sm_all_env_info;
};
struct shm_addr *shm_buf;

int file_env_info_struct(struct env_info  *rt_status,int home_id);

void *pthread_refresh(void *arg)
{
	//semaphore for access to resource limits
	if((sem_key = ftok("/tmp",'i')) < 0){
		perror("ftok failed .\n");
		exit(-1);
	}

	semid = semget(sem_key,1,IPC_CREAT|IPC_EXCL|0666);
	if(semid == -1)	{
		if(errno == EEXIST){
			semid = semget(sem_key,1,0777);
		}else{
			perror("fail to semget");
			exit(1);
		}
	}else{
		init_sem (semid, 0, 1);
	}

	//share memory for env_info refresh config
	if((shm_key = ftok("/tmp",'i')) < 0){
		perror("ftok failed .\n");
		exit(-1);
	}

	shmid = shmget(shm_key,N,IPC_CREAT|IPC_EXCL|0666);
	if(shmid == -1)	{
		if(errno == EEXIST){
			shmid = shmget(key,N,0777);
		}else{
			perror("fail to shmget");
			exit(1);
		}
	}

	//share memap
	if((shm_buf = (struct shm_addr *)shmat(shmid,NULL,0)) == (void *)-1)
	{
		perror("fail to shmat");
		exit(1);
	}

	printf("pthread_refresh ......>>>>>>>\n");
	

	bzero (shm_buf, sizeof (struct shm_addr));
	while(1){
		sem_p(semid,0);
		pthread_mutex_lock(&mutex_transfer);
		pthread_cond_wait(&cond_transfer,&mutex_transfer);
		shm_buf->shm_status = 1;
		file_env_info_struct(&shm_buf->sm_all_env_info,shm_buf->shm_status);
		sleep(1);
		pthread_mutex_unlock(&mutex_transfer);
		sem_v(semid,0);
	}


	
}



int file_env_info_struct(struct env_info *rt_status,int home_id)
{
	int  env_info_size = sizeof(struct env_info);
	
	rt_status->temp=sm_all_env_info.temp;
	rt_status->humi=sm_all_env_info.humi;
	rt_status->MQ2=sm_all_env_info.MQ2;
	rt_status->accelx=sm_all_env_info.accelx;
	rt_status->accely=sm_all_env_info.accely;
	rt_status->accelz=sm_all_env_info.accelz;
	rt_status->gyrox=sm_all_env_info.gyrox;
	rt_status->gyroy=sm_all_env_info.gyroy;
	rt_status->gyroz=sm_all_env_info.gyroz;



	
	return 0;
}







