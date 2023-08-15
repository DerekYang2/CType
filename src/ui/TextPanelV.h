#pragma once
#include "Utils.h"
#include "UIObject.h"
#include "Textbox.h"
class TextPanelV : public UIObject {
    vector<Textbox*> label_text, value_text;
    vector<pair<string, int>> labels, values;
    vector<int> label_h, value_h;
    float x, y, w, h;
    float gap;
    public:
    TextPanelV(float x, float y, float w, float h);
    TextPanelV(float x, float y, float w, float h, vector<pair<string, int>> labels_list, vector<pair<string, int>> values_list);
    void init(vector<pair<string, int>> labels_list, vector<pair<string, int>> values_list);
    void draw() override;
};