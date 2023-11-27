#include "stdafx.h"
#include "GameObject.h"
#include "Camera.h"

Camera CO;

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

GLvoid Background::Background_Mat(GameShader* gs)
{
	glm::mat4 floor = glm::mat4(1.0f);
	floor = glm::scale(floor, glm::vec3(100.f, 1.0f, 100.f));
	floor = glm::translate(floor, glm::vec3(0.1f, 0.0f, 0.1f));
	floor = glm::rotate(floor, glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));
	unsigned int StransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(floor));
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, gs->obj_type);
	int objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	unsigned isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 0.266f, 0.226f, 0.14f, 1.0);
	gluDisk(gs->qobj, 0, 0.1f, 128, 1);

	glm::mat4 sea_mat = glm::mat4(1.0f);
	sea_mat = glm::scale(sea_mat, glm::vec3(100.f, 100.f, 100.f));
	sea_mat = glm::translate(sea_mat, glm::vec3(0.1f, 0.1f, 0.1f));
	sea_mat = glm::rotate(sea_mat, glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));
	StransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(sea_mat));
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, gs->obj_type);
	objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 0.2f, 0.2f, 0.5f, 1.0);
	gluCylinder(gs->qobj, 0.1f, 0.1f, 0.2f, 128, 1);
}

