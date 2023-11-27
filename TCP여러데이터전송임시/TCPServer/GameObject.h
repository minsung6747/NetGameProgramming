#pragma once
#include <iostream>
#include <random>

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

    std::random_device rd;
    std::mt19937 gen{ rd() };

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

