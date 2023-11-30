#pragma once
#include "stdafx.h"

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

    bool get{ false }, put{ false };

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Player : public GameObject {
public:
    char type;
    Player();
    ~Player();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Background :public GameObject {
public:
	Background();
	~Background();

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
    GLvoid Reset();

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


};

