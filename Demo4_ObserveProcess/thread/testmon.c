#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
int TestExit(char * name)//test를 출력할때 사용하기 위한 함수
{
	char buf[50];
	FILE * temp;
	int count;
	sprintf(buf, "pgrep -c %s$",name); //test의 입력이 받아 졌을때 test를 카운트 한다
	temp = popen(buf, "r");
	fscanf(temp, "%d", &count);
	pclose(temp);
	return count;
}

void pressKeyboard(int signal)//커맨드를 입력받아 동작할때 사용하기위한 함수
{
	pid_t pid;
	FILE * readFile;
	int child_pid;//자식 프로세스를 파일 입력력을 통해 구분한다.(test)
	char c;
	char temp[100];
	char quit[100];
	char kill[100];
	char restart[100];
	int status;
	while( (c=getchar())==EOF || c=='\n');
	switch(c)
	{
		case 'Q' ://2개의 프로세스 종료
			readFile = popen("pgrep test$", "r");
                        fscanf(readFile, "%d", &child_pid);
                        pclose(readFile);
                        sprintf(quit,"kill -9 %d",child_pid);
                        system(quit);
                        wait(&status);
			exit(1);
		case 'K' ://test 종료
			if(TestExit("test"))
			{
				readFile = popen("pgrep test$", "r");
				fscanf(readFile, "%d", &child_pid);
				pclose(readFile);
				sprintf(kill,"kill -9 %d",child_pid);
				system(kill);
				wait(&status);
			}
			else
			{
				printf("already terminated\n");
			}
			break;
		case 'S' ://test가 꺼져 있을때 실행
			if(TestExit("test"))
			{
				printf("already executed\n");
			}
         		else
			{
				wait(&status);
				pid = fork();
				switch(pid)
				{
					case -1:
						printf("fork failed\n");
                				exit(1);
					case 0:
						execl("./test", "test", NULL);
				}
			}
			break;
		case 'R' ://test를 종료후 재실행
			if(TestExit("test") == 1)
			{
				readFile = popen("pgrep test$", "r");
				fscanf(readFile, "%d", &child_pid);
				pclose(readFile);
				sprintf(restart,"kill -9 %d",child_pid);
				system(restart);
				wait(&status);
			}
			pid = fork();
			switch(pid)
			{
				case -1:
					printf("fork failed\n");
					exit(1);
				case 0:
					printf("restart\n");
					execl("./test", "test", NULL);
				default:
					sleep(1);
			}
         		break;
		default :
			printf("wrong command\n");
			break;
	}
}

int getCharacter = 0;//PID값 출력
void * t_function(void *arg)
{
	FILE * statusFile;
	char buf[200];
	printf("thread_function :: thread start\n");
	while(1)
	{
		if(TestExit("test"))
		{
			statusFile = popen("ps -ef | grep test$", "r");
			fgets(buf, 200, statusFile);
			pclose(statusFile);
			printf("Current process PID : %s", buf);
			getCharacter = 1;
			sleep(5);
		}
		else
		{
			getCharacter = 1;
		}
	}
}

int main(void)
{
	int a;
	pthread_t tid;
	a = pthread_create(&tid, NULL, t_function, NULL);
	if(a != 0)
	{
		perror("Thread creation failed");
		exit(0);
	}
	while(1)
	{
		if(getCharacter == 1)
		{
			pressKeyboard(0);
			getCharacter = 0;
		}
	}
	return 0;
}
