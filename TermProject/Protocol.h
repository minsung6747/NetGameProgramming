#pragma once

constexpr char SC_PLAYER_MOVE = 0;
constexpr char SC_KEY_INPUT = 1;

// ���÷� ����ü ����



struct MOVE_PACKET {
	char type;
	float fx, fy, fz;
};

struct INPUT_PACKET {
	char type;
	char input;
};
