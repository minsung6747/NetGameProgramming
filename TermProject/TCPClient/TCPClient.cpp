#include "..\Common.h"
#include "stdafx.h"
#include "Camera.h"
#include "GameShader.h"
#include "GameObject.h"
#include <GL/glew.h>
#include <iostream>
#include "..\Protocol.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512

using namespace std;
struct ClientInfo {
	SOCKET socket;
	bool bisLoggedIn;

};
struct SOCK_INFO {
	SOCKET server_sock{};
	int id{};
	SOCK_INFO* GetSockInfo() { return this; }
};

GemStonePacket receivedGemStonePacket[9];
float fGemstoneX[9], fGemstoneY[9], fGemstoneZ[9];

random_device rd;
mt19937 gen{ rd() };

int g_id;
SOCK_INFO* sock_info;
SOCKET sock;

Camera CO;
GameShader* gs = new GameShader;
Background* bg = new Background();
Gemstone* wp_Gemstone = new Gemstone();
Bomb* wp_Bomb = new Bomb();
Bubble* wp_Bubble = new Bubble();
//alstkdalsdl0
GLvoid drawScene();
GLvoid KeyBoard(unsigned char, int, int);
GLvoid KeyBoardUp(unsigned char, int, int);
GLvoid SpecialKeyBoard(int, int, int);
GLvoid SpecialKeyBoardUp(int, int, int);
GLvoid Timer(int);
GLvoid MouseChange(int, int);
GLvoid Mouse(int, int);
void SendPlayerPosition();
typedef struct object {
	float trans_x{}, trans_y{}, trans_z{};               // 기본 좌표값
	float trans_x_aoc{}, trans_y_aoc{}, trans_z_aoc{};      // 좌표 변화량 aoc - amount of change

	float rotate_x{}, rotate_y{}, rotate_z{};            // trans(x_trans + x_trans_aoc, 0.0f, 0.0f)
	float rotate_x_aoc{}, rotate_y_aoc{}, rotate_z_aoc{};   // You know what im say??

	float scale_x{ 1.0f }, scale_y{ 1.0f }, scale_z{ 1.0f };
	float scale_x_aoc{}, scale_y_aoc{}, scale_z_aoc{};

	float color_r{}, color_g{}, color_b{};
	float color_r_aoc{ 0.001 }, color_g_aoc{ 0.001 }, color_b_aoc{ 0.001 };

	bool get{ false }, put{ false };

	float r{};
}O;
typedef struct function {
	bool is_x_plus_move{ false };
}F;

O subMarine[3];

struct Mouse_Handling {
	bool left_button;
	bool right_button;
	bool first_mouse_pos_flag = true;

	float first_mouse_pos_x;
	float first_mouse_pos_y;

	float delta_mouse_x;
	float delta_mouse_y;

	float mouse_x;
	float mouse_y;
};
Mouse_Handling MH;
//상민
GLint Collision(float first_x1, float first_x2, float last_x1, float last_x2)
{
	if (first_x1 <= last_x1 && last_x1 <= first_x2)
		return 1;
	if (first_x1 <= last_x2 && last_x2 <= first_x2)
		return 1;
	if (last_x1 <= first_x1 && first_x1 <= last_x2)
		return 1;
	if (last_x1 <= first_x2 && first_x2 <= last_x2)
		return 1;
	return 0;
}

