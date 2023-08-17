#include "TextPanelV.h"

TextPanelV::TextPanelV(float x, float y, float w, float h) : x(x), y(y), w(w), h(h)
{
}

TextPanelV::TextPanelV(float x, float y, float w, float h, vector<pair<string, int>> labels_list, vector<pair<string, int>> values_list) : x(x), y(y), w(w), h(h)
{
    labels = labels_list, values = values_list;
    assert(labels_list.size() == values_list.size());
    label_text.clear(), value_text.clear();
    float tot_height = 0;
    for (int i = 0; i < labels.size(); i++)
    {
        label_text.push_back(Textbox(0, 0, w, INT_MAX, labels_list[i].first, labels_list[i].second, "sub", true));
        tot_height += label_text.back().get_height();
    }
    for (int i = 0; i < values.size(); i++)
    {
        value_text.push_back(Textbox(0, 0, w, INT_MAX, values_list[i].first, values_list[i].second, "main", true));
        tot_height += value_text.back().get_height();
    }
    gap = (h - tot_height) / (labels.size() - 1);
    float y_pos = y;
    // Set positions
    for (int i = 0; i < labels.size(); i++) {
        label_text[i].set_pos(x, y_pos);
        y_pos += label_text[i].get_height();
        value_text[i].set_pos(x, y_pos);
        y_pos += value_text[i].get_height() + gap;
    }
}

TextPanelV::TextPanelV(float x, float y, float w, vector<pair<string, int>> labels_list, vector<pair<string, int>> values_list) : x(x), y(y), w(w)
{
    assert(labels_list.size() == values_list.size());
    labels = labels_list, values = values_list;
    label_text.clear(), value_text.clear();
    float tot_height = 0;
    for (int i = 0; i < labels.size(); i++)
    {
        label_text.push_back(Textbox(0, 0, w, INT_MAX, labels_list[i].first, labels_list[i].second, "sub", true));
        tot_height += label_text.back().get_height();
    }
    for (int i = 0; i < values.size(); i++)
    {
        value_text.push_back(Textbox(0, 0, w, INT_MAX, values_list[i].first, values_list[i].second, "main", true));
        tot_height += value_text.back().get_height();
    }
    gap = font_measure.title_height;
    tot_height += gap * (labels.size() - 1);
    h = tot_height;
    
    float y_pos = y;
    // Set positions
    for (int i = 0; i < labels.size(); i++) {
        label_text[i].set_pos(x, y_pos);
        y_pos += label_text[i].get_height();
        value_text[i].set_pos(x, y_pos);
        y_pos += value_text[i].get_height() + gap;
    }
}

void TextPanelV::init(vector<pair<string, int>> labels_list, vector<pair<string, int>> values_list, vector<string> hints)
{
    labels = labels_list, values = values_list;
    assert(labels_list.size() == values_list.size());
    label_text.clear(), value_text.clear();
    float tot_height = 0;
    for (int i = 0; i < labels.size(); i++)
    {
        label_text.push_back(Textbox(0, 0, w, INT_MAX, labels_list[i].first, labels_list[i].second, "sub", true));
        tot_height += label_text.back().get_height();
    }
    for (int i = 0; i < values.size(); i++)
    {
        value_text.push_back(Textbox(0, 0, w, INT_MAX, values_list[i].first, values_list[i].second, "main", true, hints.empty()?"":hints[i]));
        tot_height += value_text.back().get_height();
    }
    gap = (h - tot_height) / (labels.size() - 1);
    float y_pos = y;
    // Set positions
    for (int i = 0; i < labels.size(); i++) {
        label_text[i].set_pos(x, y_pos);
        y_pos += label_text[i].get_height();
        value_text[i].set_pos(x, y_pos);
        y_pos += value_text[i].get_height() + gap;
    }
}

void TextPanelV::update()
{
    for (auto & textbox : label_text)
    {
        textbox.update();
    }
    for (auto & textbox : value_text)
    {
        textbox.update();
    }
}

void TextPanelV::draw()
{
    for (int i = 0; i < labels.size(); i++)
    {
        label_text[i].draw();
        value_text[i].draw();
    }
}

void TextPanelV::draw_hint()
{
    for (int i = 0; i < labels.size(); i++)
    {
        label_text[i].draw_hint();
        value_text[i].draw_hint();
    }
}

void TextPanelV::set_pos(float x2, float y2)
{
    x = x2, y = y2;
    float y_pos = y;
    // Set positions
    for (int i = 0; i < labels.size(); i++) {
        label_text[i].set_pos(x, y_pos);
        y_pos += label_text[i].get_height();
        value_text[i].set_pos(x, y_pos);
        y_pos += value_text[i].get_height() + gap;
    }
}

float TextPanelV::get_width()
{
    return w;
}

float TextPanelV::get_height()
{
    return h;
}
