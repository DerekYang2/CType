#ifndef _BUTTON_H
#define _BUTTON_H
#include "globals.h"
#include "Stopwatch.h"
#include "UIObject.h"
class Button : public UIObject
{
    Rectangle hitbox;
    bool hover;
    Texture* texture;
    std::function<void()> triggerFunc;
    std::function<void(Rectangle)> drawFunc = NULL;
    Stopwatch pressWatch;
    const float delay = 0.15;
    float fontSize;
    string message; //for now
    float msg_width, msg_height;
    int stroke = 2;

public:
    bool flipped = false;
    Button(float x, float y, float w, float h, Texture *texture_pointer, std::function<void()> f = NULL);
    Button(float x, float y, float w, float h, string text, std::function<void()> f = NULL);
    void attachDraw(std::function<void(Rectangle)> f);
    void attach_trigger(std::function<void()> f);
    void set_pos(float x, float y) override;
    void update();
    void setStroke(int strokeWidth);
    void draw() override;
    float get_width();
    float get_height();
};

#endif