GLvoid Background::Warehouse_Mat(GameShader* gs)
{
	glm::mat4 Bottom_Matrix = glm::mat4(1.0f);
	Bottom_Matrix = glm::translate(Bottom_Matrix, glm::vec3(10.0f, 0.0f, 10.0f));
	Bottom_Matrix = glm::scale(Bottom_Matrix, glm::vec3(10.f, 10.0f, 10.f));
	Bottom_Matrix = glm::translate(Bottom_Matrix, glm::vec3(0.0f, 0.11f, 0.0f));
	unsigned int StransformLocation = glGetUniformLocation(gs->s_program, "transform");
	glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(Bottom_Matrix));
	gs->qobj = gluNewQuadric();
	gluQuadricDrawStyle(gs->qobj, gs->obj_type);
	int objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
	unsigned isCheck = glGetUniformLocation(gs->s_program, "isCheck");
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 0.2f, 0.0f, 0.2f, 1.0);
	glBindVertexArray(gs->VAO[0]);
	glDrawArrays(GL_TRIANGLES, 12, 24);

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
GLvoid Gemstone::Ore_Mat(GameShader* gs)
{
    SetPosition();
    glm::mat4 rock_mat = glm::mat4(1.0f);
    /*if(!ore[1].put&& !ore[1].get)*/rock_mat = glm::translate(rock_mat, glm::vec3(ore[1].transX, ore[1].transY, ore[1].transZ));
    rock_mat = glm::scale(rock_mat, glm::vec3(1.f, 5.f, 1.f));
    rock_mat = glm::rotate(rock_mat, glm::radians(45.f), glm::vec3(1.0f, .0f, 0.0f));
    rock_mat = glm::rotate(rock_mat, glm::radians(45.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock_mat = glm::translate(rock_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    unsigned int StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    unsigned objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    int isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0);
    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock_mat = glm::mat4(1.0f);
    rock_mat = glm::translate(rock_mat, glm::vec3(ore[1].transX, ore[1].transY, ore[1].transZ));
    rock_mat = glm::rotate(rock_mat, glm::radians(25.f), glm::vec3(1.0f, 0.0f, 0.0f));
    rock_mat = glm::scale(rock_mat, glm::vec3(1.f, 3.f, 1.f));
    rock_mat = glm::rotate(rock_mat, glm::radians(45.f), glm::vec3(1.0f, .0f, 0.0f));
    rock_mat = glm::rotate(rock_mat, glm::radians(45.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock_mat = glm::translate(rock_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0);
    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock_mat = glm::mat4(1.0f);
    rock_mat = glm::translate(rock_mat, glm::vec3(ore[1].transX, ore[1].transY, ore[1].transZ));
    rock_mat = glm::rotate(rock_mat, glm::radians(-30.f), glm::vec3(1.0f, 0.0f, 0.0f));
    rock_mat = glm::scale(rock_mat, glm::vec3(1.f, 3.f, 1.f));
    rock_mat = glm::rotate(rock_mat, glm::radians(45.f), glm::vec3(1.0f, .0f, 0.0f));
    rock_mat = glm::rotate(rock_mat, glm::radians(45.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock_mat = glm::translate(rock_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0);
    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 rock1_mat = glm::mat4(1.0f);
    rock1_mat = glm::translate(rock1_mat, glm::vec3(ore[2].transX, ore[2].transY, ore[2].transZ));
    rock1_mat = glm::scale(rock1_mat, glm::vec3(1.f, 5.f, 1.f));
    rock1_mat = glm::rotate(rock1_mat, glm::radians(37.f), glm::vec3(1.0f, .0f, 0.0f));
    rock1_mat = glm::rotate(rock1_mat, glm::radians(26.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock1_mat = glm::translate(rock1_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock1_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0);
    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock1_mat = glm::mat4(1.0f);
    rock1_mat = glm::translate(rock1_mat, glm::vec3(ore[2].transX, ore[2].transY, ore[2].transZ));
    rock1_mat = glm::rotate(rock1_mat, glm::radians(15.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock1_mat = glm::scale(rock1_mat, glm::vec3(1.f, 3.f, 1.f));
    rock1_mat = glm::rotate(rock1_mat, glm::radians(30.f), glm::vec3(1.0f, .0f, 0.0f));
    rock1_mat = glm::rotate(rock1_mat, glm::radians(23.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock1_mat = glm::translate(rock1_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock1_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0);
    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock1_mat = glm::mat4(1.0f);
    rock1_mat = glm::translate(rock1_mat, glm::vec3(ore[2].transX, ore[2].transY, ore[2].transZ));
    rock1_mat = glm::rotate(rock1_mat, glm::radians(-30.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock1_mat = glm::scale(rock1_mat, glm::vec3(1.f, 3.f, 1.f));
    rock1_mat = glm::rotate(rock1_mat, glm::radians(34.f), glm::vec3(1.0f, .0f, 0.0f));
    rock1_mat = glm::rotate(rock1_mat, glm::radians(25.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock1_mat = glm::translate(rock1_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock1_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0);
    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 rock2_mat = glm::mat4(1.0f);
    rock2_mat = glm::translate(rock2_mat, glm::vec3(ore[0].transX, ore[0].transY, ore[0].transZ));
    rock2_mat = glm::scale(rock2_mat, glm::vec3(1.f, 5.f, 1.f));
    rock2_mat = glm::translate(rock2_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock2_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0);
    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock2_mat = glm::mat4(1.0f);
    rock2_mat = glm::translate(rock2_mat, glm::vec3(ore[0].transX, ore[0].transY, ore[0].transZ));
    rock2_mat = glm::rotate(rock2_mat, glm::radians(15.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock2_mat = glm::scale(rock2_mat, glm::vec3(1.f, 3.f, 1.f));
    rock2_mat = glm::translate(rock2_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock2_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0);
    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock2_mat = glm::mat4(1.0f);
    rock2_mat = glm::translate(rock2_mat, glm::vec3(ore[0].transX, ore[0].transY, ore[0].transZ));
    rock2_mat = glm::rotate(rock2_mat, glm::radians(-30.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock2_mat = glm::scale(rock2_mat, glm::vec3(1.f, 3.f, 1.f));
    rock2_mat = glm::translate(rock2_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock2_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0);
    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

GLvoid Gemstone::Reset()
{
    delete[] ore;
    ore = NULL;
}

GLvoid Gemstone::UpdateBoundingBox()
{
    for (int i = 0;i < 3;++i) {
        ore[i].m_bbBox.m_fMaxX = ore[i].transX + 0.1f;
        ore[i].m_bbBox.m_fMinX = ore[i].transX - 0.1f;
        ore[i].m_bbBox.m_fMaxY = ore[i].transY + 0.1f;
        ore[i].m_bbBox.m_fMinY = ore[i].transY - 0.1f;
        ore[i].m_bbBox.m_fMaxZ = ore[i].transZ + 0.1f;
        ore[i].m_bbBox.m_fMinZ = ore[i].transZ - 0.1f;
    }
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

GLvoid Bomb::Bomb_Mat(GameShader* gs)
{
    SetPosition();
    glm::mat4 bomb_mat = glm::mat4(1.0f);
    glm::mat4 line_mat = glm::mat4(1.0f);
    for (int i = 0; i < 50; ++i) {
        if (bomb[i].transY > bomb[i].MaxTransY) {
            mp_IsUp[i] = false;
        }
        else if (bomb[i].transY < bomb[i].MaxTransY / 2.) {
            mp_IsUp[i] = true;
        }
        if (mp_IsUp[i]) bomb[i].transY += bomb[i].transY_aoc;
        else bomb[i].transY = bomb[i].transY - bomb[i].transY_aoc;
        bomb_mat = glm::mat4(1.0f);
        bomb_mat = glm::translate(bomb_mat, glm::vec3(bomb[i].transX, bomb[i].transY, bomb[i].transZ));
        bomb_mat = glm::scale(bomb_mat, glm::vec3(6.0f, 6.0f, 6.0f));
        unsigned int StransformLocation = glGetUniformLocation(gs->s_program, "transform");
        glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(bomb_mat));
        gs->qobj = gluNewQuadric();
        gluQuadricDrawStyle(gs->qobj, gs->obj_type);
        int objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
        unsigned isCheck = glGetUniformLocation(gs->s_program, "isCheck");
        glUniform1f(isCheck, false);
        glUniform4f(objColorLocation, 0.1f, 0.1f, 0.1f, 1.0);
        gluSphere(gs->qobj, 0.1f, 24, 24);
        for (int j = 0; j < bomb[i].transY * 4; ++j) {
            GLfloat a{};
            if (j & 1) {
                a = 90.f;
            }
            bomb_mat = glm::mat4(1.0f);

            bomb_mat = glm::translate(bomb_mat, glm::vec3(bomb[i].transX, bomb[i].transY - j * 0.3f, bomb[i].transZ));
            bomb_mat = glm::rotate(bomb_mat, glm::radians(a), glm::vec3(0.0f, 1.0f, 0.0f));
            bomb_mat = glm::scale(bomb_mat, glm::vec3(0.5f, 5.0f, 0.5f));
            StransformLocation = glGetUniformLocation(gs->s_program, "transform");
            glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(bomb_mat));
            gs->qobj = gluNewQuadric();
            gluQuadricDrawStyle(gs->qobj, gs->obj_type);
            objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
            isCheck = glGetUniformLocation(gs->s_program, "isCheck");
            glUniform1f(isCheck, false);
            glUniform4f(objColorLocation, 0.1f, 0.1f, 0.1f, 1.0);
            gluCylinder(gs->qobj, 0.05f, 0.05f, 0.1f, 8, 2);
        }
    }
}

GLvoid Bomb::UpdateBoundingBox()
{
    for (int i = 0;i < 50;++i) {
        bomb[i].m_bbBox.m_fMaxX = bomb[i].transX + 0.1f;
        bomb[i].m_bbBox.m_fMinX = bomb[i].transX - 0.1f;
        bomb[i].m_bbBox.m_fMaxY = bomb[i].transY + 0.1f;
        bomb[i].m_bbBox.m_fMinY = bomb[i].transY - 0.1f;
        bomb[i].m_bbBox.m_fMaxZ = bomb[i].transZ + 0.1f;
        bomb[i].m_bbBox.m_fMinZ = bomb[i].transZ - 0.1f;
    }
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

GLvoid Bubble::Bubble_Mat(GameShader* gs)
{
    Bubbles_Setting();
    glm::mat4 bubble_mat = glm::mat4(1.0f);
    for (int bc = 0; bc < 100; ++bc) {
        for (int b = 0; b < 3; ++b) {
            bubble_mat = glm::mat4(1.0f);
            bubble_mat = glm::translate(bubble_mat, glm::vec3(mpp_Bubble[bc][b].transX + mpp_Bubble[bc][b].transX_aoc, mpp_Bubble[bc][b].transY, mpp_Bubble[bc][b].transZ));

            bubble_mat = glm::scale(bubble_mat, glm::vec3(mpp_Bubble[bc][b].scaleX, mpp_Bubble[bc][b].scaleY, mpp_Bubble[bc][b].scaleZ));
            bubble_mat = glm::translate(bubble_mat, glm::vec3(0.0f, 0.1f, 0.0f));
            unsigned int StransformLocation = glGetUniformLocation(gs->s_program, "transform");
            glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(bubble_mat));
            gs->qobj = gluNewQuadric();
            gluQuadricDrawStyle(gs->qobj, gs->obj_type);
            int objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
            unsigned isCheck = glGetUniformLocation(gs->s_program, "isCheck");
            glUniform1f(isCheck, false);
            glUniform4f(objColorLocation, mpp_Bubble[bc][b].color_r, mpp_Bubble[bc][b].color_g, mpp_Bubble[bc][b].color_b, 1.0);
            gluSphere(gs->qobj, 0.1, 24, 24);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GamePlayer::GamePlayer()
{
    subMarine = new GameObject;
    subMarine->transX = 10.f;
    subMarine->transY = 1.f;
    subMarine->transZ = 10.f;
}

GamePlayer::~GamePlayer()
{
}

GLvoid GamePlayer::subMarine_Catch()
{
}

GLvoid GamePlayer::Draw_SubMarine(GameShader* gs)
{

    glm::mat4 Body = glm::mat4(1.0f);
    Body = glm::translate(Body, glm::vec3(subMarine->transX, subMarine->transY, subMarine->transZ));
    Body = glm::translate(Body, glm::vec3(0.f, 0.f, 0.0f));
    Body = glm::rotate(Body, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
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
    Head = glm::translate(Head, glm::vec3(subMarine->transX, subMarine->transY, subMarine->transZ));
    Head = glm::translate(Head, glm::vec3(0.f, 0.f, 0.0f));
    Head = glm::rotate(Head, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
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
    Tail = glm::translate(Tail, glm::vec3(subMarine->transX, subMarine->transY, subMarine->transZ));
    Tail = glm::rotate(Tail, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
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
    Wing = glm::translate(Wing, glm::vec3(subMarine->transX, subMarine->transY, subMarine->transZ));
    Wing = glm::rotate(Wing, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
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
    Wing = glm::translate(Wing, glm::vec3(subMarine->transX, subMarine->transY, subMarine->transZ));
    Wing = glm::rotate(Wing, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
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
    Grab_Arm_Body = glm::translate(Grab_Arm_Body, glm::vec3(subMarine->transX, subMarine->transY, subMarine->transZ));
    Grab_Arm_Body = glm::rotate(Grab_Arm_Body, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
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
    Grab_Arm = glm::translate(Grab_Arm, glm::vec3(subMarine->transX, subMarine->transY, subMarine->transZ));
    Grab_Arm = glm::translate(Grab_Arm, glm::vec3(0.f, -0.2f, 0.05f));
    Grab_Arm = glm::rotate(Grab_Arm, glm::radians(arm_rot), glm::vec3(1.0f, 0.0f, 0.0f));
    Grab_Arm = glm::translate(Grab_Arm, glm::vec3(0.f, 0.2f, -0.05f));
    Grab_Arm = glm::rotate(Grab_Arm, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
    Grab_Arm = glm::rotate(Grab_Arm, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
    Grab_Arm = glm::translate(Grab_Arm, glm::vec3(0.f, -0.2f, 0.05f));
    Grab_Arm = glm::scale(Grab_Arm, glm::vec3(0.03f, 0.03f, 0.1f + subMarine->scaleZ));
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
    Grab_Arm_Finger1 = glm::translate(Grab_Arm_Finger1, glm::vec3(subMarine->transX, subMarine->transY, subMarine->transZ));
    Grab_Arm_Finger1 = glm::rotate(Grab_Arm_Finger1, glm::radians(arm_rot), glm::vec3(1.0f, 0.0f, 0.0f));
    Grab_Arm_Finger1 = glm::rotate(Grab_Arm_Finger1, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
    Grab_Arm_Finger1 = glm::translate(Grab_Arm_Finger1, glm::vec3(0.f, -0.2f, -0.25f + subMarine->transZ_aoc));
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
    Grab_Arm_Finger2 = glm::translate(Grab_Arm_Finger2, glm::vec3(subMarine->transX, subMarine->transY, subMarine->transZ));
    Grab_Arm_Finger2 = glm::rotate(Grab_Arm_Finger2, glm::radians(arm_rot), glm::vec3(1.0f, 0.0f, 0.0f));
    Grab_Arm_Finger2 = glm::rotate(Grab_Arm_Finger2, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
    Grab_Arm_Finger2 = glm::translate(Grab_Arm_Finger2, glm::vec3(0.f, -0.2f, -0.25f + subMarine->transZ_aoc));
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
    Grab_Arm_Finger3 = glm::translate(Grab_Arm_Finger3, glm::vec3(subMarine->transX, subMarine->transY, subMarine->transZ));
    Grab_Arm_Finger3 = glm::rotate(Grab_Arm_Finger3, glm::radians(arm_rot), glm::vec3(1.0f, 0.0f, 0.0f));
    Grab_Arm_Finger3 = glm::rotate(Grab_Arm_Finger3, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
    Grab_Arm_Finger3 = glm::translate(Grab_Arm_Finger3, glm::vec3(0.f, -0.2f, -0.25f + subMarine->transZ_aoc));
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
    Grab_Arm_Finger4 = glm::translate(Grab_Arm_Finger4, glm::vec3(subMarine->transX, subMarine->transY, subMarine->transZ));
    Grab_Arm_Finger4 = glm::rotate(Grab_Arm_Finger4, glm::radians(arm_rot), glm::vec3(1.0f, 0.0f, 0.0f));
    Grab_Arm_Finger4 = glm::rotate(Grab_Arm_Finger4, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
    Grab_Arm_Finger4 = glm::translate(Grab_Arm_Finger4, glm::vec3(0.f, -0.2f, -0.25f + subMarine->transZ_aoc));
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

GLvoid GamePlayer::Draw_Map(GameShader* gs)
{
    glViewport(1050, 550, 150, 150);
    glm::vec3 cameraPos = glm::vec3(subMarine->transX, 5.0f, subMarine->transZ);         //위치
    glm::vec3 cameraDirection = glm::vec3(subMarine->transX, 0.0f, subMarine->transZ);   //바라보는 방향
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

GLvoid GamePlayer::Draw_Camera(GameShader* gs)
{
    glViewport(0, 0, width, height);
    glm::vec3 cameraPos = glm::vec3(subMarine->transX, subMarine->transY + 1.5f, subMarine->transZ); //--- 카메라 위치
    glm::vec3 cameraDirection = glm::vec3(subMarine->transX, 0.3f, subMarine->transZ - 5); //--- 카메라 바라보는 방향
    glm::vec3 cameraUp = glm::vec3(0.0f, 4.0f, 0.0f); //--- 카메라 위쪽 방향
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    view = glm::translate(view, glm::vec3(subMarine->transX, subMarine->transY + 1.0f, subMarine->transZ));
    view = glm::rotate(view, glm::radians(CO.camera_drgree_y), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(CO.camera_drgree_x), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(-subMarine->transX, -subMarine->transY - 1.0f, -subMarine->transZ));

    unsigned int viewLocation = glGetUniformLocation(gs->s_program, "view"); //--- 뷰잉 변환 설정
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
    projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0));
    unsigned int projectionLocation = glGetUniformLocation(gs->s_program, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}

