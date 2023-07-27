#pragma once
#include "globals.h"
#include "Stopwatch.h"
#include "UIObject.h"
extern const float TOGGLE_DELAY;
class ToggleGroup : public UIObject {
    protected:
    Rectangle bounds{ 0, 0, gameScreenWidth, gameScreenHeight };
    Vector2 corner;
    deque<Rectangle> hitbox;
    deque<string> text;
    deque<bool> hover;
    deque<float> hint_alpha;
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
    bool show_rect;
    public:
    ToggleGroup(float x, float y, float h, int init_idx, vector<string> text_list, bool show_rectangle = false);
    ToggleGroup(float x, float y, float h, int init_idx, vector<string> texture_paths, vector<string> hints, bool centered = false, bool show_rectangle = false);
    ToggleGroup(float x, float y, float h, int init_idx, vector<string> text_list, string init_msg, string img_path, bool show_rectangle = false);
    virtual void draw_hint() override;
    virtual void draw() override;
    virtual void update() override;
    virtual void set_offset(float y);
    virtual string get_selected();
    int selected_index();
    virtual void set_selected(string str);
    virtual Rectangle bounding_box();
    virtual float get_width() override;
    virtual float get_height() override;
    float space_width();
    virtual void set_pos(float x, float y) override;
    void set_bounds(Rectangle rect);
    bool was_pressed();
    float get_font_size();
};