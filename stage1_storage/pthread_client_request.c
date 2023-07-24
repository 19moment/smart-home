#include "data_global.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>

extern int msgid;
extern key_t key;

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

extern char recive_phone[12] ;
extern char center_phone[12] ;


struct msg msgbuf;
char led_cmd,beep_cmd;



//:处理消息队列里请求的线程
void *pthread_client_request(void *arg)
{
	if((key = ftok("/tmp",'g')) < 0){
		perror("ftok failed .\n");
		exit(-1);
	}

	msgid = msgget(key,IPC_CREAT|IPC_EXCL|0666);
	if(msgid == -1)	{
		if(errno == EEXIST){
			msgid = msgget(key,0777);
		}else{
			perror("fail to msgget");
			exit(1);
		}
	}
	printf("pthread_client_request\n");
	
	while(1){
		bzero(&msgbuf,sizeof(msgbuf));
			printf("wait form client request...\n");
		msgrcv (msgid, &msgbuf, sizeof (msgbuf) - sizeof (long), 1L, 0);
		printf ("Get %ldL msg\n", msgbuf.msgtype);
		printf ("text[0] = %#x\n", msgbuf.text[0]);

		//1L~5L为线程分类的消息类型
		switch(msgbuf.msgtype){
			case 1L:
					pthread_mutex_lock(&mutex_led);
					led_cmd = msgbuf.text[0];
					pthread_mutex_unlock(&mutex_led);
					pthread_cond_signal(&cond_led);
				break;
			case 2L:
					pthread_mutex_lock(&mutex_buzzer);
					beep_cmd = msgbuf.text[0];
					pthread_mutex_unlock(&mutex_buzzer);
					pthread_cond_signal(&cond_buzzer);
				break;
			case 3L:
					printf("hello seg\n");
				break;
			case 4L:
					printf("hello fan\n");
				break;
			
			case 5L:
				

				break;
			case 6L:
			case 7L:
			case 8L:
			case 9L:
					
				break;
			case 10L:
			
				 
				break;
			default:
				break;
				
		}
	}

}


#if 0

		long msgtype;//鍏蜂綋鐨勬秷鎭被鍨?
		娑堟伅绫诲瀷鐨勫垎閰嶏細
			1L: 		LED鎺у埗
			2L:			铚傞福鍣ㄦ帶鍒?
			3L:			鍥涜矾LED鐏ā鎷熺殑鏁扮爜绠?
			4L:			椋庢墖
			5L:			娓╂箍搴︽渶鍊艰缃?
			6L-7L-8L-9L,鐢ㄤ簬涓汉鐨勬墿灞?
			10L: 		3G閫氫俊妯″潡-GPRS 
		switch(msgbuf.msgtype){
			case 1L: ...  break;
			....
			default ....  break;
		}
#endif 





