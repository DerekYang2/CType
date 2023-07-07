#include "UIObject.h"
#include "globals.h"

class Textbox : public UIObject {
    string text;
    float font_size;  // desired font size, will be overriden if wrapping off
    Color color;
    Rectangle rect;
    bool wrapping;
    string fold(string str, float max_width);
    public:
    Textbox(float x, float y, float width, float height, string text_str, float fontSize, Color col, bool wrapping = false);
    Textbox(float x, float y, float width, string text, Color col);   // specific scaling text constructor
    void draw();
    void set_pos(float x, float y) override;
    Rectangle get_rect();
    float get_width();
    float get_height();
};