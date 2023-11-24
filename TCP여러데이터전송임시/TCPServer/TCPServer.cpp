#include "..\Common.h"

#define SERVERPORT 9000
#define BUFSIZE    512

#define MAXPLAYERDATANUM 10 // �ִ� �÷��̾� ������ ���� (�������� �� �ƴ�)
#define NEEDPLAYERNUM    3  // �ʿ��� �÷��̾� ��


char playerdatas[MAXPLAYERDATANUM][10][BUFSIZE + 1] = {}; // �÷��̾� ������ ����

// -------------- ������ ��ſ� ����� ���� --------------

SOCKET client_sock;
struct sockaddr_in clientaddr;
int addrlen;

char received_data[BUFSIZE + 1];

char ready_data[MAXPLAYERDATANUM][2]; // ���� ���� ����
char saved_data[MAXPLAYERDATANUM][BUFSIZE + 1];

int now_received_data_num = 0;

int iCountReady = 0;

// -------------- addClient() �Լ� --------------

int addClient(char* name, char* playersize)
{
	printf("addClient() �Լ��� ����Ǿ����ϴ�\n");
	for (int i = 0; i < MAXPLAYERDATANUM; i++)
	{
		// ���� ������ �ִٸ�
		if (strcmp(playerdatas[i][0], "") == 0)
		{
			strcpy(playerdatas[i][0], name);
			strcpy(playerdatas[i][1], playersize);
			return i;
		}

		// ��� ������ á�ٸ�
		else if (i == MAXPLAYERDATANUM - 1)
		{
			printf("�����͸� �߰��� ������ �����ϴ�\n");
			return -1;
		}
	}
	return 0;
}

// -------------- checkIfDataExist() �Լ� --------------

bool checkIfDataExist()
{
	printf("checkIfDataExist() �Լ��� ����Ǿ����ϴ�\n");

	// Ŭ���̾�Ʈ�κ��� ���� �����Ͱ� ������ �����ϴ��� Ȯ���ϰ�
	// �������� �ʴ´ٸ� ���� �����͸� ������ �߰��ϴ� �ڵ��̴�

	bool bIfDataExist = false; // �����Ͱ� ������ �̹� �����ϴ��� Ȯ��

	for (int i = 0; i < MAXPLAYERDATANUM; i++)
	{
		// �����Ͱ� ���ٸ�
		if (strcmp(playerdatas[i][0], "") == 0)
		{
			// �������� ������
			break;
		}
		// ���� �̸��� �����ϰ� �����Ѵٸ�
		else if (strcmp(playerdatas[i][0], saved_data[0]) == 0)
		{
			// �����Ͱ� ������ �̹� ������
			printf("����� �̸��� �ش��ϴ� �����Ͱ� ������ �ֽ��ϴ�\n");
			bIfDataExist = true;
		}
	}

	// ������ ����� �����Ͱ� �ƴ϶��
	if (!bIfDataExist)
	{
		// ������ �߰��Ѵ�
		int num = addClient(saved_data[0], saved_data[1]);
		printf("%d��° �÷��̾� ���� �Ϸ�\n", num + 1);

		return false; // �����Ͱ� �������� ����
	}
	else
	{
		return true; // �����Ͱ� ������
	}
}


// Thread ProcessClient
DWORD WINAPI ProcessClient(LPVOID arg)
{
	printf("�ӽ� ������ ����\n");

	return 0;
}

void RecvReadyFromClient(SOCKET listen_sock, int retval, int playernum)
{
	printf("RecvReadyFromClient() �Լ��� ����Ǿ����ϴ�.\n");
	printf("Ŭ���̾�Ʈ ReadyMessage�� �޽��ϴ�.\n");
	
	// accept()
	addrlen = sizeof(clientaddr);
	client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		err_display("accept()");
		return;
	}

	// �ӽ� Handle Create
	HANDLE hThread;

	// ������ ����
	hThread = CreateThread(NULL, 0, ProcessClient,
		(LPVOID)client_sock, 0, NULL);

	// ������ Ŭ���̾�Ʈ ���� ���
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		addr, ntohs(clientaddr.sin_port));

	retval = recv(client_sock, received_data, BUFSIZE, 0);

	printf("Ready ���� recv �Ϸ�\n");

	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return;
	}
	else if (retval == 0)
		return;

	// ���� �����͸� ready_data�� �ش��ϴ� ��ġ�� �ִ´�
	received_data[retval] = '\0';
	strcpy(ready_data[playernum], received_data);

	// ���� �ݱ�
	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		addr, ntohs(clientaddr.sin_port));

	printf("RecvReadyFromClient() �Լ��� ����Ǿ����ϴ�\n\n");
}

