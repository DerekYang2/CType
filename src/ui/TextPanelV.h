#pragma once
#include "Utils.h"
#include "UIObject.h"
#include "Textbox.h"
class TextPanelV : public UIObject
{
    deque<Textbox> label_text, value_text;
    vector<pair<string, int>> labels, values;
    float x, y, w, h;
    float gap;
    public:
    TextPanelV(float x, float y, float w, float h);
    TextPanelV(float x, float y, float w, float h, vector<pair<string, int>> labels_list, vector<pair<string, int>> values_list);
    TextPanelV(float x, float y, float w, vector<pair<string, int>> labels_list, vector<pair<string, int>> values_list);
    void init(vector<pair<string, int>> labels_list, vector<pair<string, int>> values_list, vector<string> hints = {});
    void update() override;
    void draw() override;
    void draw_hint() override;
    void set_pos(float x2, float y2) override;
    float get_width() override;
    float get_height() override;
};