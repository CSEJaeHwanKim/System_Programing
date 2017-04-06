#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Important.h"

int main(int argc, char *argv[])
{
	int temp;
        char userName[20];
	char err[100];
        struct Message msg;
	fd_set status;
	int connectnum=0;
	if(argc != 3)
	{
		printf("서버 접속을 위한 입력 정보 : %s <서버주소> <서보 포트번호>\n", argv[0]);
		exit(1);
	}
	char *servIP = argv[1];//서버의 아이피주소
	in_port_t servPort = atoi(argv[2]);//서버의 포트번호
	// socket() : 소켓 생성
	int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock == -1)//에러발생
	{
		error_handling("socket error");
	}
	// connect() : 서버에 접속
 	struct sockaddr_in serv_addr;
 	memset(&serv_addr, 0, sizeof(serv_addr));
 	serv_addr.sin_family = AF_INET;
 	serv_addr.sin_addr.s_addr = inet_addr(servIP);
 	serv_addr.sin_port = htons(servPort);
 	connectnum=connect(sock, &serv_addr, sizeof(serv_addr));
 	if(connectnum == -1)
	{
		printf("return error = %d\n",connectnum);
  		error_handling("connect error");


 	}
	printf("클라이언트가 만들어 졌습니다\n");
	// nickname 입력 받기
 	printf("닉네임을 입력하세요 : ");
 	fflush(stdout);
 	temp = read(0, userName, sizeof(userName));
 	userName[temp-1] = '\0';
	// 클라이언트 접속 정보를 서버에 보내기
	strcpy(msg.m_userName, userName);
	strcpy(msg.m_buffer, userName);
 	strcat(msg.m_buffer, "님이 접속하였습니다.");
 	getTimeStr(msg.m_time);
 	write(sock, &msg, sizeof(msg));
	while(1)
	{
		FD_ZERO(&status); // 0으로 초기화
  		FD_SET(0, &status); // stdin을 set
  		FD_SET(sock, &status); // socket을 set
  		temp = select(sock+1, &status, 0, 0, 0);
		// select() 에러 처리
  		if(temp < 0)
		{
   			printf("select error\n");
   			write(sock, "/quit", sizeof("/quit"));
   			break;
  		}
		// stdin에 입력이 들어왔을 때
  		if(FD_ISSET(0, &status) == 1)
		{
   			temp = read(0, msg.m_buffer, sizeof(msg.m_buffer));
   			msg.m_buffer[temp-1] = '\0';
   			strcpy(msg.m_userName, userName);
   			getTimeStr(msg.m_time);
			// 입력이 ENQ(Ctrl+E) 이거나 "/quit"일 경우, 접속 종료
   		if(msg.m_buffer[0] == 0x05 || strcmp(msg.m_buffer, "/quit") == 0)
		{
    			printf("Terminate Client...\n");
    			strcpy(msg.m_buffer, "/quit");
    			write(sock, &msg, sizeof(msg));
    			break;
   		}
		write(sock, &msg, sizeof(msg));
  		}
  		// 서버로부터 메세지가 왔을 때
  		else if(FD_ISSET(sock, &status) == 1)
		{
   			temp = read(sock, &msg, sizeof(msg));
   			if(temp <= 0)
			{
    				printf("서버와 접속이 끊겼습니다. Terminate Client...\n");
    				break;
   			}
			if(strcmp("/quit", msg.m_buffer) == 0)
			{
				printf("Server is terminated. Terminate Client...\n");
    				break;
   			}
			printf("[%s : ", msg.m_userName);
   			printf("%s ", msg.m_buffer);
   			printf("(%s)]\n", msg.m_time);
  		}
  		usleep(100000); // 0.1s delay
 	} // End of while(1)
	// close() : 소켓 종료
 	close(sock);
	return 0;
}
