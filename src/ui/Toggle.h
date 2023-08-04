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
    string text = "";
    Texture* texture = nullptr, *texture_off = nullptr;
    float font_size;
    float img_scale, img_scale_off;
    float space_width = 0;  // space between text and icon
    Stopwatch pressWatch;  
// store time of last pressed
    bool on;
    bool pressed; 
    public:
    // Toggle radio button
    Toggle(float x, float y, float w, float h, bool initState);
    // Toggle icon and text
    Toggle(float x, float y, float h, bool initState, string toggle_text = "", string img_path = "");
    // Toggle two icons, scaled into a square
    Toggle(float x, float y, float h, bool initState, Texture *texture_on, Texture *texture_off);

    void set_pos(float x, float y) override;
    void set_x(float x);
    void flip();
    void set_state(bool state);
    void update();
    void draw();
    float get_width();
    float get_height();
    string get_text();
    bool toggled();
    bool was_pressed();
};

