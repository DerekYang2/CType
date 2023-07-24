#include "SettingBar.h"

/**
 * store popuphandler
 * if toggleGroup PRESSED = custom, activate popuphandler
 * if toggle group selected = custom, return popuphandler input value
*/

SettingBar::SettingBar(float center_x, float center_y, initializer_list<Toggle*> toggle_list, ToggleGroup* toggleGroup, PopupHandler* popupHandler) : cx(center_x), cy(center_y), toggle_group(toggleGroup), popup_handler(popupHandler)
{
    h = toggle_group->height();
    space_width = toggle_group->space_width();
    for (auto t : toggle_list)
        toggle_map[t->get_text()] = t;

    tot_width = space_width;  // first space
    for (auto t : toggle_list)
    {
        tot_width += t->width();
        tot_width += space_width;
    }
    separator_x = tot_width + 1.5f*space_width; 
    tot_width += 3 * space_width;  // separator between toggles and group
    tot_width += toggle_group->width();  // last space is auto handled by toggle group

    // fix toggle positions 
    float x_pos = cx - tot_width / 2;
    separator_x += x_pos;

    x_pos += space_width;
    for (auto& [label, toggle] : toggle_map)
    {
        toggle->set_pos(x_pos, cy - h / 2);
        x_pos += toggle->width() + space_width;
    }
    x_pos += 3*space_width;
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
    float padding = 2 * space_width;  // also equal to y padding
    float x_pad = padding - space_width; // x already has some padding
    DrawRectangleRoundedAlign(cx, cy, tot_width + 2 * x_pad, h + 2 * padding, 0.2f, h/5, theme.sub_alt, CENTER, CENTER); 
    DrawRectangleAlign(separator_x, cy, space_width * 0.6f, h + padding, theme.background, CENTER, CENTER);
    for (auto& [label, toggle] : toggle_map)
        toggle->draw();
    toggle_group->draw();
}

bool SettingBar::needs_update()
{
    bool recent_update = toggle_group->was_pressed();
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

