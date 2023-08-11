#pragma once
#include "ToggleGroup.h"
#include "Scrollbar.h"
#include "TextGeneration.h"
extern const int THEME_PER_ROW;
/**
 * TODO: Fix when there are two scrollbars in one scene (if ever a case when this happens)
*/
class VerticalToggle : public ToggleGroup {
    Scrollbar * scrollbar;
    float padding;
    float visible_h;
    Rectangle total_hitbox;
    Rectangle total_bounds{ 0, 0, gameScreenWidth, gameScreenHeight };  // Bounds of this vertical toggle
    Vector2 local_init_corner;
    public:
    VerticalToggle(float x, float y, float w, float h, int rows, vector<string> options, string init_option);
    void update() override;
    void draw() override;
    void set_bounds(Rectangle boundRect);
    void set_pos(float x2, float y2) override;
    void set_local_offset(float dx, float dy);
    void set_offset(float dx, float dy) override;
    Rectangle bounding_box() override;
    float get_width() override;
    float get_height() override;
    string get_selected() override;
    bool was_pressed() override;  
};