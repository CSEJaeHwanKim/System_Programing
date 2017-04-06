#ifndef PRACTICAL_H_
#define PRACTICAL_H_
void error_handling(char *message);
void getTimeStr(char *destTime);
struct Message
{
	char m_userName[20];//입력받는 사용자 이름
  	char m_buffer[256];
  	char m_time[30];//시간
};
	#define BUFSIZE 1024
	#define MAX_USER 50
#endif
