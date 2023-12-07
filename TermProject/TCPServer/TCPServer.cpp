#include "..\Common.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include "..\Protocol.h"
#include "GameObject.h"
#include <random>
#include <chrono>

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
float fTransY[9];
float fTransZ[9];
void iRandomSetting()
{
	for (int i = 0; i < 9; i++)
	{
		bool bOverlap = false; // �ٸ� ������� ��ġ���� ����
		bool bCenterpos = false; // ����� �ִ��� ����
		bool bMapOutside = false; // �� �ۿ� �ִ��� ����

		const float OVERLAPSIZE = 2.f; // ��ħ ���� ũ�� (�ӽ�)
		const float CENTERSIZE = 2.f; // ��� ���� ũ�� (�ӽ�)

		float fxPosFromCenter = 0.f; // ������� ������ x �Ÿ�
		float fzPosFromCenter = 0.f; // ������� ������ z �Ÿ�

		// ��ǥ ����
		fTransX[i] = uid(gen) / 100;
		fTransZ[i] = uid(gen) / 100;

		while (1) {

			fxPosFromCenter = 10.f - fTransX[i]; // ������� ������ x �Ÿ�
			fzPosFromCenter = 10.f - fTransZ[i]; // ������� ������ z �Ÿ�

			bOverlap = false; // �ٸ� ������� ��ġ���� ����
			bCenterpos = false; // ����� �ִ��� ����
			bMapOutside = false; // �� �ۿ� �ִ��� ����

			// ����κ��� �Ÿ�
			float fLengthFromCenter = sqrt(fxPosFromCenter * fxPosFromCenter + fzPosFromCenter * fzPosFromCenter);

			// ����� ������ �Ѿ�� ���
			if (fLengthFromCenter >= 7.f) {
				// �� �ۿ� ����
				bMapOutside = true;
			}

			// ���� ������ ��ǥ��� ��ġ������ ����
			for (int j = 0; j < i; j++) {
				if ((fTransX[i] <= fTransX[j] + OVERLAPSIZE)
					&& (fTransX[i] >= fTransX[j] - OVERLAPSIZE)
					&& (fTransZ[i] <= fTransZ[j] + OVERLAPSIZE)
					&& (fTransZ[i] >= fTransZ[j] - OVERLAPSIZE))
				{
					bOverlap = true;
				}
			}

			// ����� �ִ��� ����
			if ((fTransX[i] <= 10.f + CENTERSIZE)
				&& (fTransX[i] >= 10.f - CENTERSIZE)
				&& (fTransZ[i] <= 10.f + CENTERSIZE)
				&& (fTransZ[i] >= 10.f - CENTERSIZE))
			{
				bCenterpos = true;
			}

			// ��ġ�� �ʰ� ����� ���� �ʰ� �� �ۿ� ���� �ʴ´ٸ� �������� ���´�
			if (!bOverlap && !bCenterpos && !bMapOutside) {

				printf("[%d]��° GemStone x, z ��ǥ : [%.2f, %.2f] ",
					i, fTransX[i], fTransZ[i]);

				fxPosFromCenter = 10.f - fTransX[i]; // ������� ������ x �Ÿ�
				fzPosFromCenter = 10.f - fTransZ[i]; // ������� ������ z �Ÿ�
				printf("(�������κ��� �Ÿ� : %.2f)\n", sqrt(fxPosFromCenter * fxPosFromCenter + fzPosFromCenter * fzPosFromCenter));

				break;
			}

			// ��ģ�ٸ� ��ǥ�� �������ϰ� ������ ������Ѵ�
			else {
				// ��ǥ ������
				fTransX[i] = uid(gen) / 100;
				fTransZ[i] = uid(gen) / 100;
			}
		}
	}
}
void SendGemStonePacket(SOCKET clientSocket) 
{
		
		GemStonePacket GemStonePacket[9];
	
			for (int i = 0; i < 9; ++i) {

				
				GemStonePacket[i].fX = fTransX[i];
				GemStonePacket[i].fY = fTransY[i];             //�׳� �ʱ�ȭ
				GemStonePacket[i].fZ = fTransZ[i];
				GemStonePacket[i].num = i;
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
	for (int i = 0;i < 3;++i) {
		g_Players[i]->transX = 9.f+i;
		g_Players[i]->transY = 1.f;
		g_Players[i]->transZ = 10.f;
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

void SendToMove(SOCK_INFO* sock_info, char input, bool KeyDown){

	switch (input) {
	case 'w':
	{
		g_Players[sock_info->id]->bFowardKeyDown = KeyDown;
		break;
	}
	case 'a':
	{
		g_Players[sock_info->id]->bLeftKeyDown = KeyDown;
		
		break;
	}
	case 's':
	{
		g_Players[sock_info->id]->bBackKeyDown = KeyDown;
		break;
	}
	case 'd':
	{
		g_Players[sock_info->id]->bRightKeyDown = KeyDown;
		break;
	}
	}
	//for (const auto& client : ClientList) {
	//SEND_PLAYER* packet_sendP = new SEND_PLAYER;
	//packet_sendP->type = SC_SEND_PLAYER;
	//packet_sendP->id = sock_info->id;
	//send(client.socket, reinterpret_cast<char*>(packet_sendP), sizeof(SEND_PLAYER), 0);
	//delete packet_sendP;
	//MOVE_PACKET* packet_tr = new MOVE_PACKET;
	////ROTATE_PACKET* packet_ro = new ROTATE_PACKET;

	//packet_tr->type = SC_PLAYER_MOVE;
	////packet_ro->type = SC_PLAYER_ROTATE;
	//packet_tr->fx = g_Players[sock_info->id]->transX;
	//packet_tr->fy = g_Players[sock_info->id]->transY;
	//packet_tr->fz = g_Players[sock_info->id]->transZ;
	//packet_tr->fro = g_Players[sock_info->id]->rotateY;
	////packet_ro->fy = g_Players[sock_info->id]->rotateY;
	//cout << sock_info->id << " " << packet_tr->fz << endl;
	//send(client.socket, reinterpret_cast<char*>(packet_tr), sizeof(MOVE_PACKET), 0);
	////send(client.socket, reinterpret_cast<char*>(packet_ro), sizeof(ROTATE_PACKET), 0);
	//delete packet_tr;
	////delete packet_ro;
	//}


}

bool Collision(float firstX, float firstY, float firstZ,
				float lastX, float lastY, float lastZ, float fDistance) 
{
	//bool flag = true;
	if (firstX + fDistance < lastX - fDistance || lastX + fDistance < firstX - fDistance)
		return false;
	if (firstY + fDistance < lastY - fDistance || lastY + fDistance < firstY - fDistance)
		return false;
	if (firstZ + fDistance < lastZ - fDistance || lastZ + fDistance < firstZ - fDistance)
		return false;

	/*if (firstX - fDistance > lastX + fDistance && firstX + fDistance > lastX + fDistance)
		return false;
	if (firstY - fDistance > lastY + fDistance && firstY + fDistance > lastY + fDistance)
		return false;
	if (firstZ - fDistance > lastZ + fDistance && firstZ + fDistance > lastZ + fDistance)
		return false;*/
	return true;
}


void UpdatePlayer() {
	bool flag = false;
	bool pFlag = false;
	for (int id = 0;id < 3;++id) {
		if (g_Players[id]->bCatched == -1) {
			for (int i = 0;i < 9;++i) {
				flag = Collision(g_Players[id]->transX, g_Players[id]->transY, g_Players[id]->transZ,
					fTransX[i], 0, fTransZ[i],0.5f);
				if (flag) {
					g_Players[id]->bCatched = i;
				}
			}
		}
		else {
			if (Collision(g_Players[id]->transX, g_Players[id]->transY, g_Players[id]->transZ,
				10.f, 1.f, 10.f, 1.f)) {
				switch (g_Players[id]->bCatched) {
				case 0:
				case 1:
				case 2: 
				{
					fTransX[g_Players[id]->bCatched] = 9.8f + 0.2* g_Players[id]->bCatched;
					fTransY[g_Players[id]->bCatched] = 0.f;
					fTransZ[g_Players[id]->bCatched] = 9.8f;
					break;
				}
				case 3:
				case 4:
				case 5:
				{
					fTransX[g_Players[id]->bCatched] = 9.8f + 0.2 * (g_Players[id]->bCatched - 3);
					fTransY[g_Players[id]->bCatched] = 0.f;
					fTransZ[g_Players[id]->bCatched] = 10.f;
					break;
				}
				case 6:
				case 7:
				case 8:
				{
					fTransX[g_Players[id]->bCatched] = 9.8f + 0.2 * (g_Players[id]->bCatched - 6);
					fTransY[g_Players[id]->bCatched] = 0.f;
					fTransZ[g_Players[id]->bCatched] = 10.2f;
					break;
				}
				}
				g_Players[id]->bCatched = -1;
				++g_Players[id]->iPoint;
				cout << ClientList[id].sName << " - " << g_Players[id]->iPoint << endl;
			}
			fTransX[g_Players[id]->bCatched] = g_Players[id]->transX;
			fTransY[g_Players[id]->bCatched] = g_Players[id]->transY + 0.2f;
			fTransZ[g_Players[id]->bCatched] = g_Players[id]->transZ;
		}

		if (g_Players[id]->bFowardKeyDown) {
			g_Players[id]->transX -= 0.03f * sin(g_Players[id]->rotateY * atan(1) * 4 / 180);
			g_Players[id]->transZ -= 0.03f * cos(g_Players[id]->rotateY * atan(1) * 4 / 180);
		}
		if (g_Players[id]->bBackKeyDown) {
			g_Players[id]->transX += 0.03f * sin(g_Players[id]->rotateY * atan(1) * 4 / 180);
			g_Players[id]->transZ += 0.03f * cos(g_Players[id]->rotateY * atan(1) * 4 / 180);
		}
		if (g_Players[id]->bLeftKeyDown) {
			g_Players[id]->rotateY += 1.f;
		}
		if (g_Players[id]->bRightKeyDown) {
			g_Players[id]->rotateY -= 1.f;
		}
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

		//std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
		// ������ ������
		if (ClientList.size() == 3) {
			UpdatePlayer();
			for (const auto& client : ClientList) {

				SEND_PLAYER* packet_sendP = new SEND_PLAYER;
				packet_sendP->type = SC_SEND_PLAYER;
				packet_sendP->id = sock_info->id;

				MOVE_PACKET* packet_tr = new MOVE_PACKET;
				packet_tr->type = SC_PLAYER_MOVE;
				packet_tr->fx = g_Players[sock_info->id]->transX;
				packet_tr->fy = g_Players[sock_info->id]->transY;
				packet_tr->fz = g_Players[sock_info->id]->transZ;
				packet_tr->fro = g_Players[sock_info->id]->rotateY;


				//lock_guard<mutex> lock(clientListMutex);
				send(client.socket, reinterpret_cast<char*>(packet_sendP), sizeof(SEND_PLAYER), 0);
				send(client.socket, reinterpret_cast<char*>(packet_tr), sizeof(MOVE_PACKET), 0);

				SendGemStonePacket(client.socket);
				//cout << sock_info->id << " " << packet_tr->fz << endl;
				delete packet_sendP;
				delete packet_tr;

			}
		}

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
			//cout << "[" << sock_info->id << "]" << packet->input << endl;
			SendToMove(sock_info, packet->input, packet->bKeyDown);
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



DWORD WINAPI SendClient(LPVOID arg)
{

	int retval;
	SOCK_INFO* sock_info = reinterpret_cast<SOCK_INFO*> (arg);

	SOCKET client_sock = sock_info->client_sock;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

	while (1) {
		//auto start_time = chrono::steady_clock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
		if (ClientList.size() == 3) {
		UpdatePlayer();
		for (int id = 0;id < 3;++id) {
			for (const auto& client : ClientList) {

					SEND_PLAYER* packet_sendP = new SEND_PLAYER;
					packet_sendP->type = SC_SEND_PLAYER;
					packet_sendP->id = id;

					MOVE_PACKET* packet_tr = new MOVE_PACKET;
					packet_tr->type = SC_PLAYER_MOVE;
					packet_tr->fx = g_Players[id]->transX;
					packet_tr->fy = g_Players[id]->transY;
					packet_tr->fz = g_Players[id]->transZ;
					packet_tr->fro = g_Players[id]->rotateY;


					//lock_guard<mutex> lock(clientListMutex);
					send(sock_info->client_sock, reinterpret_cast<char*>(packet_sendP), sizeof(SEND_PLAYER), 0);
					send(sock_info->client_sock, reinterpret_cast<char*>(packet_tr), sizeof(MOVE_PACKET), 0);

					//cout << sock_info->id << " " << packet_tr->fz << endl;
					delete packet_sendP;
					delete packet_tr;

			}
		}
		}
		//auto end_time = chrono::steady_clock::now();
			//auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

			//// 1�ʿ� 30�� ������ ���� ��� �ð� ���
			//int sleep_time = max(0, static_cast<int>(1000 / 30) - static_cast<int>(elapsed_time));
			//this_thread::sleep_for(chrono::milliseconds(sleep_time));
	}
	// ���� �ݱ�
	/*closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		addr, ntohs(clientaddr.sin_port));*/
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
	HANDLE hThread, hSendThread;
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
		lock_guard<mutex> lock(clientListMutex);
		Clients[++i].client_sock = client_sock;
		Clients[i].id = i;
		Player* newPlayer = new Player;
		g_Players.push_back(newPlayer);

		// ������ Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			addr, ntohs(clientaddr.sin_port));

		/*hSendThread = CreateThread(NULL, 0, SendClient,
			reinterpret_cast<LPVOID*>(Clients[i].GetSockInfo()), 0, NULL);
		if (hSendThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hSendThread); }*/

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient,
			reinterpret_cast<LPVOID*>(Clients[i].GetSockInfo()), 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
		/*hSendThread = CreateThread(NULL, 0, SendClient,
			reinterpret_cast<LPVOID*>(Clients[i].GetSockInfo()), 0, NULL);*/
		/*hSendThread = CreateThread(NULL, 0, SendClient,
			reinterpret_cast<LPVOID*>(Clients[i].GetSockInfo()), 0, NULL);
		if (hSendThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hSendThread); }*/
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}


