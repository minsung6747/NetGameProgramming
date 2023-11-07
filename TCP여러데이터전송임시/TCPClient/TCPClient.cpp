#include "..\Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512

int main(int argc, char *argv[])
{
	int retval;

	// 명령행 인수가 있으면 IP 주소로 사용
	if (argc > 1) SERVERIP = argv[1];

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET sock1 = socket(AF_INET, SOCK_STREAM, 0);
	SOCKET sock2 = socket(AF_INET, SOCK_STREAM, 0);
	if (sock1 == INVALID_SOCKET) err_quit("socket()");
	if (sock2 == INVALID_SOCKET) err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = connect(sock1, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
	retval = connect(sock2, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// 데이터 통신에 사용할 변수
	int len;

	char playerlength[BUFSIZE + 1] = "1000";

	// playerlength 보내기
	retval = send(sock1, playerlength, (int)strlen(playerlength), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 1;
	}
	printf("playerlength 전송 완료\n");

	char playername[BUFSIZE + 1] = "testplayer";

	// playername 보내기
	retval = send(sock2, playername, (int)strlen(playername), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 1;
	}
	printf("playername 전송 완료\n");
	
	// printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

	// 소켓 닫기
	closesocket(sock1);
	closesocket(sock2);

	// 윈속 종료
	WSACleanup();
	return 0;
}
