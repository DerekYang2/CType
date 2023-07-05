#include "InputBox.h"

/**
 * TODO: add double click = highlight token (delimiter = ' ')
*/

InputBox::InputBox(float x, float y, float width, float height, string default_text, bool numeric) : default_text(default_text), text(default_text), numeric(numeric)
{
    const float padding = max(width, height) * 0.05f * 0.5f;
    rect = Rectangle(x, y, width, height);
    outer_rect = Rectangle(x - padding, y - padding, width + 2 * padding, height + 2 * padding);
    font_size = MeasureFontSize("|", 1000, rect.height);
    active = false;
    active_frames = 0;
    invalid_frames = 0;
    text_idx = (int)text.size() - 1;
    left_frames = 0;
    right_frames = 0;
    // space between chars
    spacing_x = MeasureTextEx("ab", font_size).x - MeasureTextEx("b", font_size).x - MeasureTextEx("a", font_size).x;
    // selection to none
    select_start = -2;
    select_end = -2;
    selecting = false;
}

void InputBox::set_range(int minv, int maxv)
{
    min_v = minv;
    max_v = maxv;
}

int InputBox::get_index(float pos_x)
{
    // search index (floor) 
    float width_sum = 0;
    int idx = (int)text.size() - 1;  // set to last
    for (int i = 0; i < text_w.size(); i++)
    {
        width_sum += text_w[i];
        if (rect.x + width_sum > pos_x)
        {
            // check whether to take i-1 or i
            if (pos_x >= rect.x + width_sum - 0.5f*text_w[i])  // right half
            {
                idx = i;
            } else
            {
                idx = i - 1;
            }
            break;
        }
    }
    return idx;
}

bool InputBox::update_special(char c)
{
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
    {
        if (c == 'a')  // select all
        {
            selecting = false;
            select_start = -1;
            select_end = (int)text.size() - 1;
            return true;
        } else if (c == 'v') // paste
        {
            const char* clipboard = GetClipboardText();
            if (clipboard == NULL)
                return true;
            string clipboard_str = clipboard;
            for (char c : clipboard_str)
            {
                char ch = ' ';
                if (' ' <= c && c <= '~')
                    ch = c;
            
                if (select_start != select_end)  // current selection, first delete 
                {
                    int select_l = min(select_start, select_end), select_r = max(select_start, select_end);
                    string before = (select_l >= 0) ? substrI(text, 0, select_l) : "";
                    string after = (select_r + 1 < text.size()) ? text.substr(select_r + 1) : "";
                    text = before + after;
                    text_idx = select_l;

                    selecting = false;
                    select_start = select_end = -1;
                }
                if (MeasureTextEx(text + ch, font_size).x <= rect.width)
                {
                    string after_text = (text_idx + 1 < text.size()) ? text.substr(text_idx + 1) : "";
                    text = substrI(text, 0, text_idx) + ch + after_text;
                    text_idx++;
                    active_frames = 30;
                } else
                {
                    invalid_frames = 15;
                }
            }
            return true;
        } else if (c == 'c') // copy
        {
            int left_i = min(select_start, select_end) + 1, right_i = max(select_start, select_end);
            if (left_i <= right_i)
                SetClipboardText(substrI(text, left_i, right_i).c_str());
            else
                SetClipboardText("");
            return true;
        }
    }
    return false;
}

void InputBox::update()
{
    if (invalid_frames > 0) invalid_frames--;
    bool colliding = CheckCollisionPointRec(mouse, rect);
    if (colliding)
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

    // place before active for direct carry through
    if (!active)
    {
        if (colliding && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            active = true;
        }
    }
    if (active)
    {
        if (!colliding && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            active = false;
        if (IsKeyPressed(KEY_ENTER))
            active = false;
        if (!active)
        {
            select_start = select_end = -1;            
            return;
        }
        
        left_frames = IsKeyDown(KEY_LEFT) ? left_frames + 1 : 0;
        right_frames = IsKeyDown(KEY_RIGHT) ? right_frames + 1 : 0;
        
        // HANDlE LEFT OR RIGHT KEY PRESS 
        if ((IsKeyPressed(KEY_LEFT) || left_frames > 30) && text_idx >= 0)
        {
            active_frames = 30;
            text_idx--;
        }

        if ((IsKeyPressed(KEY_RIGHT) || right_frames > 30) && text_idx < (int)text.size() - 1)
        {
            active_frames = 30;
            text_idx++;
        }

        // HANDLE CARET CLICK
        // update char measurements
        text_w.clear();
        for (int i = 0; i < text.size(); i++)
            text_w.push_back(MeasureTextEx(string(1, text[i]), font_size).x + spacing_x);

        int cur_idx = get_index(mouse.x);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            active_frames = 30;
            // search index (floor) 
            text_idx = cur_idx;
            select_start = text_idx;
            selecting = true;
        }
        // UPDATE SELECT
        if (selecting)
        {
            select_end = cur_idx;
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                selecting = false;
            }
        }
        // UPDATE ACTIVE FRAMES
        active_frames--;
        if (active_frames < -60) active_frames = -1;  // prevent overflow
    } 
}

