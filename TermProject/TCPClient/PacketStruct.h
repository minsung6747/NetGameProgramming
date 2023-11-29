#pragma once
// 예시로 구조체 정의

constexpr char CS_MOVE = 0;

struct MOVE_PACKET {
	char type;
	float fx, fy, fz;
};

struct INPUT_PACKET {
	char type;
	char input;
};
