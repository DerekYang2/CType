#pragma once
#include "UIObject.h"
#include "Textbox.h"
#include "InputBox.h"
#include "Button.h"

class PopupHandler : public UIObject {
    bool active, return_call;
    int init_id;
    Rectangle rect;
    Textbox* title;
    Textbox* description;
    InputBox* input_box;
    Button* button;
    public:
    PopupHandler(float x, float y, float width, float height, Textbox* title, Textbox* description, InputBox* input_box, Button* button);  // x,y centered by default
    void update(); 
    void draw();
    void set_active();
    string input_text();
    string input_number();
    bool is_numeric();
};