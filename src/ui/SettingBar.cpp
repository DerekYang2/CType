#include "SettingBar.h"

/**
 * store popuphandler
 * if toggleGroup PRESSED = custom, activate popuphandler
 * if toggle group selected = custom, return popuphandler input value
*/

SettingBar::SettingBar(float center_x, float center_y, vector<Toggle*> toggle_list, ToggleGroup* toggleGroup, PopupHandler* popupHandler) : cx(center_x), cy(center_y), toggle_group(toggleGroup), popup_handler(popupHandler)
{
    h = toggle_group->get_height();
    space_width = toggleGroup->space_width();
    for (auto t : toggle_list)
        toggle_map[trim(t->get_text())] = t;

    tot_width = 0;
    for (auto t : toggle_list)
    {
        tot_width += t->get_width() + space_width;
    }
    separator_x = tot_width + space_width;
    tot_width += 2 * space_width + space_width * 0.3f;  // separator between toggles and group (add separator width)
    tot_width += toggle_group->get_width();  // last space is auto handled by toggle group
    tot_width += space_width;  // Padding at the very end
    
    // fix toggle positions 
    float x_pos = cx - tot_width / 2;
    separator_x += x_pos;
    // Set positions
    for (auto toggle : toggle_list)
    {
        x_pos += space_width;
        toggle->set_pos(x_pos, cy - h / 2);
        x_pos += toggle->get_width();
    }
    x_pos += 2*space_width + space_width * 0.3f;
    toggle_group->set_pos(x_pos, cy - h / 2);
}

void SettingBar::update()
{
    for (auto& [label, toggle] : toggle_map)
        toggle->update();
    toggle_group->update();
    popup_handler->update();
    if (toggle_group->was_pressed() && toggle_group->get_selected() == "custom")  // if custom pressed
    {
        popup_handler->set_active();
        // need to switch scene to popup
        switch_popup();
    }
}

void SettingBar::draw()
{
    float padding = space_width/2;  // also equal to y padding
    DrawRectangleRoundedAlign(cx, cy, tot_width, h + 2 * padding, 0.3f, 4, theme.sub_alt, CENTER, CENTER); 
    DrawRectangleAlign(separator_x, cy, space_width * 0.3f, h + padding * 0.8f, theme.background, LEFT, CENTER);
    for (auto& [label, toggle] : toggle_map)
        toggle->draw();
    toggle_group->draw();
}

bool SettingBar::needs_update()
{
    bool recent_update = false;  // Does not include the toggle group (these toggles such as time do not require a test refresh)
    for (auto& [label, toggle] : toggle_map)
        recent_update |= toggle->was_pressed();
    return recent_update;
}

bool SettingBar::is_toggled(string str)
{
    auto it = toggle_map.find(str);
    if (it == toggle_map.end())
        return false;
    return it->second->toggled();
}

string SettingBar::custom_input()
{
    return popup_handler->is_numeric() ? popup_handler->input_number() : popup_handler->input_text();
}

string SettingBar::group_selected()
{
    string ret = toggle_group->get_selected();
    if (ret == "custom")
    {
        ret = custom_input();
    }
    return ret;
}

