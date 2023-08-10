#pragma once
#include "UIObject.h"
#include "Textbox.h"
#include "InputBox.h"
#include "Button.h"
#include "VerticalToggle.h"

class PopupHandler : public UIObject {
    bool active, return_call;
    int init_id;
    Rectangle rect;
    Textbox* title = nullptr;
    Textbox* description = nullptr;
    InputBox* input_box = nullptr; /*or*/ VerticalToggle* panel = nullptr;
    Button* button = nullptr;

    public:
    // Title, description, input textbox, return button
    PopupHandler(float x, float y, float width, float height, Textbox* title, Textbox* description, InputBox* input_box, Button* button);  // x,y centered by default
    // Title, description, vertical toggle panel, return button
    PopupHandler(float x, float y, float width, float toggle_height, int rows, Textbox* title, Textbox* description, Button* button, vector<string> options, string init_option);
    void update();
    void draw();
    void set_active();
    string input_text();
    string get_selected();
    string input_number();
    bool is_numeric();
};