#include "TogglePanel.h"

TogglePanel::TogglePanel(float x, float y, float w, vector<ToggleGroup*> toggles, vector<pair<string, string>> toggleInfo) : corner_init{x, y}, x(x), y(y), width(w), toggles(toggles)
{
    toggle_h = toggles.back()->get_height();
    
    float cur_y = y;
    font_size = toggles.back()->get_font_size();
    gap = toggle_h * 0.5f;
    int idx = 0;
    
    for (auto& [label, detail] : toggleInfo)
    {
        string fold_detail = fold(detail, width - toggles[idx]->get_width() * 1.1f);
        toggle_info.push_back({ label, fold_detail });
        text_height.push_back({ MeasureTextEx(label, font_size).y, MeasureTextEx(fold_detail, font_size).y });
        float sum_h = text_height.back().first + text_height.back().second;
        cur_y += gap;
        toggles[idx]->set_pos(x + width - toggles[idx]->get_width(), cur_y + (sum_h - toggles[idx]->get_height()) * 0.5f);  // right align x, center y
        cur_y += sum_h + gap;
        idx++;
    }

    total_h = cur_y - y;
}

void TogglePanel::update()
{
    for (int i = 0; i < toggles.size(); i++)
    {
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

void TogglePanel::set_pos(float x2, float y2)
{
    x = x2;
    y = y2;
    corner_init = { x, y };

    float cur_y = y;
    for (int i = 0; i < toggles.size(); i++)
    {
        float sum_h = text_height.back().first + text_height.back().second;
        cur_y += gap;
        toggles[i]->set_pos(x + width - toggles[i]->get_width(), cur_y + (sum_h - toggles[i]->get_height()) * 0.5f);  // right align x, center y
        cur_y += sum_h + gap;
    }
}

void TogglePanel::set_offset(float dx, float dy)
{
    x = corner_init.x + dx;
    y = corner_init.y + dy;
    float cur_y = y;
    for (int i = 0; i < toggles.size(); i++)
    {
        float sum_h = text_height.back().first + text_height.back().second;
        cur_y += gap;
        toggles[i]->set_pos(x + width - toggles[i]->get_width(), cur_y + (sum_h - toggles[i]->get_height()) * 0.5f);  // right align x, center y
        cur_y += sum_h + gap;
    }
}

void TogglePanel::set_bounds(Rectangle rect)
{
    bounds = rect;
    for (auto toggle_group : toggles)
    {
        toggle_group->set_bounds(bounds);
    }
}

string TogglePanel::selected(int idx)
{
    return toggles[idx]->get_selected();
}

float TogglePanel::get_height()
{
    return total_h;
}

string TogglePanel::fold(string str, float max_width)
{
    int prev_space = 0;
    int start_pos = 0;
    string fold_str = "";
    for (int i = 0; i < str.length(); i++)
    {
        string char_str(1, str[i]);
        // check remaining 
        float remain = max_width - MeasureTextEx(substrI(str, start_pos, i), font_size).x;
        if (remain < 0)
        {
            fold_str += substrI(str, start_pos, prev_space);
            fold_str += "\n";
            start_pos = prev_space + 1;
            i = start_pos;
        }
        if (char_str == " ")
            prev_space = i;
    }
    // final line
    fold_str += substrE(str, start_pos, str.length());
    return fold_str;
}
