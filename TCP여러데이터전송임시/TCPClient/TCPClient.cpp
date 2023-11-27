#include "..\Common.h"
#include "stdafx.h"
#include "Camera.h"
#include "GameShader.h"
#include "GameObject.h"
#include <GL/glew.h>

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512


random_device rd;
mt19937 gen{ rd() };

//Camera CO;
GameShader* gs = new GameShader;
Background* bg = new Background();
Gemstone* wp_Gemstone = new Gemstone();
Bomb* wp_Bomb = new Bomb();
Bubble* wp_Bubble = new Bubble();
GamePlayer* wp_Player = new GamePlayer();

//alstkdalsdl0
GLvoid drawScene();
GLvoid KeyBoard(unsigned char, int, int);
GLvoid KeyBoardUp(unsigned char, int, int);
GLvoid SpecialKeyBoard(int, int, int);
GLvoid SpecialKeyBoardUp(int, int, int);
GLvoid Timer(int);
GLvoid MouseChange(int, int);
GLvoid Mouse(int, int);

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

//O subMarine;

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
GLvoid Setting();
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

GLfloat rot;
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

void drawScene()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //깊이 체크 (컬링)
    glUseProgram(gs->s_program);
    for (int i = 0; i < 2; ++i) {

        if (i == 1) {
            wp_Player->Draw_Camera(gs);

        }
        else {
            wp_Player->Draw_Map(gs);

        }
        //상민
        wp_Bubble->Bubble_Mat(gs);
        bg->Background_Mat(gs);
        wp_Gemstone->Ore_Mat(gs);
        wp_Bomb->Bomb_Mat(gs);
        bg->Warehouse_Mat(gs);
        //은형
        wp_Player->Draw_SubMarine(gs);
        /*subMarine_collison();
        subMarine_Catch();*/
    }

    glutSwapBuffers();
}
GLvoid KeyBoardUp(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'a':
        left_key = false;
        break;
    case 'd':
        right_key = false;
        break;
    case 'w':
        up_key = false;
        break;
    case 's':
        down_key = false;
        break;
    }
    glutPostRedisplay();
}
GLvoid KeyBoard(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'r':
        wp_Gemstone->Reset();
        wp_Bomb->Reset();
        break;
    case ' ':
        Grab_Arm_Long = !Grab_Arm_Long;
        break;
    case '8':
        jump_key = true;
        break;
    case '5':
        jump_down_key = true;
        break;
    case 'a':
        left_key = true;
        break;
    case 'd':
        right_key = true;
        break;
    case 'w':
        up_key = true;
        break;
    case 's':
        down_key = true;
        break;
    }
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
      /*  CO.camera_drgree_x += MH.delta_mouse_x / (width / 2);
        CO.camera_drgree_y += MH.delta_mouse_y / (height / 2);*/
    }
}
void MouseChange(int x, int y) {
    MH.mouse_x = x;
    MH.mouse_y = y;
}



GLvoid Timer(int value)
{
    MH.first_mouse_pos_flag = true;

    //if (L_key == true && arm_rot <= 20.f) {
    //    arm_rot += 1.f;
    //}
    //if (R_key == true && arm_rot >= -30.f) {
    //    arm_rot -= 1.f;
    //}
    //if (Grab_Arm_Long == true && subMarine.scale_z <= 0.2f) {
    //    subMarine.scale_z += 0.01f;
    //}
    //if (Grab_Arm_Long == false && subMarine.scale_z != 0.f) {
    //    subMarine.scale_z -= 0.01f;
    //}
    //if (Grab_Arm_Long == true && subMarine.trans_z_aoc >= -0.35f) subMarine.trans_z_aoc -= 0.02f;
    //if (Grab_Arm_Long == false && subMarine.trans_z_aoc <= 0.f)subMarine.trans_z_aoc += 0.02f;


    //if (right_key) {
    //    rot -= 1.f;
    //}
    //if (left_key) {
    //    rot += 1.f;
    //}
    //if (up_key) {
    //    subMarine.trans_x -= 0.03f * sin(rot * atan(1) * 4 / 180);
    //    subMarine.trans_z -= 0.03f * cos(rot * atan(1) * 4 / 180);
    //}
    //if (down_key) {
    //    subMarine.trans_x += 0.03f * sin(rot * atan(1) * 4 / 180);
    //    subMarine.trans_z += 0.03f * cos(rot * atan(1) * 4 / 180);
    //}
    //rot_t += 10.f;
    glutPostRedisplay();
    glutTimerFunc(5, Timer, 1);
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
    SOCKET sock1 = socket(AF_INET, SOCK_STREAM, 0);
    SOCKET sock2 = socket(AF_INET, SOCK_STREAM, 0);
    if (sock1 == INVALID_SOCKET) err_quit("socket()");
    if (sock2 == INVALID_SOCKET) err_quit("socket2()");



    // connect()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);

    retval = connect(sock1, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");
    retval = connect(sock2, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect2()");

    // 데이터 통신에 사용할 변수

    char playername[BUFSIZE + 1];

    printf("이름을 입력해주세요 : ");
    scanf("%s", &playername);

    // playername 보내기
    retval = send(sock1, playername, (int)strlen(playername), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return 1;
    }
    printf("playername 전송 완료\n");
    
    int iForReady = 0;
    printf("Are you Ready For Playing Game??!\n");
    printf("Press 1 or 2 (1:Ready 2:Not Ready)\n");
    //scanf("%d", &iForReady);
 
    //// Ready 상태를 서버에 전송
    //if (iForReady == 1) {
    //    const char* readyMessage = "Ready";
    //    retval = send(sock1, readyMessage, (int)strlen(readyMessage), 0);
    //    if (retval == SOCKET_ERROR) {
    //        err_display("send()");
    //        return 1;
    //    }
    //    printf("Ready 상태를 서버에 전송했습니다.\n");
    //}
	 glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(width, height);
    glutCreateWindow("BLUE HOLE");
    //Setting();

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cerr << "NOT INIT" << endl;
    }
    else
        cout << "INIT<<endl";
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

	

	

	char playersize[BUFSIZE + 1] = "100m";

	// playersize 보내기
	retval = send(sock2, playersize, (int)strlen(playersize), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()2");
		return 1;
	}
	printf("playersize 전송 완료\n");

	// printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

	// 소켓 닫기
	closesocket(sock1);
	closesocket(sock2);

	// 윈속 종료
	WSACleanup();
	return 0;
}