//GLfloat rot;
GLfloat rot_t;
GLfloat camera_rot;
bool Grab_Arm_Long = false;
bool Limit_Long = false;
bool jump_key = false;
bool jump_down_key = false;
bool left_key = false;
bool right_key = false;
bool up_key = false;
bool down_key = false;
bool L_key = false;
bool R_key = false;
bool C_R_U = false;
bool C_R_D = false;
bool C_R_R = false;
bool C_R_L = false;
GLfloat C_R;
GLfloat arm_rot;
GLfloat limit;
GLvoid Draw_SubMarine(int id) {

	glm::mat4 Body = glm::mat4(1.0f);
	Body = glm::translate(Body, glm::vec3(subMarine[id].trans_x, subMarine[id].trans_y, subMarine[id].trans_z));
	Body = glm::translate(Body, glm::vec3(0.f, 0.f, 0.0f));
	Body = glm::rotate(Body, glm::radians(subMarine[id].rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	//Body = glm::rotate(Body, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
	Body = glm::scale(Body, glm::vec3(0.2f, 0.2f, 0.3f));
	unsigned int HtransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(HtransformLocation, 1, GL_FALSE, glm::value_ptr(Body));
	int objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	unsigned isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, GLU_FILL);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 0.8f, 0.8f, 0.8f, 1.0);
	gluCylinder(gs->qobj, 1.0, 1.0, 2.0, 20, 8);

	glm::mat4 Head = glm::mat4(1.0f);
	Head = glm::translate(Head, glm::vec3(subMarine[id].trans_x, subMarine[id].trans_y, subMarine[id].trans_z));
	Head = glm::translate(Head, glm::vec3(0.f, 0.f, 0.0f));
	Head = glm::rotate(Head, glm::radians(subMarine[id].rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	Head = glm::scale(Head, glm::vec3(0.2f, 0.2f, 0.2f));
	HtransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(HtransformLocation, 1, GL_FALSE, glm::value_ptr(Head));
	objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, GLU_FILL);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 0.8f, 0.8f, 0.8f, 1.0);
	gluSphere(gs->qobj, 1.0, 20, 20);

	glm::mat4 Tail = glm::mat4(1.0f);
	Tail = glm::translate(Tail, glm::vec3(subMarine[id].trans_x, subMarine[id].trans_y, subMarine[id].trans_z));
	Tail = glm::rotate(Tail, glm::radians(subMarine[id].rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	//Tail = glm::rotate(Tail, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
	Tail = glm::translate(Tail, glm::vec3(0.f, 0.f, 0.6f));
	Tail = glm::scale(Tail, glm::vec3(0.2f, 0.2f, 0.2f));
	HtransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(HtransformLocation, 1, GL_FALSE, glm::value_ptr(Tail));
	objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, GLU_FILL);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 0.8f, 0.8f, 0.8f, 1.0);
	gluCylinder(gs->qobj, 1.0, 0.0, 2.0, 20, 8);

	glm::mat4 Wing = glm::mat4(1.0f);
	Wing = glm::translate(Wing, glm::vec3(subMarine[id].trans_x, subMarine[id].trans_y, subMarine[id].trans_z));
	Wing = glm::rotate(Wing, glm::radians(subMarine[id].rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	Wing = glm::rotate(Wing, glm::radians(rot_t), glm::vec3(0.0f, 0.0f, 1.0f));
	Wing = glm::translate(Wing, glm::vec3(0.f, 0.f, 0.9f));
	Wing = glm::scale(Wing, glm::vec3(0.3f, 0.005f, 0.05f));
	HtransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(HtransformLocation, 1, GL_FALSE, glm::value_ptr(Wing));
	objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, GLU_FILL);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 1.0f, 1.0f, 0.0f, 1.0);
	gluCylinder(gs->qobj, 1.0, 1.0, 2.0, 20, 8);

	Wing = glm::mat4(1.0f);
	Wing = glm::translate(Wing, glm::vec3(subMarine[id].trans_x, subMarine[id].trans_y, subMarine[id].trans_z));
	Wing = glm::rotate(Wing, glm::radians(subMarine[id].rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	Wing = glm::rotate(Wing, glm::radians(rot_t), glm::vec3(0.0f, 0.0f, 1.0f));
	Wing = glm::rotate(Wing, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	Wing = glm::translate(Wing, glm::vec3(0.f, 0.f, 0.9f));
	Wing = glm::scale(Wing, glm::vec3(0.3f, 0.005f, 0.05f));
	HtransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(HtransformLocation, 1, GL_FALSE, glm::value_ptr(Wing));
	objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, GLU_FILL);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 1.0f, 1.0f, 0.0f, 1.0);
	gluCylinder(gs->qobj, 1.0, 1.0, 2.0, 20, 8);

	glm::mat4 Grab_Arm_Body = glm::mat4(1.0f);
	Grab_Arm_Body = glm::translate(Grab_Arm_Body, glm::vec3(subMarine[id].trans_x, subMarine[id].trans_y, subMarine[id].trans_z));
	Grab_Arm_Body = glm::rotate(Grab_Arm_Body, glm::radians(subMarine[id].rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	Grab_Arm_Body = glm::translate(Grab_Arm_Body, glm::vec3(0.f, -0.2f, 0.f));
	Grab_Arm_Body = glm::scale(Grab_Arm_Body, glm::vec3(0.05f, 0.05f, 0.05f));
	HtransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(HtransformLocation, 1, GL_FALSE, glm::value_ptr(Grab_Arm_Body));
	objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, GLU_FILL);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 1.0f, 1.0f, 1.0f, 1.0);
	gluSphere(gs->qobj, 1.0, 20, 20);

	glm::mat4 Grab_Arm = glm::mat4(1.0f);
	Grab_Arm = glm::translate(Grab_Arm, glm::vec3(subMarine[id].trans_x, subMarine[id].trans_y, subMarine[id].trans_z));
	Grab_Arm = glm::translate(Grab_Arm, glm::vec3(0.f, -0.2f, 0.05f));
	Grab_Arm = glm::rotate(Grab_Arm, glm::radians(arm_rot), glm::vec3(1.0f, 0.0f, 0.0f));
	Grab_Arm = glm::translate(Grab_Arm, glm::vec3(0.f, 0.2f, -0.05f));
	Grab_Arm = glm::rotate(Grab_Arm, glm::radians(subMarine[id].rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	Grab_Arm = glm::rotate(Grab_Arm, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
	Grab_Arm = glm::translate(Grab_Arm, glm::vec3(0.f, -0.2f, 0.05f));
	Grab_Arm = glm::scale(Grab_Arm, glm::vec3(0.03f, 0.03f, 0.1f + subMarine[id].scale_z));
	HtransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(HtransformLocation, 1, GL_FALSE, glm::value_ptr(Grab_Arm));
	objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, GLU_FILL);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 1.0f, 1.0f, 1.0f, 1.0);
	gluCylinder(gs->qobj, 1.0, 1.0, 2.0, 20, 8);

	glm::mat4 Grab_Arm_Finger1 = glm::mat4(1.0f);
	Grab_Arm_Finger1 = glm::translate(Grab_Arm_Finger1, glm::vec3(subMarine[id].trans_x, subMarine[id].trans_y, subMarine[id].trans_z));
	Grab_Arm_Finger1 = glm::rotate(Grab_Arm_Finger1, glm::radians(arm_rot), glm::vec3(1.0f, 0.0f, 0.0f));
	Grab_Arm_Finger1 = glm::rotate(Grab_Arm_Finger1, glm::radians(subMarine[id].rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	Grab_Arm_Finger1 = glm::translate(Grab_Arm_Finger1, glm::vec3(0.f, -0.2f, -0.25f + subMarine[id].trans_z_aoc));
	Grab_Arm_Finger1 = glm::rotate(Grab_Arm_Finger1, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
	Grab_Arm_Finger1 = glm::rotate(Grab_Arm_Finger1, glm::radians(45.f), glm::vec3(1.0f, 0.0f, 0.0f));
	Grab_Arm_Finger1 = glm::scale(Grab_Arm_Finger1, glm::vec3(0.03f, 0.005f, 0.09f));
	HtransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(HtransformLocation, 1, GL_FALSE, glm::value_ptr(Grab_Arm_Finger1));
	objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, GLU_FILL);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 1.0f, 1.0f, 1.0f, 1.0);
	gluCylinder(gs->qobj, 1.0, 1.0, 2.0, 20, 8);

	glm::mat4 Grab_Arm_Finger2 = glm::mat4(1.0f);
	Grab_Arm_Finger2 = glm::translate(Grab_Arm_Finger2, glm::vec3(subMarine[id].trans_x, subMarine[id].trans_y, subMarine[id].trans_z));
	Grab_Arm_Finger2 = glm::rotate(Grab_Arm_Finger2, glm::radians(arm_rot), glm::vec3(1.0f, 0.0f, 0.0f));
	Grab_Arm_Finger2 = glm::rotate(Grab_Arm_Finger2, glm::radians(subMarine[id].rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	Grab_Arm_Finger2 = glm::translate(Grab_Arm_Finger2, glm::vec3(0.f, -0.2f, -0.25f + subMarine[id].trans_z_aoc));
	Grab_Arm_Finger2 = glm::rotate(Grab_Arm_Finger2, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
	Grab_Arm_Finger2 = glm::rotate(Grab_Arm_Finger2, glm::radians(-45.f), glm::vec3(1.0f, 0.0f, 0.0f));
	Grab_Arm_Finger2 = glm::scale(Grab_Arm_Finger2, glm::vec3(0.03f, 0.005f, 0.09f));
	HtransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(HtransformLocation, 1, GL_FALSE, glm::value_ptr(Grab_Arm_Finger2));
	objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, GLU_FILL);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 1.0f, 1.0f, 1.0f, 1.0);
	gluCylinder(gs->qobj, 1.0, 1.0, 2.0, 20, 8);

	glm::mat4 Grab_Arm_Finger3 = glm::mat4(1.0f);
	Grab_Arm_Finger3 = glm::translate(Grab_Arm_Finger3, glm::vec3(subMarine[id].trans_x, subMarine[id].trans_y, subMarine[id].trans_z));
	Grab_Arm_Finger3 = glm::rotate(Grab_Arm_Finger3, glm::radians(arm_rot), glm::vec3(1.0f, 0.0f, 0.0f));
	Grab_Arm_Finger3 = glm::rotate(Grab_Arm_Finger3, glm::radians(subMarine[id].rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	Grab_Arm_Finger3 = glm::translate(Grab_Arm_Finger3, glm::vec3(0.f, -0.2f, -0.25f + subMarine[id].trans_z_aoc));
	Grab_Arm_Finger3 = glm::rotate(Grab_Arm_Finger3, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	Grab_Arm_Finger3 = glm::rotate(Grab_Arm_Finger3, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
	Grab_Arm_Finger3 = glm::rotate(Grab_Arm_Finger3, glm::radians(-45.f), glm::vec3(1.0f, 0.0f, 0.0f));
	Grab_Arm_Finger3 = glm::scale(Grab_Arm_Finger3, glm::vec3(0.03f, 0.005f, 0.09f));
	HtransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(HtransformLocation, 1, GL_FALSE, glm::value_ptr(Grab_Arm_Finger3));
	objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, GLU_FILL);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 1.0f, 1.0f, 1.0f, 1.0);
	gluCylinder(gs->qobj, 1.0, 1.0, 2.0, 20, 8);

	glm::mat4 Grab_Arm_Finger4 = glm::mat4(1.0f);
	Grab_Arm_Finger4 = glm::translate(Grab_Arm_Finger4, glm::vec3(subMarine[id].trans_x, subMarine[id].trans_y, subMarine[id].trans_z));
	Grab_Arm_Finger4 = glm::rotate(Grab_Arm_Finger4, glm::radians(arm_rot), glm::vec3(1.0f, 0.0f, 0.0f));
	Grab_Arm_Finger4 = glm::rotate(Grab_Arm_Finger4, glm::radians(subMarine[id].rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	Grab_Arm_Finger4 = glm::translate(Grab_Arm_Finger4, glm::vec3(0.f, -0.2f, -0.25f + subMarine[id].trans_z_aoc));
	Grab_Arm_Finger4 = glm::rotate(Grab_Arm_Finger4, glm::radians(-90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	Grab_Arm_Finger4 = glm::rotate(Grab_Arm_Finger4, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
	Grab_Arm_Finger4 = glm::rotate(Grab_Arm_Finger4, glm::radians(-45.f), glm::vec3(1.0f, 0.0f, 0.0f));
	Grab_Arm_Finger4 = glm::scale(Grab_Arm_Finger4, glm::vec3(0.03f, 0.005f, 0.09f));
	HtransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(HtransformLocation, 1, GL_FALSE, glm::value_ptr(Grab_Arm_Finger4));
	objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, GLU_FILL);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 1.0f, 1.0f, 1.0f, 1.0);
	gluCylinder(gs->qobj, 1.0, 1.0, 2.0, 20, 8);

}
GLvoid subMarine_collison() {
	for (int x = 0; x < 50; x++) {
		/*if (subMarine.trans_x <= wp_Bomb[x].transX + 0.2f && subMarine.trans_x >= wp_Bomb[x].transX - 0.2f &&
			subMarine.trans_y <= wp_Bomb[x].transY + 0.2f && subMarine.trans_y >= wp_Bomb[x].transY - 0.2f &&
			subMarine.trans_z <= wp_Bomb[x].transZ + 0.2f && subMarine.trans_z >= wp_Bomb[x].transZ - 0.2f
			) {
			subMarine.trans_x = 10.f;
			subMarine.trans_y = 1.f;
			subMarine.trans_z = 10.f;
		}*/
	}
}
GLvoid subMarine_Catch() {
	/*float test = subMarine.trans_z + subMarine.trans_z_aoc;
	for (int x = 0; x < 3; x++) {
		if (wp_Gemstone[x].transX - 0.6f <= subMarine.trans_x && subMarine.trans_x <= wp_Gemstone[x].transX + 0.6f &&
			wp_Gemstone[x].transY - 1.f <= subMarine.trans_y && subMarine.trans_y <= wp_Gemstone[x].transY + 1.f &&
			wp_Gemstone[x].transZ - 0.35f <= subMarine.trans_z && subMarine.trans_z <= wp_Gemstone[x].transZ + 0.35f) {
			wp_Gemstone[x].get = true;
		}
	}
	for (int x = 0; x < 3; x++) {
		if (wp_Gemstone[x].get) {
			wp_Gemstone[x].transX = subMarine.trans_x;
			wp_Gemstone[x].transY = subMarine.trans_y;
			wp_Gemstone[x].transZ = subMarine.trans_z;
		}
	}
	for (int x = 0; x < 3; x++) {
		if (wp_Gemstone[x].transX - 0.6f <= 11.f && 9.f <= wp_Gemstone[x].transX + 0.6f &&
			wp_Gemstone[x].transY - 1.f <= 1.f && 0.f <= wp_Gemstone[x].transY + 1.f &&
			wp_Gemstone[x].transZ - 0.35f <= 11.f && 9.f <= wp_Gemstone[x].transZ + 0.35f) {
			wp_Gemstone[x].get = false;
			wp_Gemstone[x].transX = 10.f;
			wp_Gemstone[x].transY = 0.f;
			wp_Gemstone[x].transZ = 10.f;
		}
	}*/

}
void drawScene()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //깊이 체크 (컬링)
	glUseProgram(gs->s_program);
	for (int i = 0; i < 2; ++i) {

		if (i == 1) {
			glViewport(0, 0, width, height);
			glm::vec3 cameraPos = glm::vec3(subMarine[g_id].trans_x, subMarine[g_id].trans_y + 1.5f, subMarine[g_id].trans_z); //--- 카메라 위치
			glm::vec3 cameraDirection = glm::vec3(subMarine[g_id].trans_x, 0.3f, subMarine[g_id].trans_z - 5); //--- 카메라 바라보는 방향
			glm::vec3 cameraUp = glm::vec3(0.0f, 4.0f, 0.0f); //--- 카메라 위쪽 방향
			glm::mat4 view = glm::mat4(1.0f);
			view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
			view = glm::translate(view, glm::vec3(subMarine[g_id].trans_x, subMarine[g_id].trans_y + 1.0f, subMarine[g_id].trans_z));
			view = glm::rotate(view, glm::radians(CO.camera_drgree_y), glm::vec3(1.0f, 0.0f, 0.0f));
			view = glm::rotate(view, glm::radians(CO.camera_drgree_x), glm::vec3(0.0f, 1.0f, 0.0f));
			view = glm::translate(view, glm::vec3(-subMarine[g_id].trans_x, -subMarine[g_id].trans_y - 1.0f, -subMarine[g_id].trans_z));

			unsigned int viewLocation = glGetUniformLocation(gs->s_program, "view"); //--- 뷰잉 변환 설정
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

			glm::mat4 projection = glm::mat4(1.0f);
			projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
			projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0));
			unsigned int projectionLocation = glGetUniformLocation(gs->s_program, "projection");
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

		}
		else {
			glViewport(1050, 550, 150, 150);
			glm::vec3 cameraPos = glm::vec3(subMarine[g_id].trans_x, 5.0f, subMarine[g_id].trans_z);         //위치
			glm::vec3 cameraDirection = glm::vec3(subMarine[g_id].trans_x, 0.0f, subMarine[g_id].trans_z);   //바라보는 방향
			glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, -10.0f);         //카메라 상향
			glm::mat4 view = glm::mat4(1.0f);
			view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

			unsigned int viewLocation = glGetUniformLocation(gs->s_program, "view");
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
			glm::mat4 projection = glm::mat4(1.0f);
			projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
			projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0));
			unsigned int projectionLocation = glGetUniformLocation(gs->s_program, "projection");
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

		}
		//상민
		wp_Bubble->Bubble_Mat(gs);
		bg->Background_Mat(gs);
		wp_Gemstone->Ore_Mat(gs, fGemstoneX, fGemstoneY, fGemstoneZ);
		wp_Bomb->Bomb_Mat(gs);
		bg->Warehouse_Mat(gs);
		//은형
		for (int i = 0;i < 3;++i) {
			Draw_SubMarine(i);
		}
		subMarine_collison();
		subMarine_Catch();
	}
		glutSwapBuffers();
}
//        //상민
//        wp_Bubble->Bubble_Mat(gs);
//        bg->Background_Mat(gs);
//        wp_Gemstone->Ore_Mat(gs, receivedGemStonePacket);
//        wp_Bomb->Bomb_Mat(gs);
//        bg->Warehouse_Mat(gs);
//        //은형
//        Draw_SubMarine();
//        subMarine_collison();
//        subMarine_Catch();
//    }
//
//	
//}
GLvoid KeyBoardUp(unsigned char key, int x, int y) {
	INPUT_PACKET* packet = new INPUT_PACKET;
	switch (key)
	{
	case 'a':
		packet->type = SC_KEY_INPUT;
		packet->input = 'a';
		packet->bKeyDown = false;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	case 'd':
		packet->type = SC_KEY_INPUT;
		packet->input = 'd';
		packet->bKeyDown = false;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	case 'w':
		packet->type = SC_KEY_INPUT;
		packet->input = 'w';
		packet->bKeyDown = false;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	case 's':
		packet->type = SC_KEY_INPUT;
		packet->input = 's';
		packet->bKeyDown = false;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	}
	delete packet;
	glutPostRedisplay();
}
GLvoid KeyBoard(unsigned char key, int x, int y) {
		INPUT_PACKET* packet = new INPUT_PACKET;
	switch (key)
	{
	case 'r':
	{
		wp_Gemstone->Reset();
		wp_Bomb->Reset();
		break;
	}
	case ' ':
	{
		Grab_Arm_Long = !Grab_Arm_Long;
		break;
	}
	case '8':
	{
		jump_key = true;
		break;
	}
	case '5':
	{
		jump_down_key = true;
		break;
	}
	case 'a':
	{
		packet->type = SC_KEY_INPUT;
		packet->input = 'a';
		packet->bKeyDown = true;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	}
	case 'd':
	{
		packet->type = SC_KEY_INPUT;
		packet->input = 'd';
		packet->bKeyDown = true;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	}
	case 'w':
	{
		//up_key = true;
		packet->type = SC_KEY_INPUT;
		packet->input = 'w';
		packet->bKeyDown = true;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	}
	case 's':
	{
		packet->type = SC_KEY_INPUT;
		packet->input = 's';
		packet->bKeyDown = true;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	}
	}
		delete packet;
	glutPostRedisplay();
}
GLvoid SpecialKeyBoard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		C_R_U = !C_R_U;
		break;
	case GLUT_KEY_DOWN:
		C_R_D = !C_R_D;
		break;
	case GLUT_KEY_LEFT:
		C_R_L = !C_R_L;
		break;
	case GLUT_KEY_RIGHT:
		C_R_R = !C_R_R;
		break;
	case GLUT_KEY_SHIFT_L:
		R_key = false;
		L_key = !L_key;
		break;
	case GLUT_KEY_SHIFT_R:
		L_key = false;
		R_key = !R_key;
		break;
	}
	glutPostRedisplay();
}
GLvoid SpecialKeyBoardUp(int key, int x, int y)
{
	switch (key) {
	}
	glutPostRedisplay();
}
void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		MH.left_button = true;
		MH.right_button = false;
		MH.first_mouse_pos_x = x;
		MH.first_mouse_pos_y = y;
	}
	if (button == GLUT_RIGHT_BUTTON) {
		MH.left_button = false;
		MH.right_button = true;
		MH.first_mouse_pos_x = x;
		MH.first_mouse_pos_y = y;
	}

}
void Motion(int x, int y)
{
	MouseChange(x, y);
	if (MH.left_button == true)
	{
		MH.delta_mouse_x = MH.mouse_x - MH.first_mouse_pos_x;
		MH.delta_mouse_y = MH.mouse_y - MH.first_mouse_pos_y;
		CO.camera_drgree_x += MH.delta_mouse_x / (width / 2);
		CO.camera_drgree_y += MH.delta_mouse_y / (height / 2);
	}
}
void MouseChange(int x, int y) {
	MH.mouse_x = x;
	MH.mouse_y = y;
}


