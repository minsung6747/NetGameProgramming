#pragma once

constexpr char SC_PLAYER_MOVE = 0;
constexpr char SC_KEY_INPUT = 1;

// 예시로 구조체 정의



struct MOVE_PACKET {
	char type;
	float fx, fy, fz;
};

struct INPUT_PACKET {
	char type;
	char input;
};