// recvDataFromSocket() �Լ�

void RecvDataFromClient(SOCKET listen_sock, int retval, int playernum)
{
	printf("RecvDataFromClient() �Լ��� ����Ǿ����ϴ�\n");
	printf("Ŭ���̾�Ʈ�� Send()�� ��ٸ��� �ֽ��ϴ�\n");


	// accept()
	addrlen = sizeof(clientaddr);
	client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		err_display("accept()");
		return;
	}

	// �ӽ� Handle Create
	HANDLE hThread;

	// ������ ����
	hThread = CreateThread(NULL, 0, ProcessClient,
		(LPVOID)client_sock, 0, NULL);
	
	// ������ Ŭ���̾�Ʈ ���� ���
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		addr, ntohs(clientaddr.sin_port));

	retval = recv(client_sock, received_data, BUFSIZE, 0);

	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return;
	}
	else if (retval == 0)
		return;

	// ���� �����͸� saved_data�� �ش��ϴ� ��ġ�� �ִ´�
	received_data[retval] = '\0';
	strcpy(saved_data[playernum], received_data);

	// ���� �ݱ�
	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		addr, ntohs(clientaddr.sin_port));

	printf("RecvDataFromClient() �Լ��� ����Ǿ����ϴ�\n\n");
}


// -------------- main �Լ� --------------

int main(int argc, char* argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
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

	printf("[���� ������ �������ϴ�]\n\n");

	// ���� �÷��̾� ��ȣ
	int iplayernum = 1;
	
	// Ŭ���̾�Ʈ�κ��� �����͸� ������ 0�� ��ġ�� �����Ѵ�
	RecvDataFromClient(listen_sock, retval, 0);
	printf("�÷��̾� �̸� ������ �������� �Ϸ�\n");

	// �̸��� ����Ǿ� �ִ��� Ȯ��
	bool bDataExist = checkIfDataExist();
	if (now_received_data_num == 0)
		if (bDataExist)
		{
			// �̹� ����� �̸��� ��� �Ѿ��
			printf("�̹� �����ϴ� �̸��Դϴ�\n");
		}
		else
		{
			printf("������ %d��° �÷��̾� �̸��� �߰��Ͽ����ϴ�\n\n", iplayernum);

			// Ŭ���̾�Ʈ�κ��� ũ�� �����͸� ������ 1�� ��ġ�� �����Ѵ�
			// RecvDataFromClient(listen_sock, retval, 1);
			// printf("%d��° �÷��̾� ũ�� ������ �������� �Ϸ�\n", iplayernum);

			// �����͸� playerdatas[]�� ���� �÷��̾� ��ġ�� �����Ѵ�
			for (int i = 0; i < 1; i++)
			{
				strcpy(playerdatas[iplayernum - 1][i], saved_data[i]);
			}

			// �÷��̾� ������ �޾ƿ� ��ġ�� �������� �̵�
			iplayernum++;
		}

	printf("�÷��̾� ������ �������� �Ϸ�\n\n");
	
	// �÷��̾��� ���� �����͸� �޴´�
	RecvReadyFromClient(listen_sock, retval, 0);
	
	printf("Ready ������ ���� �Ϸ�\n");

	for (int i = 0; i < NEEDPLAYERNUM; i++)
	{
		if (strcmp(ready_data[i], "Ready") == 0) printf("%d��° �÷��̾� : �غ��\n", i + 1);
		else printf("%d��° �÷��̾� : �غ� �� ��\n", i + 1);
	}

	// ### �׽�Ʈ�� : �÷��̾� ������ ���
	for (int i = 0; i < NEEDPLAYERNUM; i++)
	{
		printf("\n[%d��° �÷��̾� ������]\n", i + 1);
		printf("�÷��̾� �̸� : %s\n", playerdatas[i][0]);
		// printf("�÷��̾� ũ�� : %s\n", playerdatas[i][1]);
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	printf("\n[���� ������ �ݾҽ��ϴ�]\n\n");

	// ���� ����
	WSACleanup();
	return 0;
}
