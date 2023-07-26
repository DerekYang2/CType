#pragma once
#include "ToggleGroup.h"
#include "Theme.h"
extern const int THEME_PER_ROW;
class ThemeToggle : public ToggleGroup {
    
    public:
    ThemeToggle(float x, float y, float w, float h, string init_theme);
    void draw() override;
    Rectangle bounding_box() override;
    float width() override;
    float height() override;
};