#include "ThemeToggle.h"

const int THEME_PER_ROW = 4;

ThemeToggle::ThemeToggle(float x, float y, float w, float h, string init_theme) : ToggleGroup(x, y, h, 0, {"anything"}, true)
{
    tot_width = w;
    text.clear();
    for (auto& [key, value] : theme_map) text.push_back(key);
    sort(text.begin(), text.end());
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == init_theme) selected = i;
    }
    font_size = MeasureFontSize("A", INT_MAX, h/1.5f);
    
    hitbox.clear();
    for (int i = 0; i < text.size(); i++)
        hitbox.push_back({ 0, 0, 0, h });
    
    float padding = h / 5;
    float rect_w = (tot_width - (THEME_PER_ROW-1) * padding) / (THEME_PER_ROW);
    for (int i = 0; i < hitbox.size(); i++)
    {
        int col = i%THEME_PER_ROW, row = i/THEME_PER_ROW;
        hitbox[i].x = corner.x + col * (padding + rect_w);
        hitbox[i].y = corner.y + row * h + (row + 1) * padding;
        hitbox[i].width = rect_w;
    }
}

void ThemeToggle::draw()
{
    const float stroke_w = 3.f;
    
    vector<int> index_order;
    int hover_idx = -1;
    for (int i = 0; i < hitbox.size(); i++)
    {
        if (i != selected)
            index_order.push_back(i);
        if (hover[i])
            hover_idx = i;
    }

    if (hover_idx != -1)
        index_order.push_back(hover_idx);
    index_order.push_back(selected);

    for (int i : index_order)
    {
        Color rect_col, text_col;
        rect_col = theme_map[text[i]].background;
        text_col = theme_map[text[i]].main;

        bool pressed = (i == selected && pressWatch.s() < TOGGLE_DELAY);
       
        // 6 * border for width so that width is extra elongated (makes it easy to see if selected/hover)
        float border = (i == selected || hover[i]) ? (stroke_w) : 0;
        DrawRectangleRoundedAlign(hitbox[i].x + hitbox[i].width * 0.5f, hitbox[i].y + hitbox[i].height * 0.5f, hitbox[i].width + 8 * border + 2 * pressed, hitbox[i].height + 2 * border + 2 * pressed, 0.4f, 5, rect_col, CENTER, CENTER);
        if (i == selected)
        {
            DrawRectangleRoundedLinesAlign(hitbox[i].x + hitbox[i].width * 0.5f, hitbox[i].y + hitbox[i].height * 0.5f, hitbox[i].width + 8 * border + 2 * pressed, hitbox[i].height + 2 * border + 2 * pressed, 0.4f, 5, stroke_w, text_col, CENTER, CENTER);
            // Draw preview caret
            if ((globalFrame / 30) & 1)
            {
                float cursor_h = MeasureTextEx("|", font_size).y;
                DrawRectangleRounded(Rectangle(hitbox[i].x + 0.5f * (hitbox[i].width + MeasureTextEx(text[i], font_size).x), hitbox[i].y + 0.5f * (hitbox[i].height - cursor_h), 3, cursor_h), 0.8f, 4, theme_map[text[i]].caret);
            }
        }
        if (i == selected || i == hover_idx)
        {
            float right_x = hitbox[i].x + hitbox[i].width + 4 * border + pressed;
            // Draw 3 preview squares
            constexpr float rect_w = 21, rect_pad = rect_w / 3;
            DrawRectangleRoundedAlign(right_x - rect_pad, hitbox[i].y + hitbox[i].height * 0.5f, rect_w, rect_w, 0.4f, 4, theme_map[text[i]].text, RIGHT, CENTER);
            DrawRectangleRoundedAlign(right_x - rect_w - 2*rect_pad, hitbox[i].y + hitbox[i].height * 0.5f, rect_w, rect_w, 0.4f, 4, theme_map[text[i]].sub, RIGHT, CENTER);
            DrawRectangleRoundedAlign(right_x - 2*rect_w - 3*rect_pad, hitbox[i].y + hitbox[i].height * 0.5f, rect_w, rect_w, 0.4f, 4, theme_map[text[i]].main, RIGHT, CENTER);
        }
        DrawTextAlign(text[i], hitbox[i].x + hitbox[i].width * 0.5f, hitbox[i].y + hitbox[i].height * 0.5f, font_size, text_col, CENTER, CENTER);
    }

}

Rectangle ThemeToggle::bounding_box()
{
    return Rectangle(corner.x, corner.y, tot_width, height());
}

float ThemeToggle::width()
{
    return tot_width;
}

float ThemeToggle::height()
{
    float h = hitbox[0].height;
    float padding = h / 5;
    int tot_rows = ceil_div(hitbox.size(), THEME_PER_ROW);
    return tot_rows * h + (tot_rows + 1) * padding;
}