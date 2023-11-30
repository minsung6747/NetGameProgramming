#include "..\Common.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <random>
//#include "GameObject.h"
#include "PacketStruct.h"
#include "PacketNumber.h"
#define SERVERPORT 9000
#define BUFSIZE    512



struct ClientInfo {
	SOCKET socket;
	bool BIsLoggedIn;
	std::string sName;

};


using namespace std;

mutex clientListMutex;
vector<ClientInfo> ClientList;

random_device rd;
mt19937 gen{ rd() };
uniform_int_distribution<int> uid{ 0,2000 };

float fTransX[9];
float fTransZ[9];
void iRandomSetting()
{
	for (int i = 0; i < 9; ++i)
	{
		fTransX[i] = uid(gen) / 100;
		fTransZ[i] = uid(gen) / 100;
	}
}
void SendGemStonePacket(SOCKET clientSocket) 
{
		
		GemStonePacket GemStonePacket[9];
	
			for (int i = 0; i < 9; ++i) {

				
				GemStonePacket[i].fX = fTransX[i];
				GemStonePacket[i].fY = 0;             //�׳� �ʱ�ȭ
				GemStonePacket[i].fZ = fTransZ[i];
				GemStonePacket->cType = PACKET_GEMSTONE;

				// �����͸� Ŭ���̾�Ʈ���� ����
				send(clientSocket, reinterpret_cast<char*>(&GemStonePacket[i]), sizeof(GemStonePacket[0]), 0);
			}
}

void StartGame() {
	cout << "���ӽ���Test��" << endl;

	//������ ���۵Ǿ����� Ŭ���̾�Ʈ���� �˸���. 
	//lock_guard<mutex> lock(clientListMutex);
	iRandomSetting();
	for (const auto& client : ClientList) {
		send(client.socket, (char*)"START", 5, 0);
		SendGemStonePacket(client.socket);
	}
	

}

void HandleLogin(SOCKET clientSocket, const string& clientName) {
	//Ŭ���̾�Ʈ �Ѹ��� �α��� �ϰԵǸ�,�α��λ��·θ����.�׸��� Vector Push 

	lock_guard<mutex> lock(clientListMutex); // clientList �����嵿������ ������ 

	ClientInfo newClient;
	newClient.socket = clientSocket;
	newClient.BIsLoggedIn = true;
	newClient.sName = clientName;

	ClientList.push_back(newClient);

	cout << "Ŭ���̾�Ʈ : " << clientName << " " << endl;
	
	if (ClientList.size() == 3) {
	
		for (const auto& client : ClientList) {
			


			StartGame();
			//lock_guard<mutex> unlock(clientListMutex);
		}
	}
}




// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));


	char cNameBuffer[256]; // �̸� �����ž�
	retval = recv(client_sock, cNameBuffer, sizeof(cNameBuffer), 0);
	if (retval > 0) {  //���������� �޾Ҵٸ�
		cNameBuffer[retval] = '\0';
		string clientName(cNameBuffer);
		HandleLogin(client_sock, clientName);
	}


	while (1) {
		// ������ �ޱ�
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// ���� ������ ���
		buf[retval] = '\0';
		std::printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

		// ������ ������
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		MOVE_PACKET movePacket;
		retval = recv(client_sock, reinterpret_cast<char*>(&movePacket), sizeof(MOVE_PACKET), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// MOVE_PACKET ó��
		printf("[TCP/%s:%d] Received MOVE_PACKET: Type=%d, X=%f, Y=%f, Z=%f\n",
			addr, ntohs(clientaddr.sin_port), movePacket.cType, movePacket.fX, movePacket.fY, movePacket.fZ);

	}

	// ���� �ݱ�
	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		addr, ntohs(clientaddr.sin_port));
	return 0;
}



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

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			addr, ntohs(clientaddr.sin_port));

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}


