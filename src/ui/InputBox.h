#pragma once 
#include "globals.h"
#include "UIObject.h"
#include "IOHandler.h"
class InputBox : public UIObject {
    bool active; 
    Rectangle rect;
    int font_size;
    string default_text, text;
    bool numeric;
    int min_v, max_v;
    public:
    InputBox(float x, float y, float width, float height, string default_text, bool numeric);
    void set_range(int minv, int maxv);
    void update() override;
    void draw() override;
    string get_text();
};