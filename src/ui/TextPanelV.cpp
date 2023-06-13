#include "TextPanelV.h"

TextPanelV::TextPanelV(float x, float y, float w, float h) : x(x), y(y), w(w), h(h)
{
}

TextPanelV::TextPanelV(float x, float y, float w, float h, vector<pair<string, int>> labels_list, vector<pair<string, int>> values_list) : x(x), y(y), w(w), h(h)
{
    labels = labels_list, values = values_list;
    label_h.clear(), value_h.clear();
    float tot_height = 0;
    for (auto& [txt, font_size] : labels)
    {
        label_h.push_back(MeasureTextEx(txt, font_size).y);
        tot_height += label_h.back();
    }
    for (auto& [txt, font_size] : values)
    {
        value_h.push_back(MeasureTextEx(txt, font_size).y);
        tot_height += value_h.back();
    }
    gap = (h - tot_height) / (labels.size() - 1);
}

void TextPanelV::init(vector<pair<string, int>> labels_list, vector<pair<string, int>> values_list)
{
    labels = labels_list, values = values_list;
    label_h.clear(), value_h.clear();
    float tot_height = 0;
    for (auto& [txt, font_size] : labels)
    {
        label_h.push_back(MeasureTextEx(txt, font_size).y);
        tot_height += label_h.back();
    }
    for (auto& [txt, font_size] : values)
    {
        value_h.push_back(MeasureTextEx(txt, font_size).y);
        tot_height += value_h.back();
    }
    gap = (h - tot_height) / (labels.size() - 1);
}

void TextPanelV::draw()
{
    float y_pos = y;
    for (int i = 0; i < labels.size(); i++)
    {
        DrawText(labels[i].first, x, y_pos, labels[i].second, theme.sub);
        y_pos += label_h[i];
        DrawText(values[i].first, x, y_pos, values[i].second, theme.main);
        y_pos += value_h[i];
        y_pos += gap;  // gap between labels and values
    }
}