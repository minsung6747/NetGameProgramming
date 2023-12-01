#include "..\Common.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include "..\Protocol.h"
#include "GameObject.h"
#include <random>
//#include "GameObject.h"
//#include "PacketStruct.h"
//#include "PacketNumber.h"
#define SERVERPORT 9000
#define BUFSIZE    512



struct ClientInfo {
	SOCKET socket;
	bool BIsLoggedIn;
	std::string sName;
	int id;
};

struct SOCK_INFO {
	SOCKET client_sock{};
	int id{};

	SOCK_INFO* GetSockInfo() { return this; }
};

using namespace std;

mutex clientListMutex;
vector<ClientInfo> ClientList;
vector<Player*> g_Players;

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
		send(client.socket, (char*)&client.id, sizeof(int), 0);
		SendGemStonePacket(client.socket);
	}
	

}

void HandleLogin(SOCK_INFO* clientSocket, const string& clientName) {
	//Ŭ���̾�Ʈ �Ѹ��� �α��� �ϰԵǸ�,�α��λ��·θ����.�׸��� Vector Push 


	lock_guard<mutex> lock(clientListMutex); // clientList �����嵿������ ������ 
	ClientInfo newClient;
	newClient.socket = clientSocket->client_sock;
	newClient.BIsLoggedIn = true;
	newClient.sName = clientName;
	newClient.id = clientSocket->id;

	ClientList.push_back(newClient);

	cout << "Ŭ���̾�Ʈ : " << "[" << newClient.id << "]" << clientName << " " << endl;

	if (ClientList.size() == 3) {


		StartGame();
		//lock_guard<mutex> unlock(&clientListMutex);
	}
}

void SendToMove(SOCK_INFO* sock_info, char input){

	switch (input) {
	case 'w':
	{
		g_Players[sock_info->id]->transX -=  0.03f * sin(g_Players[sock_info->id]->rotateY * atan(1) * 4 / 180);
		g_Players[sock_info->id]->transZ -= 0.03f * cos(g_Players[sock_info->id]->rotateY * atan(1) * 4 / 180);
		break;
	}
	case 'a':
	{
		g_Players[sock_info->id]->rotateY += 1.f;
		break;
	}
	case 's':
	{
		g_Players[sock_info->id]->transX += 0.03f * sin(g_Players[sock_info->id]->rotateY * atan(1) * 4 / 180);
		g_Players[sock_info->id]->transZ += 0.03f * cos(g_Players[sock_info->id]->rotateY * atan(1) * 4 / 180);
		break;
	}
	case 'd':
	{
		g_Players[sock_info->id]->rotateY -= 1.f;
		break;
	}
	}
	for (const auto& client : ClientList) {
	SEND_PLAYER* packet_sendP = new SEND_PLAYER;
	packet_sendP->type = SC_SEND_PLAYER;
	packet_sendP->id = sock_info->id;
	send(client.socket, reinterpret_cast<char*>(packet_sendP), sizeof(SEND_PLAYER), 0);
	delete packet_sendP;
		MOVE_PACKET* packet_tr = new MOVE_PACKET;
		ROTATE_PACKET* packet_ro = new ROTATE_PACKET;

		packet_tr->type = SC_PLAYER_MOVE;
		packet_ro->type = SC_PLAYER_ROTATE;
		packet_tr->fx = g_Players[sock_info->id]->transX;
		packet_tr->fy = g_Players[sock_info->id]->transY;
		packet_tr->fz = g_Players[sock_info->id]->transZ;
		packet_ro->fy = g_Players[sock_info->id]->rotateY;
		cout << sock_info->id << " " << packet_tr->fz << endl;
			send(client.socket, reinterpret_cast<char*>(packet_tr), sizeof(MOVE_PACKET), 0);
			send(client.socket, reinterpret_cast<char*>(packet_ro), sizeof(ROTATE_PACKET), 0);
		delete packet_tr;
		delete packet_ro;
		}


}




// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{

	int retval;
	SOCK_INFO* sock_info = reinterpret_cast<SOCK_INFO*> (arg);

	SOCKET client_sock = sock_info->client_sock;
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
		HandleLogin(sock_info, clientName);
	}


	while (1) {
		// ������ �ޱ�
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		switch (buf[0]) {
		case SC_KEY_INPUT:
		{

			INPUT_PACKET* packet = reinterpret_cast<INPUT_PACKET*>(buf);
			cout << "[" << sock_info->id << "]" << packet->input << endl;
			SendToMove(sock_info, packet->input);
			break;
		}
		}

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
	SOCK_INFO* Clients = new SOCK_INFO[3];
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	HANDLE hThread;
	int addrlen;
	int i{ -1 };

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		Clients[++i].client_sock = client_sock;
		Clients[i].id = i;
		Player* newPlayer = new Player;
		g_Players.push_back(newPlayer);

		// ������ Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			addr, ntohs(clientaddr.sin_port));

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient,
			reinterpret_cast<LPVOID*>(Clients[i].GetSockInfo()), 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}


