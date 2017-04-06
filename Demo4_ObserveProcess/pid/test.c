#include <stdio.h>
#include <time.h>
int main()
{
	time_t _time;
	struct tm *ptm;
	while(1)
	{
		_time=time(NULL);
		ptm=localtime(&_time);
		printf("localtime(): %d.%d.%d %d:%d:%d\n", ptm->tm_year+1900, ptm->tm_mon+1,ptm->tm_mday, ptm->tm_hour, ptm->tm_min,ptm->tm_sec);
		sleep(2);
	}
	return 0;
}
