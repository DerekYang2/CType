#pragma once
#include "globals.h"
#include "Stopwatch.h"
#include "UIObject.h"
class ToggleGroup : public UIObject {
    Vector2 corner;
    deque<Rectangle> hitbox;
    deque<string> text;
    deque<bool> hover;
    Texture* texture;
    vector<Texture*> textures;
    vector<float> img_scales;
    string init_message;
    float font_size;
    float img_scale;
    float offset_y;
    float tot_width;
    bool pressed;
    int selected;
    Stopwatch pressWatch;
    public:
    ToggleGroup(float x, float y, float h, int init_idx, vector<string> text_list);
    ToggleGroup(float x, float y, float h, int init_idx, vector<string> texture_paths, vector<string> hints, bool centered = false);
    ToggleGroup(float x, float y, float h, int init_idx, vector<string> text_list, string init_msg, string img_path);
    void draw() override;
    void update() override;
    void set_offset(float y);
    string get_selected();
    void set_selected(string str);
    float width();
    float height();
    float space_width();
    void set_pos(float x, float y);
    bool was_pressed();
    float get_font_size();
};