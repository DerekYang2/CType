#pragma once
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
    float height;
    string message = "";
    string hint = "";
    int stroke = 2;

    // For texture and text button (constructor 3)
    float img_scale;
    float space_width;
    float hint_alpha = 0;
    public:
    bool flipped = false;
    Button(float x, float y, float h, Texture *texture_pointer, std::function<void()> f = NULL, string hint = "");
    Button(float x, float y, float w, float h, string text, std::function<void()> f = NULL);
    Button(float x, float y, float h, string text, std::function<void()> f = NULL);
    Button(float x, float y, float h, string text, Texture *texture_pointer, std::function<void()> f = NULL);
    void reset();  // resets hover and watch, used when scene is switched after update and before draw, meaning hover may be outdated for one frame
    void attachDraw(std::function<void(Rectangle)> f);
    void attach_trigger(std::function<void()> f);
    void set_pos(float x, float y) override;
    void set_text(string text);  // Only for constructor 4 for now
    void update();
    void setStroke(int strokeWidth);
    void draw() override;
    void draw_hint() override;
    float get_width();
    float get_height();
};

