#include <set>
#include "FontToggle.h"

const int FONT_PER_ROW = 4;
void init_font(string path)
{
    if (font_path == path) return;
    cout << "Loading font: " << path << endl;
    font_path = path;
    font = load_font(path);  // set global font to f
    font_spacing = 12;  // Default Font 
    font_measure.set_font(font);
}

FontToggle::FontToggle(float x, float y, float w, float h, string init_font) : ToggleGroup(x, y, h, 0, {"anything"}, true)
{
    tot_width = w;
    text.clear();
    
    vector<string> font_paths = directory_files(FONTS_FOLDER, ".ttf");
    // TODO:  add .otf too
    text = deque(font_paths.begin(), font_paths.end());
    sort(text.begin(), text.end());

    // Get display text from filename
    for (string str : text)
    {
        size_t lastSlashPos = str.find_last_of('/');
        if (lastSlashPos != std::string::npos) {
            str.erase(0, lastSlashPos + 1);
        }
        size_t lastDotPos = str.find_last_of('.');
        if (lastDotPos != std::string::npos) {
            str.erase(lastDotPos);
        }
        std::string result = "";
        result += toupper(str[0]);
        // Replace camel case with space
        for (size_t i = 1; i < str.length()-1; i++)
        {
            if (isupper(str[i]) && (islower(str[i - 1]) || islower(str[i + 1])))
                result += ' ';
            result += str[i];
        }
        result.erase(std::remove(result.begin(), result.end(), '-'), result.end());
        result += str.back();
        display_text.push_back(result);
    }
    
    // load fonts
    for (int i = 0; i < text.size(); i++)
    {
#ifdef _WIN32
        std::set<char> req_chars(display_text[i].begin(), display_text[i].end());
        int fontChars[req_chars.size()];
        int idx = 0;
        for (char c : req_chars)
            fontChars[idx++] = (int)c;
        Font f = load_font(text[i], 24, fontChars, req_chars.size());
#elif __linux__
        Font f = load_font(text[i], 24, NULL, 95);  // Need to load full set on linux for some reason
#endif
        // LoadFontEx(path.c_str(), 16, fontChars, req_chars.size());
        fonts.push_back(f);
    }
    
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == init_font) selected = i;
    }
    
    hitbox.clear();
    for (int i = 0; i < text.size(); i++)
        hitbox.push_back({ 0, 0, 0, h });
    
    padding = h / 5;
    float rect_w = (tot_width - (FONT_PER_ROW-1) * padding) / (FONT_PER_ROW);
    for (int i = 0; i < hitbox.size(); i++)
    {
        int col = i%FONT_PER_ROW, row = i/FONT_PER_ROW;
        hitbox[i].x = corner.x + col * (padding + rect_w);
        hitbox[i].y = corner.y + row * h + (row + 1) * padding;
        hitbox[i].width = rect_w;
    }
}

void FontToggle::draw()
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
        float fontSize = MeasureFontSize(fonts[i], display_text[i], INT_MAX, hitbox[i].height * 0.8f);
        Color rect_col, text_col;
        if (hover[i])
        {
            rect_col = theme.text;
            text_col = theme.background;
        } else if (i == selected)
        {
            rect_col = theme.main;
            text_col = theme.background;
        } else
        {
            rect_col = theme.sub_alt;
            text_col = theme.text;
        }
        bool pressed = (i == selected && pressWatch.s() < TOGGLE_DELAY);
        if (pressed) 
            rect_col = theme.sub;  // blink sub when click
        float border = (i == selected || hover[i])? stroke_w : 0;
        DrawRectangleRoundedAlign(hitbox[i].x + hitbox[i].width * 0.5f, hitbox[i].y + hitbox[i].height * 0.5f, hitbox[i].width + 2 * border + 2 * pressed, hitbox[i].height + 2 * border + 2 * pressed, 0.35f, roundedSegments(hitbox[i].height), rect_col, CENTER, CENTER);
        DrawTextAlign(fonts[i], display_text[i], hitbox[i].x + hitbox[i].width * 0.5f, hitbox[i].y + hitbox[i].height * 0.5f, fontSize, text_col, CENTER, CENTER);
    }
}

void FontToggle::update()
{
    ToggleGroup::update();
    if (was_pressed())
    {
        init_font(get_selected());
    }
}

Rectangle FontToggle::bounding_box()
{
    return Rectangle(corner.x, corner.y, tot_width, get_height());
}

string FontToggle::get_selected()
{
    return (text[selected]);
}

float FontToggle::get_width()
{
    return tot_width;
}

float FontToggle::get_height()
{
    float h = hitbox[0].height;
    float padding = h / 5;
    int tot_rows = ceil_div(hitbox.size(), FONT_PER_ROW);
    return tot_rows * h + (tot_rows + 1) * padding;
}