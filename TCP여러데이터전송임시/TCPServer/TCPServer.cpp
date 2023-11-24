#include "..\Common.h"

#define SERVERPORT 9000
#define BUFSIZE    512

#define MAXPLAYERDATANUM 10 // 최대 플레이어 데이터 개수 (동시접속 수 아님)
#define NEEDPLAYERNUM    3  // 필요한 플레이어 수


char playerdatas[MAXPLAYERDATANUM][10][BUFSIZE + 1] = {}; // 플레이어 데이터 저장

// -------------- 데이터 통신에 사용할 변수 --------------

SOCKET client_sock;
struct sockaddr_in clientaddr;
int addrlen;

char received_data[BUFSIZE + 1];

char ready_data[MAXPLAYERDATANUM][2]; // 레디 여부 저장
char saved_data[MAXPLAYERDATANUM][BUFSIZE + 1];

int now_received_data_num = 0;

int iCountReady = 0;

// -------------- addClient() 함수 --------------

int addClient(char* name, char* playersize)
{
	printf("addClient() 함수가 실행되었습니다\n");
	for (int i = 0; i < MAXPLAYERDATANUM; i++)
	{
		// 남는 공간이 있다면
		if (strcmp(playerdatas[i][0], "") == 0)
		{
			strcpy(playerdatas[i][0], name);
			strcpy(playerdatas[i][1], playersize);
			return i;
		}

		// 모든 공간이 찼다면
		else if (i == MAXPLAYERDATANUM - 1)
		{
			printf("데이터를 추가할 공간이 없습니다\n");
			return -1;
		}
	}
	return 0;
}

// -------------- checkIfDataExist() 함수 --------------

bool checkIfDataExist()
{
	printf("checkIfDataExist() 함수가 실행되었습니다\n");

	// 클라이언트로부터 받은 데이터가 서버에 존재하는지 확인하고
	// 존재하지 않는다면 받은 데이터를 서버에 추가하는 코드이다

	bool bIfDataExist = false; // 데이터가 서버에 이미 존재하는지 확인

	for (int i = 0; i < MAXPLAYERDATANUM; i++)
	{
		// 데이터가 없다면
		if (strcmp(playerdatas[i][0], "") == 0)
		{
			// 루프에서 나간다
			break;
		}
		// 받은 이름이 동일하게 존재한다면
		else if (strcmp(playerdatas[i][0], saved_data[0]) == 0)
		{
			// 데이터가 서버에 이미 존재함
			printf("저장된 이름에 해당하는 데이터가 서버에 있습니다\n");
			bIfDataExist = true;
		}
	}

	// 서버에 저장된 데이터가 아니라면
	if (!bIfDataExist)
	{
		// 계정을 추가한다
		int num = addClient(saved_data[0], saved_data[1]);
		printf("%d번째 플레이어 저장 완료\n", num + 1);

		return false; // 데이터가 존재하지 않음
	}
	else
	{
		return true; // 데이터가 존재함
	}
}


// Thread ProcessClient
DWORD WINAPI ProcessClient(LPVOID arg)
{
	printf("임시 스레드 생성\n");

	return 0;
}

void RecvReadyFromClient(SOCKET listen_sock, int retval, int playernum)
{
	printf("RecvReadyFromClient() 함수가 실행되었습니다.\n");
	printf("클라이언트 ReadyMessage를 받습니다.\n");
	
	// accept()
	addrlen = sizeof(clientaddr);
	client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		err_display("accept()");
		return;
	}

	// 임시 Handle Create
	HANDLE hThread;

	// 스레드 생성
	hThread = CreateThread(NULL, 0, ProcessClient,
		(LPVOID)client_sock, 0, NULL);

	// 접속한 클라이언트 정보 출력
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));

	retval = recv(client_sock, received_data, BUFSIZE, 0);

	printf("Ready 정보 recv 완료\n");

	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return;
	}
	else if (retval == 0)
		return;

	// 받은 데이터를 ready_data의 해당하는 위치에 넣는다
	received_data[retval] = '\0';
	strcpy(ready_data[playernum], received_data);

	// 소켓 닫기
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));

	printf("RecvReadyFromClient() 함수가 종료되었습니다\n\n");
}

