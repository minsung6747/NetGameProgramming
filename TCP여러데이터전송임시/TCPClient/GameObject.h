#pragma once
#include "GameShader.h"


struct BoundingBox {
    float m_fMaxX, m_fMinX;
    float m_fMaxY, m_fMinY;
    float m_fMaxZ, m_fMinZ;
};

class GameObject
{
public:
	GameObject();
	~GameObject();
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

	random_device rd;
	mt19937 gen{ rd() };

    float transX{}, transY{}, transZ{};

    float rotateX{}, rotateY{}, rotateZ{};

    float scaleX{ 1.0f }, scaleY{ 1.0f }, scaleZ{ 1.0f };

    float color_r{}, color_g{}, color_b{};

    float transX_aoc{}, transY_aoc{}, transZ_aoc{};      // ÁÂÇ¥ º¯È­·® aoc - amount of change

    float MaxTransY{};

    float rotateX_aoc{}, rotateY_aoc{}, rotateZ_aoc{};   // You know what im say??

    float scaleX_aoc{}, scaleY_aoc{}, scaleZ_aoc{};

    float colorR_aoc{ 0.001 }, colorG_aoc{ 0.001 }, colorB_aoc{ 0.001 };

    float m_radius{};

    BoundingBox m_bbBox;

    bool get{ false }, put{ false };

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Background :public GameObject {
public:
	Background();
	~Background();

    GLvoid Background_Mat(GameShader* gs);
    GLvoid Warehouse_Mat(GameShader* gs);

private:
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Gemstone :public GameObject
{
public:
    Gemstone();
    ~Gemstone();

private:
	GLvoid SetPosition() ;

public:
    GLvoid Ore_Mat(GameShader* gs);
    GLvoid Reset();
    GLvoid UpdateBoundingBox();

private:

	GameObject* ore = NULL;

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Bomb : public GameObject
{

private:
    void SetPosition();

    GameObject* bomb = NULL;
    bool* mp_IsUp = NULL;

public:
    Bomb();
    ~Bomb();

    GLvoid Reset();

    GLvoid Bomb_Mat(GameShader* gs);
    GLvoid UpdateBoundingBox();

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Bubble :public GameObject
{
public:
    Bubble();
    ~Bubble();

private:
    GameObject* temp;
    GLvoid Bubble_Timer();
   
    GLvoid Bubble_Setting(int j);
    
    GLvoid Bubbles_Setting();
   
    GameObject** mpp_Bubble = NULL;

public:
    GLvoid Bubble_Mat(GameShader* gs);


};

class GamePlayer : public GameObject
{

public:
    GamePlayer();
    ~GamePlayer();
    GLvoid subMarine_Catch();
    GLvoid Draw_SubMarine(GameShader* gs);
    GLvoid Draw_Camera(GameShader* gs);
    GLvoid Draw_Map(GameShader* gs);

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
private:

    GameObject* subMarine = NULL;
};