#include "ToggleGroup.h"

#define TOGGLE_DELAY 0.1

ToggleGroup::ToggleGroup(float x, float y, float h, int init_idx, vector<string> text_list)
{
    corner = { x, y };
    init_message = "";
    selected = init_idx;
    texture = nullptr;
    
    text = deque<string>(text_list.begin(), text_list.end());
    text[0] = " " + text[0];
    for (int i = 0; i < text.size(); i++)
        text[i] = " " + text[i] + " ";
    
    int maxFit = 2;
    font_size = maxFit;
    float padding = h / 15;

    while (true)  //find the max font size that fits
    {
        Vector2 textSize = MeasureTextEx(font, text[0].c_str(), maxFit, maxFit / font_spacing);
        textSize.y /= 1.5;
        if (textSize.y + 2 * padding <= h)
        {
            font_size = maxFit;
        } else
            break;
        maxFit++;
    }

    tot_width = 0;
    float x_pos = x;
    for (string& str : text)
    {
        hitbox.push_back({ x_pos, y, 0, h });
        hitbox.back().width = MeasureTextEx(font, str.c_str(), font_size, font_size / font_spacing).x;
        tot_width += hitbox.back().width;
        x_pos += hitbox.back().width;
    }

    offset_y = 0;
}

ToggleGroup::ToggleGroup(float x, float y, float h, int init_idx, vector<string> text_list, string init_msg, string img_path) 
{
    corner = { x, y };
    init_message = init_msg + " ";
    selected = init_idx;
    texture = &textureOf[img_path];
    img_scale = h / texture->height;
    
    text = deque<string>(text_list.begin(), text_list.end());
    text[0] = " " + text[0];
    for (int i = 0; i < text.size(); i++)
        text[i] = " " + text[i] + " ";
    
    int maxFit = 2;
    font_size = maxFit;
    float padding = h / 15;

    while (true)  //find the max font size that fits
    {
        Vector2 textSize = MeasureTextEx(font, text[0].c_str(), maxFit, maxFit / font_spacing);
        textSize.y /= 1.5;
        if (textSize.y + 2 * padding <= h)
        {
            font_size = maxFit;
        } else
            break;
        maxFit++;
    }

    tot_width = img_scale * texture->width;
    float x_pos = tot_width;
    for (string& str : text)
    {
        hitbox.push_back({ x_pos, y, 0, h });
        hitbox.back().width = MeasureTextEx(font, str.c_str(), font_size, font_size / font_spacing).x;
        tot_width += hitbox.back().width;
        x_pos += hitbox.back().width;
    }

    offset_y = 0;
}

void ToggleGroup::draw()
{
    //DrawRectangle(corner.x, corner.y, tot_width, hitbox[0].height, BLACK);
    float x_pos = corner.x;
    if (texture != nullptr)
    {
        DrawTextureEx(*texture, corner, 0, img_scale, theme.main);
        x_pos += texture->width * img_scale;
    }

    for (int i = 0; i < hitbox.size(); i++)
    {
        Color col = (hover[i] || i == selected) ? theme.main : theme.sub;
        if (i == selected && pressWatch.s() < TOGGLE_DELAY) col = theme.sub;  // blink off when click
        DrawTextAlign(text[i], x_pos, hitbox[i].y + hitbox[i].height * 0.5f, font_size, col, LEFT, CENTER);
        x_pos += hitbox[i].width;
    }
}

void ToggleGroup::update()
{
    pressed = false;
    // just in case collisionrec can result in two hovers
    int hover_idx = -1;
    for (int i = 0; i < hitbox.size(); i++)
    {
        if (CheckCollisionPointRec({mouse.x, mouse.y - offset_y}, hitbox[i])) hover_idx = i;
    }
    for (int i = 0; i < hitbox.size(); i++)
        if (i != hover_idx)
            hover[i] = false;
    if (hover_idx != -1)
        hover[hover_idx] = true;

    for (int i = 0; i < hitbox.size(); i++)
    {
        if (hover[i] && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pressWatch.s() > TOGGLE_DELAY)
        {
            pressWatch.start();
            selected = i;
            pressed = true;
        }
    }

}

void ToggleGroup::set_offset(float y)
{
    offset_y = y;
}

string ToggleGroup::get_selected()
{
    string str = text[selected];
        // Trim leading spaces
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    // Trim trailing spaces
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), str.end());
    return str;
}

float ToggleGroup::width()
{
    return tot_width;
}

float ToggleGroup::height()
{
    return hitbox[0].height;
}

float ToggleGroup::space_width()
{
    return MeasureTextEx(font, " ", font_size, font_size / font_spacing).x;
}

void ToggleGroup::set_pos(float x, float y)
{
    float shift_x = x - corner.x, shift_y = y - corner.y;
    corner.x += shift_x, corner.y += shift_y;
    for (int i = 0; i < hitbox.size(); i++)
        hitbox[i].x += shift_x, hitbox[i].y += shift_y;
    
}

bool ToggleGroup::was_pressed()
{
    return pressed;
}

float ToggleGroup::get_font_size()
{
    return font_size;
}
