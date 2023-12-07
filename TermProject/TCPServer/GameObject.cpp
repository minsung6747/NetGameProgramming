#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Background::Background()
{
}

Background::~Background()
{
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Gemstone::Gemstone()
{
}

Gemstone::~Gemstone()
{
}

GLvoid Gemstone::SetPosition()
{
	if (ore == NULL) {
		ore = new GameObject[3];
		uniform_int_distribution<int> dis_xz{ 0,2000 };
		while (true) {
			bool flag{ false };
			for (int i = 0; i < 3; ++i) {
				ore[i].transX = dis_xz(gen) / 100.;
				ore[i].transZ = dis_xz(gen) / 100.;
			}
			for (int i = 0; i < 3; ++i) {
				if (pow(9, 2) < pow(ore[i].transZ - 10, 2) + pow(ore[i].transX - 10, 2))
					flag = true;
			}
			if (!flag)break;
		}
	}
}

GLvoid Gemstone::Reset()
{
    delete[] ore;
    ore = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Bomb::SetPosition()
{
    if (bomb == NULL) {
        bomb = new GameObject[50];
        uniform_int_distribution<int> dis_y{ 20,70 };
        uniform_int_distribution<int> dis_xz{ 0,2000 };
        uniform_int_distribution<int> dis0{ 1,2 };
        for (int i = 0; i < 50; ++i) {
            bomb[i].transX = dis_xz(gen) / 100.;
            bomb[i].transZ = dis_xz(gen) / 100.;
            bomb[i].MaxTransY = dis_y(gen) / 10.;
            bomb[i].transY = bomb[i].MaxTransY;
            bomb[i].transY_aoc = dis0(gen) / 100.;
        }
    }
}

Bomb::Bomb()
{
	mp_IsUp = new bool[50];
}

Bomb::~Bomb()
{
	delete[] mp_IsUp;
}

GLvoid Bomb::Reset()
{
    delete[] bomb;
    bomb = NULL;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Bubble::Bubble()
{
	temp = new GameObject[3];
}

Bubble::~Bubble()
{
	delete[] temp;
}

GLvoid Bubble::Bubble_Timer()
{
    for (int j = 0; j < 100; ++j) {
        for (int i = 0; i < 3; ++i) {
            if (mpp_Bubble[j][i].color_b > 0.5f) {
                mpp_Bubble[j][i].color_b -= mpp_Bubble[j][i].colorB_aoc;
                mpp_Bubble[j][i].transY += mpp_Bubble[j][i].transY_aoc;
                float move_x = 1 / 30. * cos(mpp_Bubble[j][i].m_radius * atan(1) * 4 / 180);
                mpp_Bubble[j][i].m_radius += 7;
                mpp_Bubble[j][i].transX_aoc = move_x;
            }
            else {
                memcpy(&mpp_Bubble[j], &temp, sizeof mpp_Bubble[j]);
                Bubble_Setting(j);
            }
        }
    }
}

GLvoid Bubble::Bubble_Setting(int j)
{
    float random_x{};
    float random_z{};
    float random_scale{};
    float random_color_aoc{};
    float random_y_aoc{};
    uniform_int_distribution<int> dis{ 1,200 };
    uniform_int_distribution<int> dis0{ 100,200 };
    uniform_int_distribution<int> dis1{ 0,5 };
    if (mpp_Bubble[j][0].transY == 0.0) {
        mpp_Bubble[j][2].transY = 0.0f;
        mpp_Bubble[j][1].transY = -0.2f;
        mpp_Bubble[j][0].transY = -0.4f;
        mpp_Bubble[j][1].m_radius = -30;
        mpp_Bubble[j][0].m_radius = -60;
        while (true) {
            random_x = dis(gen) / 10.;
            random_z = dis(gen) / 10.;

            random_scale = dis0(gen) / 300.;
            random_color_aoc = dis(gen) / 100000.;
            random_y_aoc = dis0(gen) / 20000.;
            if (pow(10, 2) > pow(random_x - 10, 2) + pow(random_z - 10, 2))break;
        }
        for (int i = 0; i < 3; ++i) {
            mpp_Bubble[j][i].transX = random_x;
            mpp_Bubble[j][i].transZ = random_z;
            mpp_Bubble[j][i].scaleX = (0.5f + i / 10.) * random_scale;
            mpp_Bubble[j][i].scaleY = (0.5f + i / 10.) * random_scale;
            mpp_Bubble[j][i].scaleZ = (0.5f + i / 10.) * random_scale;

            mpp_Bubble[j][i].color_r = 0.2f;
            mpp_Bubble[j][i].color_g = 0.2f;
            mpp_Bubble[j][i].color_b = 1.0f;
            mpp_Bubble[j][i].colorB_aoc = random_color_aoc;

            mpp_Bubble[j][i].transX_aoc = 0.001f;
            mpp_Bubble[j][i].transY_aoc = random_y_aoc;
            mpp_Bubble[j][i].transZ_aoc = 0.001f;
        }
    }

}

GLvoid Bubble::Bubbles_Setting()
{
    if (mpp_Bubble != NULL) Bubble_Timer();
    if (mpp_Bubble == NULL) {
        mpp_Bubble = new GameObject * [100];
        for (int i = 0;i < 100;++i)
            mpp_Bubble[i] = new GameObject[3];

        uniform_int_distribution<int> dis{ 1,200 };
        uniform_int_distribution<int> dis0{ 100,200 };
        uniform_int_distribution<int> dis1{ 0,5 };
        for (int j = 0; j < 100; ++j) {
            if (mpp_Bubble[j][0].transY == 0) {
                mpp_Bubble[j][2].transY = 0.0f;
                mpp_Bubble[j][1].transY = -0.2f;
                mpp_Bubble[j][0].transY = -0.4f;
                mpp_Bubble[j][1].m_radius = -30;
                mpp_Bubble[j][0].m_radius = -60;
                float random_x = dis(gen) / 10.;
                float random_z = dis(gen) / 10.;
                float random_scale = dis0(gen) / 300.;
                float random_color_aoc = dis(gen) / 100000.;
                float random_y_aoc = dis0(gen) / 20000.;
                for (int i = 0; i < 3; ++i) {
                    mpp_Bubble[j][i].transX = random_x;
                    mpp_Bubble[j][i].transZ = random_z;

                    mpp_Bubble[j][i].scaleX = (0.5f + i / 10.) * random_scale;
                    mpp_Bubble[j][i].scaleY = (0.5f + i / 10.) * random_scale;
                    mpp_Bubble[j][i].scaleZ = (0.5f + i / 10.) * random_scale;

                    mpp_Bubble[j][i].color_r = 0.2f;
                    mpp_Bubble[j][i].color_g = 0.2f;
                    mpp_Bubble[j][i].color_b = 1.0f;
                    mpp_Bubble[j][i].colorB_aoc = random_color_aoc;

                    mpp_Bubble[j][i].transX_aoc = 0.001f;
                    mpp_Bubble[j][i].transY_aoc = random_y_aoc;
                    mpp_Bubble[j][i].transZ_aoc = 0.001f;
                }
            }
        }
    }
}


Player::Player()
{
}

Player::~Player()
{
}