GLvoid Setting(int id)
{
	subMarine[id].trans_x = 10.f;
	subMarine[id].trans_y = 1.f;
	subMarine[id].trans_z = 10.f;
	subMarine[id].scale_z = 0.f;
	subMarine[id].trans_z_aoc = 0.f;
}
GLvoid Timer(int value)
{
	MH.first_mouse_pos_flag = true;

	if (L_key == true && arm_rot <= 20.f) {
		arm_rot += 1.f;
	}
	if (R_key == true && arm_rot >= -30.f) {
		arm_rot -= 1.f;
	}
	if (Grab_Arm_Long == true && subMarine[g_id].scale_z <= 0.2f) {
		subMarine[g_id].scale_z += 0.01f;
	}
	if (Grab_Arm_Long == false && subMarine[g_id].scale_z != 0.f) {
		subMarine[g_id].scale_z -= 0.01f;
	}
	if (Grab_Arm_Long == true && subMarine[g_id].trans_z_aoc >= -0.35f) subMarine[g_id].trans_z_aoc -= 0.02f;
	if (Grab_Arm_Long == false && subMarine[g_id].trans_z_aoc <= 0.f)subMarine[g_id].trans_z_aoc += 0.02f;


	//if (right_key) {
	//	rot -= 1.f;
	//}
	//if (left_key) {
	//	rot += 1.f;
	//}
	//if (up_key) {
	//	subMarine[g_id].trans_x -= 0.03f * sin(rot * atan(1) * 4 / 180);
	//	subMarine[g_id].trans_z -= 0.03f * cos(rot * atan(1) * 4 / 180);
	//}
	//if (down_key) {
	//	subMarine[g_id].trans_x += 0.03f * sin(rot * atan(1) * 4 / 180);
	//	subMarine[g_id].trans_z += 0.03f * cos(rot * atan(1) * 4 / 180);
	//}
	rot_t += 10.f;
	glutPostRedisplay();
	glutTimerFunc(5, Timer, 1);
}

