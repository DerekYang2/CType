#pragma once
#include "SoundEffects.h"
#include "ToggleGroup.h"

extern const int SOUND_PER_ROW;

class SoundToggle : public ToggleGroup {
    vector<Sound> sample;
    public:
    SoundToggle(float x, float y, float w, float h, string init_theme);
    void update() override;
    void draw() override;
    Rectangle bounding_box() override;
    float get_width() override;
    float get_height() override;
    string get_selected() override;
};