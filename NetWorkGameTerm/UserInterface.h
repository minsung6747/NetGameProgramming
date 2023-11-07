#pragma once

class Mouse_Handling {
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

class UserInterface
{
public:
    Mouse_Handling MH;

    GLvoid KeyBoardUp(unsigned char key, int x, int y);
    GLvoid KeyBoard(unsigned char key, int x, int y);
    GLvoid SpecialKeyBoard(int key, int x, int y);
    GLvoid SpecialKeyBoardUp(int key, int x, int y);
};

