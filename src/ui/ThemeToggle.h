#pragma once
#include "ToggleGroup.h"
#include "Theme.h"
extern const int THEME_PER_ROW;
class ThemeToggle : public ToggleGroup {
    public:
    ThemeToggle(float x, float y, float w, float h, string init_theme);
    void update() override;
    void draw() override;
    Rectangle bounding_box() override;
    float get_width() override;
    float get_height() override;
    string get_selected() override;
};