#pragma once

constexpr char SC_PLAYER_MOVE = 0;
constexpr char SC_KEY_INPUT = 1;
constexpr char SC_PLAYER_ROTATE = 2;
constexpr char SC_SEND_PLAYER = 3;
constexpr char  PACKET_GEMSTONE = 4;
constexpr char  SC_END = 5;


// 예시로 구조체 정의
// 예시로 구조체 정의



struct MOVE_PACKET {
	char type;
	float fx, fy, fz;
	float fro;
};

struct INPUT_PACKET {
	char type;
	bool bKeyDown;
	char input;
};

struct ROTATE_PACKET {
	char type;
	float fx, fy, fz;
};

struct SEND_PLAYER {
	char type;
	int id;
};

struct GemStonePacket {
	char cType;
	int num;
	float fX, fY, fZ;
};

struct EndPacket {
	char type;
	std::string id;
};