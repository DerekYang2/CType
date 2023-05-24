#ifndef _BUTTON_H
#define _BUTTON_H
#include "globals.h"
#include "Stopwatch.h"
#include "UIObject.h"
class Button : public UIObject
{
    Rectangle hitbox;
    bool hover;
    const std::function<void()> triggerFunc;
    std::function<void(Rectangle)> drawFunc = NULL;
    Stopwatch pressWatch;
    const float delay = 0.15;
    float fontSize;
    string message; //for now
    Color default_color = main_color, hover_color = main_color, click_color = main_color;
    Color stroke_color = rgba(199, 222, 252, 1);
    float msg_width, msg_height;
    int stroke = 2;
    string img_path = "";

public:
    bool flipped = false;
    Button(float x, float y, float w, float h, string img_path, Color tint, std::function<void()> f);
    Button(float x, float y, float w, float h, string text, std::function<void()> f);
    void attachDraw(std::function<void(Rectangle)> f);
    void setPos(float x, float y);
    void update();
    void setColor(Color defaultCol, Color hoverCol, Color clickCol);
    void setStroke(int strokeWidth);
    void draw() override;
};

#endif