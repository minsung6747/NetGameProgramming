#pragma once

constexpr char SC_PLAYER_MOVE = 0;
constexpr char SC_KEY_INPUT = 1;
constexpr char SC_PLAYER_ROTATE = 2;
constexpr char SC_SEND_PLAYER = 3;

// ���÷� ����ü ����



struct MOVE_PACKET {
	char type;
	float fx, fy, fz;
};

struct INPUT_PACKET {
	char type;
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
