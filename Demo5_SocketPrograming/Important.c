#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Important.h"
void error_handling(char *message)//에러제어
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void getTimeStr(char *destTime)//시간출력
{
	time_t tempTime;
	time(&tempTime);
	strftime(destTime, 26, "%Y-%m-%d %H:%M:%S", localtime(&tempTime));
}
