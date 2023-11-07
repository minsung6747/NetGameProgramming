#include "..\Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512

int main(int argc, char *argv[])
{
	int retval;

	// ����� �μ��� ������ IP �ּҷ� ���
	if (argc > 1) SERVERIP = argv[1];

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
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

	// ������ ��ſ� ����� ����
	int len;

	char playerlength[BUFSIZE + 1] = "1000";

	// playerlength ������
	retval = send(sock1, playerlength, (int)strlen(playerlength), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 1;
	}
	printf("playerlength ���� �Ϸ�\n");

	char playername[BUFSIZE + 1] = "testplayer";

	// playername ������
	retval = send(sock2, playername, (int)strlen(playername), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 1;
	}
	printf("playername ���� �Ϸ�\n");
	
	// printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

	// ���� �ݱ�
	closesocket(sock1);
	closesocket(sock2);

	// ���� ����
	WSACleanup();
	return 0;
}
