#pragma once

#include "globals.h"
#include "Stopwatch.h"
#include "UIObject.h"

/**
 * TODO: add toggle art (different drawing for arrow icon, muted icon, etc)
*/
class Toggle: public UIObject
{
    Rectangle hitbox;
    bool hover;
    float vx, cx, rad;  //cx = circle center
    string text;
    Texture* texture;
    float font_size;
    float img_scale;
    Stopwatch pressWatch;  // store time of last pressed

public:
    bool on;
    Toggle(float x, float y, float w, float h, bool initState);
    Toggle(float x, float y, float h, bool initState, string toggle_text = "", string img_path = "");
    void setPos(float x, float y) override;
    void flip();
    void set_state(bool state);
    void update();
    void draw();
    float width();
    float height();
};

