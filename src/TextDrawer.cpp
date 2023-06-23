#include "TextDrawer.h"
#include "globals.h"
#define side_padding 200
TextDrawer::TextDrawer() { }

TextDrawer::TextDrawer(string font_path, float fontSize, float font_spacing) 
{
    padding = side_padding;
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
    cursor_pos = { -1, -1 };
    center = gameScreenWidth / 2;
    cursor_h = char_dimension['I'].y;
    bottom_y = (gameScreenHeight + cursor_h) / 2;
    top_y = bottom_y - cursor_h;
    if (!is_tape_mode)
        top_y -= 1.5f * char_dimension['I'].y;

    newlines.push_back(-1);  // before first char
}

TextDrawer::TextDrawer(Font draw_font, float fontSize, float spacing)
{
    padding = side_padding;
    font = draw_font;
    spacing = font_spacing;
    font_size = fontSize;
    offset = 0;
    // calculating char dimensions
    for (char c = ' '; c <= '~'; c++)
    {
        string str(1, c);
        char_dimension[c] = MeasureTextEx(font, str.c_str(), font_size, font_size / spacing);
    }
    cursor_pos = { -1, -1 };
    center = gameScreenWidth / 2;
    cursor_h = char_dimension['I'].y;
    bottom_y = (gameScreenHeight + cursor_h) / 2;
    top_y = bottom_y - cursor_h;
    if (!is_tape_mode)
        top_y -= 1.5f * char_dimension['I'].y;
    
    newlines.push_back(-1);  // before first char
}

void TextDrawer::set_offset(float x)
{
    offset = x;
}

int TextDrawer::next_foldpoint()
{
    // Draw current row
    float x_pos = padding;
    int prev_space = newlines.back();
    for (int i = newlines.back() + 1; i < char_status.size(); i++)
    {
        string char_str(1, char_status[i].second);
        Vector2 dimension = char_dimension[char_status[i].second];
        // check remaining 
        float remain = gameScreenWidth - padding - (x_pos + dimension.x);
        if (remain < 0 && char_str != " ")  // need to fold 
        {
            return prev_space;
        }
        if (char_str == " ") prev_space = i;
        x_pos += dimension.x;
    }

    int extra_i = 1;
    // search in future
    for (int i = empty_i; i < generated_chars.size(); i++)
    {
        string char_str(1, generated_chars[i]);
        Vector2 dimension = char_dimension[generated_chars[i]];
        // check remaining 
        float remain = gameScreenWidth - padding - (x_pos + dimension.x);
        if (remain < 0 && char_str != " ")  // need to fold 
        {
            return prev_space;
        }
        if (char_str == " ") prev_space = char_status.size() - 1 + extra_i;
        x_pos += dimension.x;
        extra_i++;
    }
    assert(false);
}

