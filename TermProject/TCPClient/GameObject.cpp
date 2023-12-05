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
		ore = new GameObject[4];
		uniform_int_distribution<int> dis_xz{ 0,2000 };
		while (true) {
			bool flag{ false };
			for (int i = 0; i < 4; ++i) {
				ore[i].transX = dis_xz(gen) / 100.;
				ore[i].transZ = dis_xz(gen) / 100.;
			}
			for (int i = 0; i < 4; ++i) {
				if (pow(9, 2) < pow(ore[i].transZ - 10, 2) + pow(ore[i].transX - 10, 2))
					flag = true;
			}
			if (!flag)break;
		}
	}
}

bool bGemStoneColorInit = false;
int iRndColorValue[9] = {}; // GemStone's random color value

GLvoid Gemstone::Ore_Mat(GameShader* gs, float fX[9], float fY[9], float fZ[9])
{
    GemstoneData m_ReceivedData[9];

    for (int i = 0; i < 9; ++i)
    {
        m_ReceivedData[i].fX = fX[i];
        m_ReceivedData[i].fY = fY[i];
        m_ReceivedData[i].fZ = fZ[i];
    }

    // Color Data
    int icoloryamount = 0; // yellow GemStone amount
    int icolorramount = 0; // red GemStone amount
    int icolorbamount = 0; // blue GemStone amount
    
    std::uniform_int_distribution<int> rndcolor(1, 3);

    ////////// GemStone 1 //////////
   
    //SetPosition();
    glm::mat4 rock_mat = glm::mat4(1.0f);
    rock_mat = glm::translate(rock_mat, glm::vec3(m_ReceivedData[0].fX , m_ReceivedData[0].fY, m_ReceivedData[0].fZ));
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
    
    // GemStone Color Setting
    if (!bGemStoneColorInit) {
        while (1) {
            iRndColorValue[0] = rndcolor(gen);
            if ((iRndColorValue[0] == 1) && (icoloryamount < 3)) { icoloryamount++; break; }
            else if ((iRndColorValue[0] == 2) && (icolorramount < 3)) { icolorramount++;  break; }
            else if ((iRndColorValue[0] == 3) && (icolorbamount < 3)) { icolorbamount++;  break; }
        }
    }
    if (iRndColorValue[0] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[0] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[0] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b
    

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock_mat = glm::mat4(1.0f);
    rock_mat = glm::translate(rock_mat, glm::vec3(m_ReceivedData[0].fX, m_ReceivedData[0].fY, m_ReceivedData[0].fZ));
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
    
    // GemStone Color Setting
    if (iRndColorValue[0] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[0] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[0] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock_mat = glm::mat4(1.0f);
    rock_mat = glm::translate(rock_mat, glm::vec3(m_ReceivedData[0].fX, m_ReceivedData[0].fY, m_ReceivedData[0].fZ));
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
    
    // GemStone Color Setting
    if (iRndColorValue[0] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[0] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[0] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ////////// GemStone 2 //////////

    glm::mat4 rock1_mat = glm::mat4(1.0f);
    rock1_mat = glm::translate(rock1_mat, glm::vec3(m_ReceivedData[1].fX, m_ReceivedData[1].fY, m_ReceivedData[1].fZ));
    rock1_mat = glm::scale(rock1_mat, glm::vec3(1.f, 5.f, 1.f));
    rock1_mat = glm::translate(rock1_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock1_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);

    // GemStone Color Setting
    if (!bGemStoneColorInit) {
        while (1) {
            iRndColorValue[1] = rndcolor(gen);
            if ((iRndColorValue[1] == 1) && (icoloryamount < 3)) { icoloryamount++; break; }
            else if ((iRndColorValue[1] == 2) && (icolorramount < 3)) { icolorramount++;  break; }
            else if ((iRndColorValue[1] == 3) && (icolorbamount < 3)) { icolorbamount++;  break; }
        }
    }
    if (iRndColorValue[1] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[1] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[1] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock1_mat = glm::mat4(1.0f);
    rock1_mat = glm::translate(rock1_mat, glm::vec3(m_ReceivedData[1].fX, m_ReceivedData[1].fY, m_ReceivedData[1].fZ));
    rock1_mat = glm::rotate(rock1_mat, glm::radians(15.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock1_mat = glm::scale(rock1_mat, glm::vec3(1.f, 3.f, 1.f));
    rock1_mat = glm::translate(rock1_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock1_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);

    // GemStone Color Setting
    if (iRndColorValue[1] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[1] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[1] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock1_mat = glm::mat4(1.0f);
    rock1_mat = glm::translate(rock1_mat, glm::vec3(m_ReceivedData[1].fX, m_ReceivedData[1].fY, m_ReceivedData[1].fZ));
    rock1_mat = glm::rotate(rock1_mat, glm::radians(-30.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock1_mat = glm::scale(rock1_mat, glm::vec3(1.f, 3.f, 1.f));
    rock1_mat = glm::translate(rock1_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock1_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);

    // GemStone Color Setting
    if (iRndColorValue[1] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[1] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[1] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ////////// GemStone 3 //////////

    glm::mat4 rock2_mat = glm::mat4(1.0f);
    rock2_mat = glm::translate(rock2_mat, glm::vec3(m_ReceivedData[2].fX, m_ReceivedData[2].fY, m_ReceivedData[2].fZ));
    rock2_mat = glm::scale(rock2_mat, glm::vec3(1.f, 5.f, 1.f));
    rock2_mat = glm::translate(rock2_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock2_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
   
    // GemStone Color Setting
    if (!bGemStoneColorInit) {
        while (1) {
            iRndColorValue[2] = rndcolor(gen);
            if ((iRndColorValue[2] == 1) && (icoloryamount < 3)) { icoloryamount++; break; }
            else if ((iRndColorValue[2] == 2) && (icolorramount < 3)) { icolorramount++;  break; }
            else if ((iRndColorValue[2] == 3) && (icolorbamount < 3)) { icolorbamount++;  break; }
        }
    }
    if (iRndColorValue[2] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[2] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[2] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock2_mat = glm::mat4(1.0f);
    rock2_mat = glm::translate(rock2_mat, glm::vec3(m_ReceivedData[2].fX, m_ReceivedData[2].fY, m_ReceivedData[2].fZ));
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
    
    // GemStone Color Setting
    if (iRndColorValue[2] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[2] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[2] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock2_mat = glm::mat4(1.0f);
    rock2_mat = glm::translate(rock2_mat, glm::vec3(m_ReceivedData[2].fX, m_ReceivedData[2].fY, m_ReceivedData[2].fZ));
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
    
    // GemStone Color Setting
    if (iRndColorValue[2] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[2] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[2] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ////////// GemStone 4 //////////

    glm::mat4 rock3_mat = glm::mat4(1.0f);
    rock3_mat = glm::translate(rock3_mat, glm::vec3(m_ReceivedData[3].fX, m_ReceivedData[3].fY, m_ReceivedData[3].fZ));
    rock3_mat = glm::scale(rock3_mat, glm::vec3(1.f, 5.f, 1.f));
    rock3_mat = glm::translate(rock3_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock3_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);

    // GemStone Color Setting
    if (!bGemStoneColorInit) {
        while (1) {
            iRndColorValue[3] = rndcolor(gen);
            if ((iRndColorValue[3] == 1) && (icoloryamount < 3)) { icoloryamount++; break; }
            else if ((iRndColorValue[3] == 2) && (icolorramount < 3)) { icolorramount++;  break; }
            else if ((iRndColorValue[3] == 3) && (icolorbamount < 3)) { icolorbamount++;  break; }
        }
    }
    if (iRndColorValue[3] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[3] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[3] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock3_mat = glm::mat4(1.0f);
    rock3_mat = glm::translate(rock3_mat, glm::vec3(m_ReceivedData[3].fX, m_ReceivedData[3].fY, m_ReceivedData[3].fZ));
    rock3_mat = glm::rotate(rock3_mat, glm::radians(15.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock3_mat = glm::scale(rock3_mat, glm::vec3(1.f, 3.f, 1.f));
    rock3_mat = glm::translate(rock3_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock3_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    
    // GemStone Color Setting
    if (iRndColorValue[3] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[3] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[3] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock3_mat = glm::mat4(1.0f);
    rock3_mat = glm::translate(rock3_mat, glm::vec3(m_ReceivedData[3].fX, m_ReceivedData[3].fY, m_ReceivedData[3].fZ));
    rock3_mat = glm::rotate(rock3_mat, glm::radians(-30.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock3_mat = glm::scale(rock3_mat, glm::vec3(1.f, 3.f, 1.f));
    rock3_mat = glm::translate(rock3_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock3_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);

    // GemStone Color Setting
    if (iRndColorValue[3] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[3] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[3] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ////////// GemStone 5 //////////

    glm::mat4 rock4_mat = glm::mat4(1.0f);
    rock4_mat = glm::translate(rock4_mat, glm::vec3(m_ReceivedData[4].fX, m_ReceivedData[4].fY, m_ReceivedData[4].fZ));
    rock4_mat = glm::scale(rock4_mat, glm::vec3(1.f, 5.f, 1.f));
    rock4_mat = glm::translate(rock4_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock4_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    
    // GemStone Color Setting
    if (!bGemStoneColorInit) {
        while (1) {
            iRndColorValue[4] = rndcolor(gen);
            if ((iRndColorValue[4] == 1) && (icoloryamount < 3)) { icoloryamount++; break; }
            else if ((iRndColorValue[4] == 2) && (icolorramount < 3)) { icolorramount++;  break; }
            else if ((iRndColorValue[4] == 3) && (icolorbamount < 3)) { icolorbamount++;  break; }
        }
    }
    if (iRndColorValue[4] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[4] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[4] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock4_mat = glm::mat4(1.0f);
    rock4_mat = glm::translate(rock4_mat, glm::vec3(m_ReceivedData[4].fX, m_ReceivedData[4].fY, m_ReceivedData[4].fZ));
    rock4_mat = glm::rotate(rock4_mat, glm::radians(15.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock4_mat = glm::scale(rock4_mat, glm::vec3(1.f, 3.f, 1.f));
    rock4_mat = glm::translate(rock4_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock4_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    
    // GemStone Color Setting
    if (iRndColorValue[4] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[4] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[4] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock4_mat = glm::mat4(1.0f);
    rock4_mat = glm::translate(rock4_mat, glm::vec3(m_ReceivedData[4].fX, m_ReceivedData[4].fY, m_ReceivedData[4].fZ));
    rock4_mat = glm::rotate(rock4_mat, glm::radians(-30.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock4_mat = glm::scale(rock4_mat, glm::vec3(1.f, 3.f, 1.f));
    rock4_mat = glm::translate(rock4_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock4_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    
    // GemStone Color Setting
    if (iRndColorValue[4] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[4] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[4] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ////////// GemStone 6 //////////

    glm::mat4 rock5_mat = glm::mat4(1.0f);
    rock5_mat = glm::translate(rock5_mat, glm::vec3(m_ReceivedData[5].fX, m_ReceivedData[5].fY, m_ReceivedData[5].fZ));
    rock5_mat = glm::scale(rock5_mat, glm::vec3(1.f, 5.f, 1.f));
    rock5_mat = glm::translate(rock5_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock5_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    
    // GemStone Color Setting
    if (!bGemStoneColorInit) {
        while (1) {
            iRndColorValue[5] = rndcolor(gen);
            if ((iRndColorValue[5] == 1) && (icoloryamount < 3)) { icoloryamount++; break; }
            else if ((iRndColorValue[5] == 2) && (icolorramount < 3)) { icolorramount++;  break; }
            else if ((iRndColorValue[5] == 3) && (icolorbamount < 3)) { icolorbamount++;  break; }
        }
    }
    if (iRndColorValue[5] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[5] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[5] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock5_mat = glm::mat4(1.0f);
    rock5_mat = glm::translate(rock5_mat, glm::vec3(m_ReceivedData[5].fX, m_ReceivedData[5].fY, m_ReceivedData[5].fZ));
    rock5_mat = glm::rotate(rock5_mat, glm::radians(15.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock5_mat = glm::scale(rock5_mat, glm::vec3(1.f, 3.f, 1.f));
    rock5_mat = glm::translate(rock5_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock5_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    
    // GemStone Color Setting
    if (iRndColorValue[5] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[5] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[5] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock5_mat = glm::mat4(1.0f);
    rock5_mat = glm::translate(rock5_mat, glm::vec3(m_ReceivedData[5].fX, m_ReceivedData[5].fY, m_ReceivedData[5].fZ));
    rock5_mat = glm::rotate(rock5_mat, glm::radians(-30.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock5_mat = glm::scale(rock5_mat, glm::vec3(1.f, 3.f, 1.f));
    rock5_mat = glm::translate(rock5_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock5_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    
    // GemStone Color Setting
    if (iRndColorValue[5] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[5] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[5] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ////////// GemStone 7 //////////

    glm::mat4 rock6_mat = glm::mat4(1.0f);
    rock6_mat = glm::translate(rock6_mat, glm::vec3(m_ReceivedData[6].fX, m_ReceivedData[6].fY, m_ReceivedData[6].fZ));
    rock6_mat = glm::scale(rock6_mat, glm::vec3(1.f, 5.f, 1.f));
    rock6_mat = glm::translate(rock6_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock6_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    
    // GemStone Color Setting
    if (!bGemStoneColorInit) {
        while (1) {
            iRndColorValue[6] = rndcolor(gen);
            if ((iRndColorValue[6] == 1) && (icoloryamount < 3)) { icoloryamount++; break; }
            else if ((iRndColorValue[6] == 2) && (icolorramount < 3)) { icolorramount++;  break; }
            else if ((iRndColorValue[6] == 3) && (icolorbamount < 3)) { icolorbamount++;  break; }
        }
    }
    if (iRndColorValue[6] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[6] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[6] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock6_mat = glm::mat4(1.0f);
    rock6_mat = glm::translate(rock6_mat, glm::vec3(m_ReceivedData[6].fX, m_ReceivedData[6].fY, m_ReceivedData[6].fZ));
    rock6_mat = glm::rotate(rock6_mat, glm::radians(15.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock6_mat = glm::scale(rock6_mat, glm::vec3(1.f, 3.f, 1.f));
    rock6_mat = glm::translate(rock6_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock6_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    
    // GemStone Color Setting
    if (iRndColorValue[6] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[6] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[6] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock6_mat = glm::mat4(1.0f);
    rock6_mat = glm::translate(rock6_mat, glm::vec3(m_ReceivedData[6].fX, m_ReceivedData[6].fY, m_ReceivedData[6].fZ));
    rock6_mat = glm::rotate(rock6_mat, glm::radians(-30.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock6_mat = glm::scale(rock6_mat, glm::vec3(1.f, 3.f, 1.f));
    rock6_mat = glm::translate(rock6_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock6_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);
    
    // GemStone Color Setting
    if (iRndColorValue[6] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[6] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[6] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ////////// GemStone 8 //////////

    glm::mat4 rock7_mat = glm::mat4(1.0f);
    rock7_mat = glm::translate(rock7_mat, glm::vec3(m_ReceivedData[7].fX, m_ReceivedData[7].fY, m_ReceivedData[7].fZ));
    rock7_mat = glm::scale(rock7_mat, glm::vec3(1.f, 5.f, 1.f));
    rock7_mat = glm::translate(rock7_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock7_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);

    // GemStone Color Setting
    if (!bGemStoneColorInit) {
        while (1) {
            iRndColorValue[7] = rndcolor(gen);
            if ((iRndColorValue[7] == 1) && (icoloryamount < 3)) { icoloryamount++; break; }
            else if ((iRndColorValue[7] == 2) && (icolorramount < 3)) { icolorramount++;  break; }
            else if ((iRndColorValue[7] == 3) && (icolorbamount < 3)) { icolorbamount++;  break; }
        }
    }
    if (iRndColorValue[7] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[7] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[7] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock7_mat = glm::mat4(1.0f);
    rock7_mat = glm::translate(rock7_mat, glm::vec3(m_ReceivedData[7].fX, m_ReceivedData[7].fY, m_ReceivedData[7].fZ));
    rock7_mat = glm::rotate(rock7_mat, glm::radians(15.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock7_mat = glm::scale(rock7_mat, glm::vec3(1.f, 3.f, 1.f));
    rock7_mat = glm::translate(rock7_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock7_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);

    // GemStone Color Setting
    if (iRndColorValue[7] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[7] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[7] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock7_mat = glm::mat4(1.0f);
    rock7_mat = glm::translate(rock7_mat, glm::vec3(m_ReceivedData[7].fX, m_ReceivedData[7].fY, m_ReceivedData[7].fZ));
    rock7_mat = glm::rotate(rock7_mat, glm::radians(-30.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock7_mat = glm::scale(rock7_mat, glm::vec3(1.f, 3.f, 1.f));
    rock7_mat = glm::translate(rock7_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock7_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);

    // GemStone Color Setting
    if (iRndColorValue[7] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[7] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[7] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ////////// GemStone 9 //////////

    glm::mat4 rock8_mat = glm::mat4(1.0f);
    rock8_mat = glm::translate(rock8_mat, glm::vec3(m_ReceivedData[8].fX, m_ReceivedData[8].fY, m_ReceivedData[8].fZ));
    rock8_mat = glm::scale(rock8_mat, glm::vec3(1.f, 5.f, 1.f));
    rock8_mat = glm::translate(rock8_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock8_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);

    // GemStone Color Setting
    if (!bGemStoneColorInit) {
        while (1) {
            iRndColorValue[8] = rndcolor(gen);
            if ((iRndColorValue[8] == 1) && (icoloryamount < 3)) { icoloryamount++; break; }
            else if ((iRndColorValue[8] == 2) && (icolorramount < 3)) { icolorramount++;  break; }
            else if ((iRndColorValue[8] == 3) && (icolorbamount < 3)) { icolorbamount++;  break; }
        }
    }
    if (iRndColorValue[8] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[8] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[8] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock8_mat = glm::mat4(1.0f);
    rock8_mat = glm::translate(rock8_mat, glm::vec3(m_ReceivedData[8].fX, m_ReceivedData[8].fY, m_ReceivedData[8].fZ));
    rock8_mat = glm::rotate(rock8_mat, glm::radians(15.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock8_mat = glm::scale(rock8_mat, glm::vec3(1.f, 3.f, 1.f));
    rock8_mat = glm::translate(rock8_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock8_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);

    // GemStone Color Setting
    if (iRndColorValue[8] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[8] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[8] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    rock8_mat = glm::mat4(1.0f);
    rock8_mat = glm::translate(rock8_mat, glm::vec3(m_ReceivedData[8].fX, m_ReceivedData[8].fY, m_ReceivedData[8].fZ));
    rock8_mat = glm::rotate(rock8_mat, glm::radians(-30.f), glm::vec3(0.0f, 0.0f, 1.0f));
    rock8_mat = glm::scale(rock8_mat, glm::vec3(1.f, 3.f, 1.f));
    rock8_mat = glm::translate(rock8_mat, glm::vec3(0.0f, 0.1f, 0.0f));
    StransformLocation = glGetUniformLocation(gs->s_program, "transform");
    glUniformMatrix4fv(StransformLocation, 1, GL_FALSE, glm::value_ptr(rock8_mat));
    gs->qobj = gluNewQuadric();
    gluQuadricDrawStyle(gs->qobj, gs->obj_type);
    objColorLocation = glGetUniformLocation(gs->s_program, "objectColor");
    isCheck = glGetUniformLocation(gs->s_program, "isCheck");
    glUniform1f(isCheck, false);

    // GemStone Color Setting
    if (iRndColorValue[8] == 1) glUniform4f(objColorLocation, 0.8f, 0.7f, 0.0f, 1.0); // y
    else if (iRndColorValue[8] == 2) glUniform4f(objColorLocation, 0.9f, 0.2f, 0.4f, 1.0); // r
    else if (iRndColorValue[8] == 3) glUniform4f(objColorLocation, 0.0f, 0.7f, 0.6f, 1.0); // b

    glBindVertexArray(gs->VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Show GemStone's each color created count
    if (!bGemStoneColorInit) {
        printf("젬스톤 그리기 완료 : (Y : %d개 / R : %d개 / B : %d개)\n", icoloryamount, icolorramount, icolorbamount);

        for (int i = 0; i < 9; i++)
        {
            printf("[%d]번째 GemStone x, z 좌표 : [%.2f, %.2f] (색상 : %d)\n",
                i, m_ReceivedData[i].fX, m_ReceivedData[i].fZ, iRndColorValue[i]);
        }

        bGemStoneColorInit = true;
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
