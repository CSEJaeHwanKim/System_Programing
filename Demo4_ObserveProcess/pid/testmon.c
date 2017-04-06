#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
static struct termios initial_settings, new_settings;
static int peek_character = -1;

void init_keyboard()
{
	tcgetattr(0,&initial_settings);
 	new_settings = initial_settings;
 	new_settings.c_lflag &= ~ICANON;
 	new_settings.c_lflag &= ~ECHO;
 	new_settings.c_cc[VMIN] = 1;
 	new_settings.c_cc[VTIME] = 0;
 	tcsetattr(0, TCSANOW, &new_settings);
}

void close_keyboard()
{
	tcsetattr(0, TCSANOW, &initial_settings);
}

int kbhit()
{
	unsigned char ch;
 	int nread;
 	if (peek_character != -1) return 1;
 	new_settings.c_cc[VMIN]=0;
 	tcsetattr(0, TCSANOW, &new_settings);
 	nread = read(0,&ch,1);
 	new_settings.c_cc[VMIN]=1;
 	tcsetattr(0, TCSANOW, &new_settings);
 	if(nread == 1)
 	{
		peek_character = ch;
		return 1;
	}
 	return 0;
}

int getch()
{
 	char ch;
	if(peek_character != -1)
 	{
  		ch = peek_character;
  		peek_character = -1;
  		return ch;
 	}
 	read(0,&ch,1);
 	return ch;
}

int main(void)
{
 	FILE *rp;
 	char buf[BUFSIZ + 1], temp[50];
 	char cmd,flag,ent;
 	int status,pidNum,cnt,k,j;
 	double i;
 	pid_t pid;
	rp = popen("pgrep test$", "r");
	if(fgets(buf,BUFSIZ + 1,rp) != NULL)
 	{
  		pclose(rp);
  		while(1)
  		{
   			rp = popen("pgrep test$", "r");
   			if(fgets(buf,BUFSIZ + 1,rp) != NULL)
   			{	
    				printf("\n\n\n");
    				printf("test information\n");
    				sprintf(temp, "ps -f %s", buf);
    				system(temp);
   			}
   			else
   				printf("\n\ntest not found\n");
   				printf(">> operator command : ");
   				fflush(stdout);
   				pclose(rp);
   				i=0;
   				cmd = '0';
   				init_keyboard();
   				while(i<5)
   				{	
    					if(kbhit())
    					{
     						flag = getch();
     						putchar(flag);
     						fflush(stdout);
     						if(flag == '\n')
      							break;
     						else
      							cmd = flag;
    					}
   					usleep(10000);
   					i += 0.01;
   				}
   			close_keyboard();
   			switch(cmd) {
    				case 'Q': case 'q':
     					printf(">> operator message : testmon close\n");
					rp = popen("pgrep test$", "r");
                                        fgets(buf,BUFSIZ + 1, rp);
                                        pidNum = atoi(buf);
                                        kill(pidNum,SIGTERM);
     					exit(1);
    				case 'K': case 'k':
     					rp = popen("pgrep test$", "r");
     					fgets(buf,BUFSIZ + 1, rp);
     					pidNum = atoi(buf);
     					kill(pidNum,SIGTERM);
     					printf(">> operator message : test close\n");
     					while(!waitpid(pidNum,&status,0));
     						pclose(rp);
     						break;
    				case 'S': case 's':
     					rp = popen("pgrep test$", "r");
     					if(fgets(buf,BUFSIZ + 1,rp) != NULL)
     					{
      						printf(">> operator message : test already executed\n");
      						fflush(stdout);
     					}
     					else
     					{
      						pid = fork();
      						switch(pid) {
       							case -1:
        							perror("fork failed"); exit(1);
       							case 0:
        							printf(">> operator message : test start.\n");
        							execl("./test","test",(char *)NULL);
        							break;
       							default:
        							break;
      						}
     					}
     					pclose(rp);
     					break;
    				case 'R': case 'r':
     					rp = popen("pgrep test$", "r");
     					fgets(buf,BUFSIZ + 1, rp);
     					pidNum = atoi(buf);
     					kill(pidNum, SIGTERM);
     					while(!waitpid(pidNum,&status,0));
     						pid = fork();
     						switch(pid) {
      							case -1:
       								perror("fork failed"); exit(1);
      							case 0:
       								printf("test ReStart.\n");
       								execl("./test","test",(char *)NULL);
       								break;
      							default:
       								break;
     						}
     					pclose(rp);
     					break;
    				default:
     					break;
   			}
   			if(i < 5)
    				usleep(5000000 - (i * 1000000));
  			}
 		}
 			else
 			{
  				pid = fork();
  				switch(pid)
  				{
   					case -1:
    						perror("fork failed"); exit(1);
   					case 0:
    						printf("test is terminated.\n");
    						execl("./test","test",(char *)NULL);
    						break;
   					default:
    						execl("./testmon","testmon",(char *)NULL);
    						break;
  				}
 			}
 		pclose(rp);
}