void SendPlayerPosition() {
   
    //MOVE_PACKET movePacket;
    //movePacket.iType = 1;  
    //movePacket.fX = subMarine.trans_x;
    //movePacket.fY = subMarine.trans_y;
    //movePacket.fZ = subMarine.trans_z;

   
    //size_t bytes_sent = send(sock, (char*)&movePacket, sizeof(MOVE_PACKET), 0);
    //if (bytes_sent == -1) {
    //    std::cerr << "Error" << std::endl;
    //}
}

void ReceiveGemStonePacket(SOCKET serverSocket, char buf[BUFSIZE+1]) {

	GemStonePacket* gemstonePacket = reinterpret_cast<GemStonePacket*>(buf);
	for (int i = 0;i < 9;++i) {
		fGemstoneX[gemstonePacket[i].num] = gemstonePacket[i].fX;
		fGemstoneY[gemstonePacket[i].num] = gemstonePacket[i].fY;
		fGemstoneZ[gemstonePacket[i].num] = gemstonePacket[i].fZ;
	}
}

void ReceiveGemStonePacket(SOCKET serverSocket) {
	for (int i = 0; i < 9; ++i)
	{
		int bytesReceived = 0;
		recv(serverSocket, reinterpret_cast<char*>(&receivedGemStonePacket[i]), sizeof(receivedGemStonePacket[0]), 0);
		fGemstoneX[i] = receivedGemStonePacket[i].fX;
		fGemstoneY[i] = receivedGemStonePacket[i].fY;
		fGemstoneZ[i] = receivedGemStonePacket[i].fZ;
		if (bytesReceived == SOCKET_ERROR) {
			err_display("recv()");
			return;
		}
	}

}

