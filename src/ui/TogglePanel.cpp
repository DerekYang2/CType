#include "TogglePanel.h"

TogglePanel::TogglePanel(float x, float y, float w, vector<ToggleGroup*> toggles, vector<pair<string, string>> toggle_info) : x(x), y(y), width(w), toggles(toggles), toggle_info(toggle_info)
{
    toggle_h = toggles.back()->height();
    offset_y = 0;
    
    float cur_y = y;
    font_size = toggles.back()->get_font_size();
    gap = MeasureTextEx("I", font_size).y;
    int idx = 0;
    for (auto& [label, detail] : toggle_info)
    {
        text_height.push_back({ MeasureTextEx(label, font_size).y, MeasureTextEx(detail, font_size).y });
        float tot_h = text_height.back().first + text_height.back().second ;
        cur_y += gap;
        toggles[idx]->set_pos(x + width - toggles[idx]->width(), cur_y + (tot_h - toggles[idx]->height()) * 0.5f);  // center
        cur_y += tot_h + gap;
        idx++;
    }

}

void TogglePanel::update()
{
    for (int i = 0; i < toggles.size(); i++)
    {
        toggles[i]->set_offset(offset_y);
        toggles[i]->update();
    }
}

void TogglePanel::draw()
{
    float cur_y = y;
    for (int i = 0; i < toggles.size(); i++)
    {
        toggles[i]->draw();
        cur_y += gap;
        DrawTextAlign(toggle_info[i].first, x, cur_y, font_size, theme.main);
        cur_y += text_height[i].first;
        DrawTextAlign(toggle_info[i].second, x, cur_y, font_size, theme.sub);
        cur_y += text_height[i].second;
        cur_y += gap;
    }
}

string TogglePanel::selected(int idx)
{
    return toggles[idx]->get_selected();
}