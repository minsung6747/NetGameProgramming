#pragma once
#include "GameObject.h"

class GamePlayer : public GameObject
{

public:
	GamePlayer();
	~GamePlayer();
    GLvoid subMarine_Catch();
	

private:
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

};