void SendNameToServer(SOCKET clientSocket) {
	char clientName[256];
	printf("Input Name: ");
	fgets(clientName, sizeof(clientName), stdin);
	clientName[strcspn(clientName, "\n")] = '\0';

	// 입력받은 이름을 서버에 전송
	send(clientSocket, clientName, strlen(clientName), 0);
}

DWORD WINAPI ProcessClient(LPVOID arg)
{

	int retval;
	SOCK_INFO* sock_info = reinterpret_cast<SOCK_INFO*> (arg);

	SOCKET server_sock = sock_info->server_sock;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];
	int id{};

	while (1) {
		// 데이터 받기
			//for (int i = 0;i < 2;++i) {
		retval = recv(server_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		switch (buf[0]) {
		case SC_SEND_PLAYER:
		{
			SEND_PLAYER* packet_sp = reinterpret_cast<SEND_PLAYER*>(buf);
			id = packet_sp->id;
			break;
		}
		case SC_PLAYER_MOVE:
		{
			MOVE_PACKET* packet_tr = reinterpret_cast<MOVE_PACKET*>(buf);
			subMarine[id].trans_x = packet_tr->fx;
			subMarine[id].trans_y = packet_tr->fy;
			subMarine[id].trans_z = packet_tr->fz;
			subMarine[id].rotate_y = packet_tr->fro;
			std::cout << id << " " << packet_tr->fz << endl;
			break;
		}
		case SC_PLAYER_ROTATE:
		{
			ROTATE_PACKET* packet_ro = reinterpret_cast<ROTATE_PACKET*>(buf);
			subMarine[id].rotate_y = packet_ro->fy;
			//std::cout << packet_ro->fy << endl;
			break;
		}
		case PACKET_GEMSTONE:
		{
			ReceiveGemStonePacket(sock_info->server_sock,buf);
		}
		}
		//}
	}
	// 소켓 닫기
	delete sock_info;
	closesocket(server_sock);

	return 0;
}

