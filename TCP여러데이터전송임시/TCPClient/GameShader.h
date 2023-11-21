#pragma once

class GameShader
{
public:
    GLUquadricObj* qobj;
    GLuint s_program;
    GLenum obj_type{ GL_FILL };
    GLuint VAO[3], VBO[6];
    GLuint shaderID;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLfloat hexa[108] = {   //����ü
        //����
        -h_vertex, h_vertex, h_vertex,
        h_vertex, h_vertex, h_vertex,
        -h_vertex, h_vertex, -h_vertex,

        h_vertex, h_vertex, h_vertex,
        h_vertex, h_vertex, -h_vertex,
        -h_vertex, h_vertex, -h_vertex,
        //����
        -h_vertex, -h_vertex, h_vertex,
        h_vertex, -h_vertex, h_vertex,
        -h_vertex, h_vertex, h_vertex,

        h_vertex, -h_vertex, h_vertex,
        h_vertex, h_vertex, h_vertex,
        -h_vertex, h_vertex, h_vertex,

        //�Ʒ���
        -h_vertex, -h_vertex, h_vertex,
        -h_vertex, -h_vertex, -h_vertex,
        h_vertex, -h_vertex, -h_vertex,

        -h_vertex, -h_vertex, h_vertex,
        h_vertex, -h_vertex, -h_vertex,
        h_vertex, -h_vertex, h_vertex,

        //�޸�
        h_vertex, -h_vertex, -h_vertex,
        -h_vertex, -h_vertex, -h_vertex,
        h_vertex, h_vertex, -h_vertex,

        -h_vertex, -h_vertex, -h_vertex,
        -h_vertex, h_vertex, -h_vertex,
        h_vertex, h_vertex, -h_vertex,


        //������
        -h_vertex, -h_vertex, -h_vertex,
        -h_vertex, h_vertex, h_vertex,
        -h_vertex, h_vertex, -h_vertex,

        -h_vertex, -h_vertex, -h_vertex,
        -h_vertex, -h_vertex, h_vertex,
        -h_vertex, h_vertex, h_vertex,

        //������
        h_vertex, -h_vertex, h_vertex,
        h_vertex, -h_vertex, -h_vertex,
        h_vertex, h_vertex, h_vertex,

        h_vertex, -h_vertex, -h_vertex,
        h_vertex, h_vertex, -h_vertex,
        h_vertex, h_vertex, h_vertex
    };

    GLchar* filetobuf(const GLchar* file);
    void make_vertexShaders();

    void make_fragmentShader();

    void InitShader();
    GLvoid InitBuffer();

};

