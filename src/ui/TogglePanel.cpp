#include "TogglePanel.h"

TogglePanel::TogglePanel(float x, float y, float w, vector<ToggleGroup*> toggles, vector<pair<string, string>> toggleInfo) : x(x), y(y), width(w), toggles(toggles)
{
    toggle_h = toggles.back()->height();
    offset_y = 0;
    
    float cur_y = y;
    font_size = toggles.back()->get_font_size();
    gap = MeasureTextEx("I", font_size).y;
    int idx = 0;
    
    for (auto& [label, detail] : toggleInfo)
    {
        string fold_detail = fold(detail, width - toggles[idx]->width());
        toggle_info.push_back({ label, fold_detail });
        text_height.push_back({ MeasureTextEx(label, font_size).y, MeasureTextEx(fold_detail, font_size).y });
        float tot_h = text_height.back().first + text_height.back().second;
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
            break;
        }
        if (char_str == " ")
            prev_space = i;
    }
    // final line
    fold_str += substrE(str, start_pos, str.length());
    cout << fold_str << endl;
    return fold_str;
}
