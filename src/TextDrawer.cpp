#include "TextDrawer.h"
#include "globals.h"

TextDrawer::TextDrawer() { }

TextDrawer::TextDrawer(string font_path, float fontSize, float font_spacing)
{
    font = load_font("fonts/" + font_path);
    spacing = font_spacing;
    font_size = fontSize;
    offset = 0;
    // calculating char dimensions
    for (char c = ' '; c <= '~'; c++)
    {
        string str(1, c);
        char_dimension[c] = MeasureTextEx(font, str.c_str(), font_size, font_size / spacing);
    }

    center = gameScreenWidth / 2;
    cursor_h = char_dimension['I'].y;
    bottom_y = (gameScreenHeight + cursor_h) / 2;
    
}


void TextDrawer::set_offset(float x)
{
    offset = x;
}

void TextDrawer::draw()
{
    float left_most = center + offset;
    BeginShaderMode(shader);    // Activate SDF font shader    
    Color future_col = BLACK;
    const float a_offset = 100;
    future_col.a = a_offset;
    const float fall_off = 200;
    Color col;
    for (int i = char_status.size() - 1; i >= 0 && left_most >= 0; i--)
    {
        string char_str(1, char_status[i].second);
        Vector2 dimension = char_dimension[char_status[i].second];
        left_most -= dimension.x;

        if (left_most > center || char_status[i].first == MISSING)
            col = future_col;
        else if (char_status[i].first == CORRECT)
            col = BLACK;
        else if (char_status[i].first == INCORRECT)
            col = RED;
        else  // extra word 
            col = RED, col.a = a_offset;
        if (left_most < fall_off)
            col.a = a_offset * (left_most * left_most / (fall_off * fall_off));
        
        if (char_str != " ")
            DrawTextEx(font, char_str.c_str(), { left_most, bottom_y - dimension.y }, font_size, font_size / spacing, col);
        if (char_status[i].first == MISSING)
        {
            DrawLineEx({ left_most, bottom_y }, { left_most + dimension.x, bottom_y }, 2, RED);
        }
    }
    float right_most = center + offset;
    for (int i = empty_i; i < generated_chars.size() && right_most <= gameScreenWidth + 2 * char_dimension['w'].x; i++)
    {
        string char_str(1, generated_chars[i]);
        Vector2 dimension = char_dimension[generated_chars[i]];
        float dist_edge = gameScreenWidth - (right_most + dimension.x);
        col = future_col;
        if (dist_edge < fall_off)
            col.a = a_offset * (dist_edge * dist_edge / (fall_off * fall_off));
         if (char_str != " ")
            DrawTextEx(font, char_str.c_str(), { right_most, bottom_y - dimension.y }, font_size, font_size / spacing, col);
        right_most += dimension.x;
    }
    EndShaderMode();
}

void TextDrawer::draw_cursor()
{
    DrawRectangleRounded(Rectangle(center, bottom_y - cursor_h, 3, cursor_h), 0.8f, 7, BLACK);
}