void TextDrawer::draw()
{
    if (is_tape_mode)
    {
        float left_most = center + offset;
        const float a_offset = 100;
        const float fall_off = padding;
        Color col;
        for (int i = char_status.size() - 1; i >= 0 && left_most >= 0; i--)
        {
            string char_str(1, char_status[i].second);
            Vector2 dimension = char_dimension[char_status[i].second];
            left_most -= dimension.x;

            if (left_most > center || char_status[i].first == MISSING)
                col = theme.sub;
            else if (char_status[i].first == CORRECT)
                col = theme.text;
            else if (char_status[i].first == INCORRECT)
                col = theme.error;
            else  // extra word 
                col = theme.error_extra;
            if (left_most < fall_off)
                col.a = a_offset * (left_most * left_most / (fall_off * fall_off));

            if (char_str != " ")
                DrawTextEx(font, char_str.c_str(), { left_most, bottom_y - dimension.y }, font_size, font_size / spacing, col);
            if (char_status[i].first == MISSING)
            {
                DrawLineEx({ left_most, bottom_y }, { left_most + dimension.x, bottom_y }, 2, theme.error);
            }
        }
        float right_most = center + offset;
        for (int i = empty_i; i < generated_chars.size() && right_most <= gameScreenWidth + 2 * char_dimension['w'].x; i++)
        {
            string char_str(1, generated_chars[i]);
            Vector2 dimension = char_dimension[generated_chars[i]];
            float dist_edge = gameScreenWidth - (right_most + dimension.x);
            col = theme.sub; 
            if (dist_edge < fall_off)
                col.a = a_offset * (dist_edge * dist_edge / (fall_off * fall_off));
            if (char_str != " ")
                DrawTextEx(font, char_str.c_str(), { right_most, bottom_y - dimension.y }, font_size, font_size / spacing, col);
            right_most += dimension.x;
        }
        cursor_pos = cursor_target = { center, bottom_y - cursor_h };

    } else
    {
        // check if past foldpoint (deletions) 
        if (newlines.size() > 1 && (int)char_status.size() - 1 <= newlines.back())  // if not first row, move back up
        {
            newlines.pop_back();
        }
        int foldpoint = next_foldpoint();
        // calculate next foldpoint 
        if ((int)char_status.size() - 1 >= foldpoint)  // char status has reached the foldpoint
        {
            newlines.push_back(foldpoint - 1);  // newline is right before foldpoint
            // calculate new foldpoint
            foldpoint = next_foldpoint();
        }

        float current_bottom = bottom_y - 1.5f * char_dimension['I'].y;
        bool first_row = newlines.size() < 2;
        // draw previous row
        if (!first_row)
        {
            Color col;
            float x_pos = padding;
            for (int i = newlines[newlines.size() - 2] + 1; i <= newlines[newlines.size() - 1]; i++)
            {
                string char_str(1, char_status[i].second);
                Vector2 dimension = char_dimension[char_status[i].second];
                if (char_status[i].first == MISSING)
                    col = theme.sub;
                else if (char_status[i].first == CORRECT)
                    col = theme.text;
                else if (char_status[i].first == INCORRECT)
                    col = theme.error;
                else  // extra word 
                    col = theme.error_extra;

                if (char_str != " ")
                    DrawTextAlign(char_status[i].second, x_pos, current_bottom, font_size, col, LEFT, BOTTOM);
                if (char_status[i].first == MISSING)
                {
                    DrawLineEx({ x_pos, current_bottom }, { x_pos + dimension.x, current_bottom }, 2, theme.error);
                }
                if (!(char_str == " " && i == newlines[newlines.size() - 2] + 1))  // do not draw first space
                    x_pos += dimension.x;
            }
        } 

        // Draw current row
        if (!first_row)
            current_bottom = bottom_y;
        Color col;
        float x_pos = padding;
        for (int i = newlines.back() + 1; i < char_status.size(); i++)
        {
            string char_str(1, char_status[i].second);
            Vector2 dimension = char_dimension[char_status[i].second];

            if (char_status[i].first == MISSING)
                col = theme.sub;
            else if (char_status[i].first == CORRECT)
                col = theme.text;
            else if (char_status[i].first == INCORRECT)
                col = theme.error;
            else  // extra word 
                col = theme.error_extra;

            if (char_str != " ")
                DrawTextAlign(char_status[i].second, x_pos, current_bottom, font_size, col, LEFT, BOTTOM);
            if (char_status[i].first == MISSING)
            {
                DrawLineEx({ x_pos, current_bottom }, { x_pos + dimension.x, current_bottom }, 2, theme.error);
            }
            if (!(char_str == " " && i == newlines.back() + 1))  // do not draw if first space
                x_pos += dimension.x;
        }
        cursor_target = { x_pos, current_bottom - cursor_h };
        float accel_factor = 0.3f;
        if (cursor_pos.x == -1)  // not init yet 
            cursor_pos = cursor_target;
        else
            cursor_pos += accel_factor * (cursor_target - cursor_pos);
            
        int foldpoint_relative = empty_i + foldpoint - ((int)char_status.size());
        // Draw remaining current row
        for (int i = empty_i; i < foldpoint_relative; i++)
        {
            string char_str(1, generated_chars[i]);
            Vector2 dimension = char_dimension[generated_chars[i]];
            if (char_str != " ")
                DrawTextAlign(char_str, x_pos, current_bottom, font_size, theme.sub, LEFT, BOTTOM);
            x_pos += dimension.x;
        }

        // Draw next row 
        // find endpoint 
        int endpoint = -1;
        int prev_space = -1;
        x_pos = padding;
        for (int i = foldpoint_relative; i < generated_chars.size(); i++)
        {
            string char_str(1, generated_chars[i]);
            Vector2 dimension = char_dimension[generated_chars[i]];
            // check remaining 
            float remain = gameScreenWidth - padding - (x_pos + dimension.x);
            if (remain < 0)
            {
                endpoint = prev_space;
                break;
            }
            if (char_str == " ")
                prev_space = i;
            x_pos += dimension.x;
        }

        current_bottom = first_row ? bottom_y : bottom_y + 1.5f * char_dimension['I'].y;
        
        x_pos = padding;
        for (int i = foldpoint_relative; i < endpoint; i++)
        {
            string char_str(1, generated_chars[i]);
            Vector2 dimension = char_dimension[generated_chars[i]];
            // check if in padding range
            if (x_pos + dimension.x > gameScreenWidth - padding)
                break;
            if (char_str != " ")
                DrawTextAlign(char_str, x_pos, current_bottom, font_size, theme.sub, LEFT, BOTTOM);
            if (!(char_str == " " && i == foldpoint_relative))  // do not draw if first space
                x_pos += dimension.x;
        }
        // next future row at the beginning of test
        if (first_row)
        {
            current_bottom = bottom_y + 1.5f * char_dimension['I'].y;
            int start_point = endpoint;  // start point for next row
            // Draw next row 
            // find endpoint 
            endpoint = prev_space = -1;
            x_pos = padding;
            for (int i = start_point; i < generated_chars.size(); i++)
            {
                string char_str(1, generated_chars[i]);
                Vector2 dimension = char_dimension[generated_chars[i]];
                // check remaining 
                float remain = gameScreenWidth - padding - (x_pos + dimension.x);
                if (remain < 0)
                {
                    endpoint = prev_space;
                    break;
                }
                if (char_str == " ")
                    prev_space = i;
                x_pos += dimension.x;
            }
            x_pos = padding;
            for (int i = start_point; i < endpoint; i++)
            {
                string char_str(1, generated_chars[i]);
                Vector2 dimension = char_dimension[generated_chars[i]];
                // check if in padding range
                if (x_pos + dimension.x > gameScreenWidth - padding)
                    break;
                if (char_str != " ")
                    DrawTextAlign(char_str, x_pos, current_bottom, font_size, theme.sub, LEFT, BOTTOM);
                if (!(char_str == " " && i == start_point))  // do not draw if first space
                    x_pos += dimension.x;
            }
        }
    }
}

void TextDrawer::draw_caret()
{
    DrawRectangleRounded(Rectangle(cursor_pos.x, cursor_pos.y, 3, cursor_h), 0.8f, 7, theme.caret);
}

float TextDrawer::get_top_y()
{
    return top_y;
}
