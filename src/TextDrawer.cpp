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
    offset = offset_x = offset_vel = vel_target = 0;
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
    offset = offset_x = offset_vel = vel_target = 0;
    float spacing_x = MeasureTextEx("ab", font_size).x - MeasureTextEx("b", font_size).x - MeasureTextEx("a", font_size).x;
    // calculating char dimensions
    for (char c = ' '; c <= '~'; c++)
    {
        string str(1, c);
        char_dimension[c] = MeasureTextEx(str, font_size);
        char_dimension[c].x += spacing_x;
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
    // update offset
    float wpm = 0.8f * max(80.f, wpm_logger.raw_wpm());
    float secperchar = 1.0 / (wpm * 5 / 60);  // predicted seconds to type a char
    if (offset_x >= 0)
        vel_target = offset_x / max(1.f, (secperchar * 60));  // cannot divide by anything smaller than 1
    else
        vel_target = offset_x / max(1.f, (secperchar * 20));  // backspace is stronger

    constexpr float accel_factor = 0.3f;
    offset_vel += (vel_target - offset_vel) * accel_factor;
    
    offset_x -= offset_vel;
    offset = offset_x;
    
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

void TextDrawer::draw_time(string time_text, string wpm_text, float time_percent, float dash_percent, bool left_align)
{
    float text_width = MeasureTextEx(time_text, font_size).x;
    float clock_r = char_dimension['0'].y * 0.4f;

    float info_h = char_dimension['I'].y;
    float info_x = (left_align ? padding + text_width + 2 * clock_r : center), info_y = top_y - 1.5f * info_h;

    // DRAW TIME
    // TODO: FIX alignment
    
    // DRAW TIME
    DrawTextAlign(time_text, info_x, info_y, font_size, theme.main, RIGHT, CENTER);

    // DRAW TIME CLOCK
    DrawCircleSector(info_x - text_width - clock_r, info_y, clock_r * 0.86f, 180 - time_percent * 360, 180, theme.main);
    DrawRing(info_x - text_width - clock_r, info_y, clock_r, clock_r * 0.85f, theme.main);

    // DRAW DASHBOARD 
    float dash_x = info_x + char_dimension[' '].x + clock_r;  // center x of dashboard
    DrawRing(dash_x, info_y, clock_r, clock_r * 0.85f, 45, 315, theme.main);
    DrawCircle(dash_x, info_y, clock_r * 0.2f, theme.main);
    const float gap_a = 45;  // 2 * gap_a  = dashboard gap
    float angle = (90 + gap_a) + ((450 - gap_a) - (90 + gap_a)) * dash_percent;
    DrawLineEx({ dash_x - clock_r * 0.4f * cosa(angle), info_y - clock_r * 0.4f * sina(angle) }, { dash_x + clock_r * cosa(angle), info_y + clock_r * sina(angle) }, 2, theme.main);
    // DRAW WPM
    DrawTextAlign(wpm_text, dash_x + clock_r, info_y, font_size, theme.main, LEFT, CENTER);
}

void TextDrawer::draw_caret()
{
    DrawRectangleRounded(Rectangle(cursor_pos.x, cursor_pos.y, 3, cursor_h), 1.f, 4, theme.caret);
}

void TextDrawer::add_offset(float x)
{
    offset_x += x;
}

float TextDrawer::get_top_y()
{
    return top_y;
}

