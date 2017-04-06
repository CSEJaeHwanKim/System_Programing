#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Important.h"
int main(int argc, char *argv[])
{
	int i, j;
	fd_set status;
        struct Message msg;//보내는 메세지
        int currUserCnt = 0;//현재 접속해 있는 사용자의 수
        int clientSock[MAX_USER], tempSock;
        int temp;
        time_t tempTime;
        unsigned char clientBuffer[512];//접속하기전에 클라이언트 주소를 일시적으로 저장하기 위한 선언
        struct sockaddr_in client_addr;//Client의 주소값 저장
        socklen_t client_addr_len = sizeof(client_addr);//Client의 크기 저장
        char userNameArr[MAX_USER][20];
	if(argc != 2)
	{
		printf("서버생성을 위한 입력 정보 : %s <포트번호>\n", argv[0]);
		exit(1);
 	}
	in_port_t servPort = atoi(argv[1]);//port 번호 숫자로 변환하여 지정
	// socket()
 	// Domain : internet networking
 	// Type : stream socket (TCP)
	int serv_sock = socket(PF_INET, SOCK_STREAM, 0);//TCP/IP이용을 위한 기본 인터넷 프로토콜 지정, TCP사용, 미리 타입정해져 있을경우 0
 	if(serv_sock == -1)//소켓을 받아 올때 에러가 발생 했을시 -1 반환
		error_handling("socket error");
	// bind() 절
	struct sockaddr_in serv_addr;//소켓 주소를 표현하는 구조체, IPv4사용
 	memset(&serv_addr, 0, sizeof(serv_addr));
 	serv_addr.sin_family = AF_INET; // IPv4사용하므로
 	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IP 주소 저장, 서버에 연결괸 네트워크 인터 페이스를 목적지로 해서 들어오는 모든 자료 수신 가능
 	serv_addr.sin_port = htons(servPort); // Port 번호 저장, 서버의 주소와 포트 번호는 IP헤더에 저장되어 전송되는데 이를 중계하는 라우터들은 항상 네트워크 바이트 방식
 	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)// 생성한 소켓을 서버 소케으로 등록
	{
  		close(serv_sock);
  		error_handling("bind error");
 	}
	// listen()
	if(listen(serv_sock, 5) == -1)//서버 소켓을 통해 클라이언트의 접속 요청을 확인하도록 설정
	{
  		close(serv_sock);
  		error_handling("listen error");
	}
	printf("챗팅 서버가 만들어 졌습니다\n");//서버가 만들어 졌을때 출력
	int maxSockNum = serv_sock + 1;//최대 소켓의 수
	while(1)
	{
  		FD_ZERO(&status);//status의 모든 비트를 지운다
  		FD_SET(0, &status); //status를  0으로 초기화;
  		FD_SET(serv_sock, &status);//status를 serv_sock값 대입
		// maxSockNum 갱신
  		for(i = 0; i < currUserCnt; i++)
		{
			FD_SET(clientSock[i], &status);
			if(maxSockNum <= clientSock[i])
			{
				maxSockNum = clientSock[i] + 1;
			}
  		}
		temp = select(maxSockNum, &status, NULL, NULL, NULL);
  		if(temp < 0)
		{
			printf("select error\n");
   			strcpy(msg.m_userName, "Server");//유저이름
   			strcpy(msg.m_buffer, "select error");
   			getTimeStr(msg.m_time);//시간
			for(i = 0; i < currUserCnt; i++)
			{
				write(clientSock[i], &msg, sizeof(msg));
			}
			strcpy(msg.m_buffer, "/quit");
   			for(i = 0; i < currUserCnt; i++)
			{
				write(clientSock[i], &msg, sizeof(msg));
			}
			break;
  		}
		// 새로운 클라이언트가 접속했을 때
  		if(FD_ISSET(serv_sock, &status) == 1)//서버가 만들어 질경우 1 에러 -1 아무것도 없을 경우 0
		{// accept
   			tempSock = accept(serv_sock, (struct sockaddr *)&client_addr, &client_addr_len);
			if(tempSock < 0)
			{
    				printf("accept error\n");
    				continue;
   			}
			printf("----------------------------------------\n");
   			printf("접속 성공. 소켓 번호 : %d\n", tempSock);
   			printf("새로운 Client가 접속했습니다. IP (port)번호 : %s (%d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
			if(currUserCnt >= MAX_USER)
			{
    				close(tempSock);
    				printf("error : 최대 접속자 수 초과\n");
    				continue;
   			}
			clientSock[currUserCnt] = tempSock;
   			currUserCnt++;
			printf("현재 접속자 수 : %d\n", currUserCnt);
   			printf("----------------------------------------\n");
  		}
  		// stdin에서 입력이 되었을 때
  		else if(FD_ISSET(0, &status) == 1)
		{
   			temp = read(0, msg.m_buffer, sizeof(msg.m_buffer));
   			msg.m_buffer[temp-1] = '\0';
   			strcpy(msg.m_userName, "Server");
   			getTimeStr(msg.m_time);
			// ENQ(Ctrl+E) 또는 "/quit"이 입력되었을 때 클라이언트에게 서버를 종료를 알리고 서버를 종료한다.
   			if(msg.m_buffer[0] == 0x05 || strcmp(msg.m_buffer, "/quit") == 0)
			{
    				strcpy(msg.m_buffer, "/quit");
				for(i = 0; i < currUserCnt; i++)
				{
					write(clientSock[i], &msg, sizeof(msg));
    				}
    				break;
   			}
			for(i = 0; i < currUserCnt; i++)
			{
				write(clientSock[i], &msg, sizeof(msg));
   			}
  		}
		// 각 클라이언트로부터 받은 메세지를 확인
  		else
		{
			for(i = 0; i < currUserCnt; i++)
			{
    				if(FD_ISSET(clientSock[i], &status) == 1)
				{
					temp = read(clientSock[i], &msg, sizeof(msg));
					// 클라이언트의 접속이 끊겼거나 클라이언트로부터 "/quit" 명령을 받았을 때, 접속을 해제함
     				if(temp <= 0 || strcmp("/quit", msg.m_buffer) == 0)
				{
					close(clientSock[i]);
					clientSock[i] = clientSock[currUserCnt-1];
      					currUserCnt--;
					strcpy(msg.m_buffer,userNameArr[clientSock[i]]);
      					strcpy(msg.m_buffer, msg.m_userName);
      					strcpy(msg.m_userName, "Server");
      					strcat(msg.m_buffer, "님이 나갔습니다.");
      					getTimeStr(msg.m_time);
      					printf("%s\n", msg.m_buffer);
      					for(j = 0; j < currUserCnt; j++)
					{
       						write(clientSock[j], &msg, sizeof(msg));
      					}
      					i--;
      					continue;
     				}
				strcpy(userNameArr[clientSock[i]],msg.m_userName);
				// '/'로 시작하는 명령을 처리하는 부분 (현재 구현한 기능은 없음)
     				if(msg.m_buffer[0] == '/')
				{
					strcpy(msg.m_userName, "Server");
      					strcpy(msg.m_buffer, "Under constructing...");
      					write(clientSock[i], &msg, sizeof(msg));
     				}
     				// 클라이언트로부터 받은 메세지를 출력하고 다른 클라이언트들에게 broadcast
     				else
				{
					printf("[%s (%s) : ", msg.m_userName, inet_ntoa(client_addr.sin_addr));
  					printf("%s ", msg.m_buffer);
      					printf("(%s)]\n", msg.m_time);
      					for(j = 0; j < currUserCnt; j++)
					{
       						write(clientSock[j], &msg, sizeof(msg));
      					}
     				}
    			}
   		}

  	}
  	usleep(100000);
 } // End of while(1)
 
 		printf("Terminate Server...\n");
	 	close(serv_sock);
		for(i = 0; i < currUserCnt; i++)
		{
			close(clientSock[i]);
		}
	return 0;
}
