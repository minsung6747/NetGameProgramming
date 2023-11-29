#pragma once

struct SOCK_INFO {
	SOCKET client_sock = NULL;
	int id = 0;

	SOCK_INFO* GetSockInfo() { return this; }
};

struct MOVE_PACKET{
	char type;
	float fx, fy, fz;
};