// recvDataFromSocket() 함수

void RecvDataFromClient(SOCKET listen_sock, int retval, int playernum)
{
	printf("RecvDataFromClient() 함수가 실행되었습니다\n");
	printf("클라이언트의 Send()를 기다리고 있습니다\n");


	// accept()
	addrlen = sizeof(clientaddr);
	client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		err_display("accept()");
		return;
	}

	// 임시 Handle Create
	HANDLE hThread;

	// 스레드 생성
	hThread = CreateThread(NULL, 0, ProcessClient,
		(LPVOID)client_sock, 0, NULL);
	
	// 접속한 클라이언트 정보 출력
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));

	retval = recv(client_sock, received_data, BUFSIZE, 0);

	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return;
	}
	else if (retval == 0)
		return;

	// 받은 데이터를 saved_data의 해당하는 위치에 넣는다
	received_data[retval] = '\0';
	strcpy(saved_data[playernum], received_data);

	// 소켓 닫기
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));

	printf("RecvDataFromClient() 함수가 종료되었습니다\n\n");
}


// -------------- main 함수 --------------

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	printf("[게임 서버를 열었습니다]\n\n");

	// 현재 플레이어 번호
	int iplayernum = 1;
	
	// 클라이언트로부터 데이터를 가져와 0번 위치에 저장한다
	RecvDataFromClient(listen_sock, retval, 0);
	printf("플레이어 이름 데이터 가져오기 완료\n");

	// 이름이 저장되어 있는지 확인
	bool bDataExist = checkIfDataExist();
	if (now_received_data_num == 0)
		if (bDataExist)
		{
			// 이미 저장된 이름일 경우 넘어간다
			printf("이미 존재하는 이름입니다\n");
		}
		else
		{
			printf("서버에 %d번째 플레이어 이름을 추가하였습니다\n\n", iplayernum);

			// 클라이언트로부터 크기 데이터를 가져와 1번 위치에 저장한다
			// RecvDataFromClient(listen_sock, retval, 1);
			// printf("%d번째 플레이어 크기 데이터 가져오기 완료\n", iplayernum);

			// 데이터를 playerdatas[]의 현재 플레이어 위치에 저장한다
			for (int i = 0; i < 1; i++)
			{
				strcpy(playerdatas[iplayernum - 1][i], saved_data[i]);
			}

			// 플레이어 데이터 받아올 위치를 다음으로 이동
			iplayernum++;
		}

	printf("플레이어 데이터 가져오기 완료\n\n");
	
	// 플레이어의 레디 데이터를 받는다
	RecvReadyFromClient(listen_sock, retval, 0);
	
	printf("Ready 데이터 수신 완료\n");

	for (int i = 0; i < NEEDPLAYERNUM; i++)
	{
		if (strcmp(ready_data[i], "Ready") == 0) printf("%d번째 플레이어 : 준비됨\n", i + 1);
		else printf("%d번째 플레이어 : 준비 안 됨\n", i + 1);
	}

	// ### 테스트용 : 플레이어 데이터 출력
	for (int i = 0; i < NEEDPLAYERNUM; i++)
	{
		printf("\n[%d번째 플레이어 데이터]\n", i + 1);
		printf("플레이어 이름 : %s\n", playerdatas[i][0]);
		// printf("플레이어 크기 : %s\n", playerdatas[i][1]);
	}

	// 소켓 닫기
	closesocket(listen_sock);

	printf("\n[게임 서버를 닫았습니다]\n\n");

	// 윈속 종료
	WSACleanup();
	return 0;
}
