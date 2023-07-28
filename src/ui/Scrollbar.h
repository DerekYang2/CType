#pragma once
#include "globals.h"
#include "UIObject.h"

/**
 * Vertical only for now
*/
class Scrollbar : public UIObject {
    float offset = 0;
    Rectangle rect;
    float thumb_h, screen_h, total_h;
    public:
    // By default, x is right aligned rather than usual left
    // h = actual height of scrollbar, screen height is the height of the screen, total height is the height of the full scrollable content
    Scrollbar(float x, float y, float w, float h, float screen_h, float total_h);
    void update() override;
    void draw() override;
    void shift(float dx, float dy) override;
    void set_pos(float x2, float y2) override;
    float get_offset();
    Rectangle bounding_box();
    Rectangle thumb_rect();

};