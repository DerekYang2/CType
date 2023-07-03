#pragma once 
#include "globals.h"
#include "UIObject.h"

class InputBox : public UIObject {
    bool active;
    Rectangle rect, outer_rect;
    int font_size;
    string default_text, text;
    bool numeric;
    int invalid_frames;  // blocked input, either full box or numeric fail
    int min_v, max_v;
    int active_frames;
    int text_idx;
    int left_frames, right_frames;
    public:
    InputBox(float x, float y, float width, float height, string default_text, bool numeric);
    void set_range(int minv, int maxv);
    void update() override;
    void draw() override;
    string get_text();
    void push_char(char c);
    void pop_char();
    void set_IOHandler(int sceneId);
};

extern vector<InputBox*> input_boxes[SCENE_COUNT];