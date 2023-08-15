#include "TextPanelV.h"

TextPanelV::TextPanelV(float x, float y, float w, float h) : x(x), y(y), w(w), h(h)
{
}

TextPanelV::TextPanelV(float x, float y, float w, float h, vector<pair<string, int>> labels_list, vector<pair<string, int>> values_list) : x(x), y(y), w(w), h(h)
{
    assert(labels_list.size() == values_list.size());
    label_text.clear(), value_text.clear();
    float tot_height = 0;
    for (auto& [txt, font_size] : labels)
    {
        label_text.push_back(new Textbox(0, 0, w, INT_MAX, txt, font_size, "sub", true));
        tot_height += label_text.back()->get_height();
    }
    for (auto& [txt, font_size] : values)
    {
        value_text.push_back(new Textbox(0, 0, w, INT_MAX, txt, font_size, "main", true));
        tot_height += value_text.back()->get_height();
    }
    gap = (h - tot_height) / (labels.size() - 1);
    float y_pos = y;
    // Set positions
    for (int i = 0; i < labels.size(); i++) {
        label_text[i]->set_pos(x, y_pos);
        y_pos += label_text[i]->get_height();
        value_text[i]->set_pos(x, y_pos);
        y_pos += value_text[i]->get_height() + gap;
    }
}

void TextPanelV::init(vector<pair<string, int>> labels_list, vector<pair<string, int>> values_list)
{
    labels = labels_list, values = values_list;
    assert(labels_list.size() == values_list.size());
    label_text.clear(), value_text.clear();
    float tot_height = 0;
    for (auto& [txt, font_size] : labels)
    {
        label_text.push_back(new Textbox(0, 0, w, INT_MAX, txt, font_size, "sub", true));
        tot_height += label_text.back()->get_height();
    }
    for (auto& [txt, font_size] : values)
    {
        value_text.push_back(new Textbox(0, 0, w, INT_MAX, txt, font_size, "main", true));
        tot_height += value_text.back()->get_height();
    }
    gap = (h - tot_height) / (labels.size() - 1);
    float y_pos = y;
    // Set positions
    for (int i = 0; i < labels.size(); i++) {
        label_text[i]->set_pos(x, y_pos);
        y_pos += label_text[i]->get_height();
        value_text[i]->set_pos(x, y_pos);
        y_pos += value_text[i]->get_height() + gap;
    }
}

void TextPanelV::draw()
{
    for (int i = 0; i < labels.size(); i++)
    {
        label_text[i]->draw();
        value_text[i]->draw();
    }
}