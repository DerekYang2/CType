#pragma once 
#include "globals.h"
#include "UIObject.h"

class InputBox : public UIObject {
    bool active;
    Rectangle rect, outer_rect;
    int font_size;
    string default_text, text;
    vector<float> text_w;
    bool numeric;
    int invalid_frames;  // blocked input, either full box or numeric fail
    int min_v, max_v;
    int active_frames;
    int text_idx;
    int left_frames, right_frames;
    float spacing_x;
    // selection points 
    int select_start, select_end;  // select start is exclusive, end is inclusive
    bool selecting;
    int get_index(float pos_x);
    bool update_special(char c);  // returns true if special key press, trl+a, ctrl+v, etc
    void push_char_util(char c);  // no special characters to prevent infinite recursion on ctrl v
    public:
    InputBox(float x, float y, float width, float height, string default_text, bool numeric);
    void set_range(int minv, int maxv);
    void update() override;
    void draw() override;
    void set_pos(float x, float y) override;
    string get_text();
    void push_char(char c);
    void pop_char();
    void set_IOHandler(int sceneId);
};

extern vector<InputBox*> input_boxes[SCENE_COUNT];