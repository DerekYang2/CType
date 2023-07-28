#pragma once

const extern float HINT_ALPHA;

class UIObject {
public:
    float x, y;
    UIObject();
    virtual ~UIObject();
    virtual void draw();
    virtual void draw_hint();
    virtual void update();
    virtual void set_pos(float x2, float y2);
    virtual void shift(float dx, float dy);
    virtual float get_width();
    virtual float get_height();
};