int main(int argc, char* argv[])
{
	int retval;

	// 명령행 인수가 있으면 IP 주소로 사용
	if (argc > 1) SERVERIP = argv[1];

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	sock_info = new SOCK_INFO;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET) err_quit("socket()");



	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
	sock_info->server_sock = sock;


	////////////////////////////
	SendNameToServer(sock);        //서버에 이름을 보낸다.


	// 서버로부터의 게임 시작 여부 확인
	char cStartBuffer[BUFSIZE] = "";
	recv(sock, (char*)cStartBuffer, 5, 0);
	cStartBuffer[5] = '\0';
	if (strcmp(cStartBuffer, "START") == 0) {
		recv(sock, reinterpret_cast<char*>(&g_id), sizeof(int), 0);
		std::cout << "[" << g_id << "]" << "클라이언트의 게임이 시작되었습니다!" << endl;
		HANDLE hThread;
		hThread = CreateThread(NULL, 0, ProcessClient,
			reinterpret_cast<LPVOID*>(sock_info->GetSockInfo()), 0, NULL);
		if (hThread == NULL) { closesocket(sock); }
		else { CloseHandle(hThread); }
		ReceiveGemStonePacket(sock);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("BLUE HOLE");
	for(int i=0;i<3;++i)
	Setting(i);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cerr << "NOT INIT" << endl;
	}
	else
	std::cout << "INIT<<endl";
	gs->InitShader();
	gs->InitBuffer();
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(KeyBoard);
	glutSpecialFunc(SpecialKeyBoard);
	glutSpecialUpFunc(SpecialKeyBoardUp);
	glutKeyboardUpFunc(KeyBoardUp);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutReshapeFunc([](int w, int h) {
		glViewport(0, 0, w, h);
	});
	glutTimerFunc(5, Timer, 1);
	glutMainLoop();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	

	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}

