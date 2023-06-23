#include "ToggleGroup.h"

#define TOGGLE_DELAY 0.1

ToggleGroup::ToggleGroup(float x, float y, float h, int init_idx, vector<string> text_list, bool show_rectangle) : show_rect(show_rectangle)
{
    corner = { x, y };
    init_message = "";
    selected = init_idx;
    texture = nullptr;
    
    text = deque<string>(text_list.begin(), text_list.end());
    text[0] = text[0];
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
    float x_pos = corner.x;
    for (string& str : text)
    {
        hitbox.push_back({ x_pos, y, 0, h });
        hitbox.back().width = MeasureTextEx(font, str.c_str(), font_size, font_size / font_spacing).x;
        tot_width += hitbox.back().width;
        x_pos += hitbox.back().width;
    }

    offset_y = 0;
}

// all toggle images
ToggleGroup::ToggleGroup(float x, float y, float h, int init_idx, vector<string> texture_paths, vector<string> hints, bool centered, bool show_rectangle) : show_rect(show_rectangle)
{
    tot_width = h * texture_paths.size();
    corner = { x , y };
    if (centered)
        corner.x -= tot_width * 0.5f;
    selected = init_idx;
    float x_pos = corner.x;
    for (string& texture_path : texture_paths)
    {
        textures.push_back(&textureOf[texture_path]);
        img_scales.push_back(h * 0.7f / max(textures.back()->height, textures.back()->width));
        hitbox.push_back({ x_pos, y, h, h });
        x_pos += h;
    }
    // add hints to text array
    text = deque<string>(hints.begin(), hints.end());
    hint_alpha.assign(hints.size(), 0); 
    
    offset_y = 0;
}

// one starting image
ToggleGroup::ToggleGroup(float x, float y, float h, int init_idx, vector<string> text_list, string init_msg, string img_path, bool show_rectangle) : show_rect(show_rectangle)
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
    float x_pos = corner.x + tot_width;
    for (string& str : text)
    {
        hitbox.push_back({ x_pos, y, 0, h });
        hitbox.back().width = MeasureTextEx(font, str.c_str(), font_size, font_size / font_spacing).x;
        tot_width += hitbox.back().width;
        x_pos += hitbox.back().width;
    }

    offset_y = 0;
}

void ToggleGroup::update()
{
    pressed = false;
    // just in case collisionrec can result in two hovers
    int hover_idx = -1;
    for (int i = 0; i < hitbox.size(); i++)
    {
        if (CheckCollisionPointRec({ mouse.x, mouse.y - offset_y }, hitbox[i])) hover_idx = i;
    }
    for (int i = 0; i < hitbox.size(); i++)
        if (i != hover_idx)
            hover[i] = false;
    if (hover_idx != -1)
    {
        hover[hover_idx] = true;
        // set all other hint alpha to 0 
        if (!textures.empty())
        {
            for (int i = 0; i < hint_alpha.size(); i++)
                if (i != hover_idx)
                    hint_alpha[i] = 0.f;
            hint_alpha[hover_idx] = clamp(hint_alpha[hover_idx] + 0.1f, 0.f, 0.93f);  // increase current alpha
        }

        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        //cursor_path = "link_cursor";
    } else if (!textures.empty())   // no hover idx
    {
        for (int i = 0; i < hint_alpha.size(); i++)  // decrease alpha
            hint_alpha[i] = clamp(hint_alpha[i] - 0.1f, 0.f, 0.93f);
    }

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

void ToggleGroup::draw()
{
    if (textures.empty())
    {
        //DrawRectangle(corner.x, corner.y, tot_width, hitbox[0].height, BLACK);
        float x_pos = corner.x;
        if (texture != nullptr)
        {
            DrawTextureEx(*texture, corner, 0, img_scale, theme.main);
            x_pos += texture->width * img_scale;
        }

        const float stroke_w = 2.f;
        for (int i = 0; i < hitbox.size(); i++)
        {
            if (show_rect)
            {
                Color col = (hover[i] || i == selected) ? theme.main : theme.sub;
                if (i == selected && pressWatch.s() < TOGGLE_DELAY)
                    col = theme.sub;  // blink off when click
                DrawRectangleRoundedAlign(x_pos, hitbox[i].y + hitbox[i].height * 0.5f, hitbox[i].width, hitbox[i].height + MeasureTextEx(" ", font_size).x , 0.3f, 3, theme.background_shade, LEFT, CENTER);
                if (i == selected)
                    DrawRectangleRoundedLinesAlign(x_pos, hitbox[i].y + hitbox[i].height * 0.5f, hitbox[i].width, hitbox[i].height + MeasureTextEx(" ", font_size).x , 0.3f, 3, stroke_w, theme.main, LEFT, CENTER);
                DrawTextAlign(text[i], x_pos, hitbox[i].y + hitbox[i].height * 0.5f, font_size, col, LEFT, CENTER);
                x_pos += hitbox[i].width;
            } else
            {
                Color col = (hover[i] || i == selected) ? theme.main : theme.sub;
                if (i == selected && pressWatch.s() < TOGGLE_DELAY) col = theme.sub;  // blink off when click
                DrawTextAlign(text[i], x_pos, hitbox[i].y + hitbox[i].height * 0.5f, font_size, col, LEFT, CENTER);
                x_pos += hitbox[i].width;
            }
        }
    } else
    {
        float img_h = hitbox[0].height;
        for (int i = 0; i < textures.size(); i++)
        {
            Color col = (hover[i] || i == selected) ? theme.main : theme.sub;
            if (i == selected && pressWatch.s() < TOGGLE_DELAY) col = theme.sub;  // blink off when click
            DrawTexturePro(*textures[i], { 0, 0, (float)textures[i]->width, (float)textures[i]->height },
            { corner.x + (i + 0.5f) * img_h, corner.y + 0.5f * img_h, textures[i]->width * img_scales[i], textures[i]->height * img_scales[i] },
            { textures[i]->width * img_scales[i] * 0.5f, textures[i]->height * img_scales[i] * 0.5f }, 0, col);
        }
    }
}

void ToggleGroup::draw_hint()
{
    if (textures.empty()) return;
    float img_w = hitbox[0].width;
    for (int i = 0; i < hint_alpha.size(); i++)
    {
        string hint_text = " " + text[i] + " ";
        Vector2 text_dim = MeasureTextEx(hint_text, 22);
        text_dim.y += 2 * MeasureTextEx(" ", 22).x;
        DrawRectangleRoundedAlign({ corner.x + (i + 0.5f) * img_w, corner.y, text_dim.x, text_dim.y }, 0.25f, 7, rgba(0, 0, 0, hint_alpha[i]), CENTER, BOTTOM);
        DrawTextAlign(hint_text, corner.x + (i + 0.5f) * img_w, corner.y - text_dim.y * 0.5f, 22, rgba(255, 255, 255, hint_alpha[i]), CENTER, CENTER);
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

void ToggleGroup::set_selected(string str)
{
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == str)
        {
            pressWatch.start();
            selected = i;
            pressed = true;
        }
    }
}

Rectangle ToggleGroup::bounding_box()
{
    return Rectangle(corner.x, corner.y, tot_width, hitbox[0].height);
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