void InputBox::draw()
{
    if (active)
    {
        float offset_x = 0;
        if (invalid_frames > 0)
            offset_x = 0.2f * (15 - invalid_frames) * sin(2 * (15 - invalid_frames)) + sin(GetTime() * 90);
        rect.x += offset_x, outer_rect.x += offset_x;
        
        DrawRectangleRounded(outer_rect, 0.3f, 7, theme.background_shade);
        // DRAW OUTLINE
        DrawRectangleRoundedLines(outer_rect, 0.3f, 7, 2, invalid_frames > 0 ? theme.error : theme.main);

        // DRAW SELECTION
        float x_left = rect.x;
        float select_width = 0;
        int select_l = min(select_start, select_end), select_r = max(select_start, select_end);
        for (int i = 0; i <= select_l; i++)
        {
            x_left += MeasureTextEx(string(1, text[i]), font_size).x + spacing_x;
        }
        for (int i = select_l + 1; i <= select_r; i++)
        {
            select_width += MeasureTextEx(string(1, text[i]), font_size).x + spacing_x;
        }

        DrawRectangleAlign(x_left, rect.y + rect.height, select_width, rect.height, theme.main, LEFT, BOTTOM);

        // DRAW TEXT
        x_left = rect.x;
        for (int i = 0; i < text.size(); i++)
        {
            Color text_col = (select_l < i && i <= select_r) ? theme.background : theme.main;
            string char_str = string(1, text[i]);
            DrawTextAlign(char_str, x_left, rect.y + rect.height, font_size, text_col, LEFT, BOTTOM);
            x_left += MeasureTextEx(char_str, font_size).x + spacing_x;
        }
        
        // DRAW CURSOR
        if (select_l == select_r && (active_frames > 0 || (-active_frames/30) & 1))  // if active or Inactive, blink half of the time
        {
            float cursor_x = rect.x + MeasureTextEx(substrI(text, 0, text_idx), font_size).x;
            DrawRectangleRoundedAlign(Rectangle(cursor_x, rect.y + rect.height, 3, rect.height), 0.8f, 7, theme.caret, LEFT, BOTTOM);
        }
        
        rect.x -= offset_x, outer_rect.x -= offset_x;  // unoffset
    } else
    {
        DrawRectangleRounded(outer_rect, 0.3f, 7, theme.background_shade);
        // DRAW TEXT
        DrawTextAlign(text, rect.x, rect.y + rect.height, font_size, theme.main, LEFT, BOTTOM);
    }
}

string InputBox::get_text()
{
    return text;
}

void InputBox::push_char(char c)
{
    if (active)
    {
        if (update_special(c))  // if special key, do not add character
            return;
        
        if (select_start != select_end)  // current selection, first delete 
        {
            int select_l = min(select_start, select_end), select_r = max(select_start, select_end);
            string before = (select_l >= 0) ? substrI(text, 0, select_l) : "";
            string after = (select_r + 1 < text.size()) ? text.substr(select_r + 1) : "";
            text = before + after;
            text_idx = select_l;
            
            selecting = false;
            select_start = select_end = -1;
        }
        if (MeasureTextEx(text + c, font_size).x <= rect.width)
        {
            string after_text = (text_idx + 1 < text.size()) ? text.substr(text_idx + 1) : "";  
            text = substrI(text, 0, text_idx) + c + after_text;
            text_idx++;
            active_frames = 30;
        } else
        {
            invalid_frames = 15;
        }
    }
}

void InputBox::pop_char()
{
    if (active)
    {
        active_frames = 30;
        if (select_start != select_end)  // current selection
        {
            int select_l = min(select_start, select_end), select_r = max(select_start, select_end);
            string before = (select_l >= 0) ? substrI(text, 0, select_l) : "";
            string after = (select_r + 1 < text.size()) ? text.substr(select_r + 1) : "";
            text = before + after;
            text_idx = select_l;
            
            selecting = false;
            select_start = select_end = -1;
        } else if (text_idx >= 0) // text idx can become -1
        {
            string after_text = (text_idx + 1 < text.size()) ? text.substr(text_idx + 1) : "";
            text = substrI(text, 0, text_idx - 1) + after_text;
            text_idx--;
        }
    }
}

void InputBox::set_IOHandler(int sceneId)
{
    input_boxes[sceneId].push_back(this);
}

void InputBox::set_pos(float x, float y)
{
    const float padding = max(rect.width, rect.height) * 0.05f * 0.5f;
    rect = Rectangle(x, y, rect.width, rect.height);
    outer_rect = Rectangle(x - padding, y - padding, rect.width + 2 * padding, rect.height + 2 * padding);
}
