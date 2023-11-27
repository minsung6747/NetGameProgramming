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
    GLfloat hexa[108] = {   //À°¸éÃ¼
        //À­¸é
        -h_vertex, h_vertex, h_vertex,
        h_vertex, h_vertex, h_vertex,
        -h_vertex, h_vertex, -h_vertex,

        h_vertex, h_vertex, h_vertex,
        h_vertex, h_vertex, -h_vertex,
        -h_vertex, h_vertex, -h_vertex,
        //Á¤¸é
        -h_vertex, -h_vertex, h_vertex,
        h_vertex, -h_vertex, h_vertex,
        -h_vertex, h_vertex, h_vertex,

        h_vertex, -h_vertex, h_vertex,
        h_vertex, h_vertex, h_vertex,
        -h_vertex, h_vertex, h_vertex,

        //¾Æ·§¸é
        -h_vertex, -h_vertex, h_vertex,
        -h_vertex, -h_vertex, -h_vertex,
        h_vertex, -h_vertex, -h_vertex,

        -h_vertex, -h_vertex, h_vertex,
        h_vertex, -h_vertex, -h_vertex,
        h_vertex, -h_vertex, h_vertex,

        //µÞ¸é
        h_vertex, -h_vertex, -h_vertex,
        -h_vertex, -h_vertex, -h_vertex,
        h_vertex, h_vertex, -h_vertex,

        -h_vertex, -h_vertex, -h_vertex,
        -h_vertex, h_vertex, -h_vertex,
        h_vertex, h_vertex, -h_vertex,


        //ÁÂÃø¸é
        -h_vertex, -h_vertex, -h_vertex,
        -h_vertex, h_vertex, h_vertex,
        -h_vertex, h_vertex, -h_vertex,

        -h_vertex, -h_vertex, -h_vertex,
        -h_vertex, -h_vertex, h_vertex,
        -h_vertex, h_vertex, h_vertex,

        //¿ìÃø¸é
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

