#include "globals.h"

void DrawTextAlign(char c, float x, float y, float font_size, Color col, int x_align, int y_align)
{
    return DrawTextAlign(string(1, c), x, y, font_size, col, x_align, y_align);
}

void DrawTextAlign(string str, float x, float y, float font_size, Color col, int x_align, int y_align)
{
    Vector2 text_size = MeasureTextEx(font, str.c_str(), font_size, font_size / font_spacing);
    float txtw = text_size.x;
    float txth = text_size.y;

    float x_offset = 0;
    float y_offset = 0;

    // Handle x-alignment
    if (x_align == CENTER_ALIGN)
    {
        x_offset = -txtw / 2;
    }
    else if (x_align == RIGHT_ALIGN)
    {
        x_offset = -txtw;
    }

    // Handle y-alignment
    if (y_align == CENTER_ALIGN)
    {
        y_offset = -txth / 2;
    }
    else if (y_align == BOTTOM_ALIGN)
    {
        y_offset = -txth;
    }

    // Draw the text
    DrawTextEx(font, str.c_str(), { x + x_offset, y + y_offset }, font_size, font_size / font_spacing, col);
}

void DrawText(string font_name, string text, float x, float y, float font_size, Color col)
{
    DrawTextEx(font, text.c_str(), {x, y}, font_size, font_size/font_spacing, col);
}

void DrawTextCenter(string font_name, string text, float x, float y, float font_size, Color col)
{
    Vector2 textSize = MeasureTextEx(font, text.c_str(), font_size, font_size / font_spacing);
    DrawTextEx(font, text.c_str(), {x - textSize.x/2, y - textSize.y/2}, font_size, font_size/font_spacing, col);
}

void DrawLabel(string text, int x, int y, int fontSize, Color label_col, Color text_col)
{
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, fontSize / 10);
    textSize.y /= 1.5;
    float padding = fontSize / 5;
    DrawRectangle(x, y, textSize.x + 2*padding, textSize.y + 2*padding, label_col);
    DrawText(text.c_str(), x + padding, y + padding, fontSize, text_col);
}

void DrawLabel(string font_name, string text, int x, int y, int fontSize, Color label_col, Color text_col, bool rounded)
{
    Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, fontSize / font_spacing);
    float padding = fontSize / 5;
    if (rounded)
        DrawRectangleRounded(Rectangle(x, y, textSize.x + 2 * padding, textSize.y + 2 * padding), 0.15, 5, label_col);
    else
        DrawRectangle(x, y, textSize.x + 2 * padding, textSize.y + 2 * padding, label_col);
    DrawText(font_name, text.c_str(), x + padding, y + padding, fontSize, text_col);
}

// returns the shifted equivalent of keys
char shiftChar(char original)
{
    if (original >= 'a' && original <= 'z')
    {
        // Map lowercase letters
        return original - 'a' + 'A';
    }
    else if (original >= '0' && original <= '9')
    {
        // Map digits
        switch (original)
        {
            case '0':
                return ')';
            case '1':
                return '!';
            case '2':
                return '@';
            case '3':
                return '#';
            case '4':
                return '$';
            case '5':
                return '%';
            case '6':
                return '^';
            case '7':
                return '&';
            case '8':
                return '*';
            case '9':
                return '(';
        }
    }
    else
    {
        // Map other characters
        switch (original)
        {
            case '[':
                return '{';
            case ']':
                return '}';
            case '\\':
                return '|';
            case ';':
                return ':';
            case '\'':
                return '"';
            case ',':
                return '<';
            case '.':
                return '>';
            case '/':
                return '?';
            case '`':
                return '~';
            case '-':
                return '_';
            case '=':
                return '+';
        }
    }
    // Return the original character if no mapping was found
    return original;
}

char convertKey(int key)
{
    if (key >= KEY_A && key <= KEY_Z)  // a-z
        return key + 32;
    if (key == KEY_SPACE || (key >= KEY_COMMA && key <= KEY_NINE) || key == KEY_APOSTROPHE || key == KEY_SEMICOLON || key == KEY_EQUAL || key == KEY_LEFT_BRACKET || key == KEY_BACKSLASH || key == KEY_RIGHT_BRACKET || key == KEY_GRAVE)
        return key;
    return 0;
}

int convertChar(char c)
{
    if (c >= 'a' && c <= 'z') c -= 32; // fix, create char to key function
    return c;
}

void DrawRectangleBoth(int x, int y, int width, int height, Color color, float strokeWidth, Color strokeColor)
{
    DrawRectangle(x, y, width, height, color);
    DrawRectangleLinesEx(Rectangle(x, y, width, height), strokeWidth, strokeColor);
}

Rectangle formatRect(Rectangle r)  // make rectangle width and height positive
{
    Rectangle newR = r;
    if (r.width < 0) newR.x = r.x + r.width, newR.width = -r.width;
    if (r.height < 0) newR.y = r.y + r.height, newR.height = -r.height;
    return newR;
}

vector<string> split_string(string str, char delimiter) {
    vector<string> result;
    stringstream ss(str);
    string item;
    while (getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

string fileText(string file_path)
{
    if (!FileExists(file_path.c_str())) return "";
    char* charText = LoadFileText(file_path.c_str());
    if (charText == NULL) return "";
    string text = string(charText);
    UnloadFileText(charText);
    return text;
}

void writeText(string file_path, string str)
{
    try
    {
        ofstream ofs(file_path);
        if (!ofs.is_open())
        {
            std::cout << "Failed to open file: " << file_path << std::endl;
        }
        ofs << str << std::endl;
        ofs.close();
        std::cout << "Wrote to: " << file_path << std::endl;
    }
    catch(std::ofstream::failure &writeErr) 
    {
        std::cout << writeErr.what() << std::endl;
    }
}


bool starts_with(std::string str, std::string prefix)
{
    if (str.size() < prefix.size()) {
        return false;
    }
    for (int i = 0; i < prefix.size(); ++i) {
        if (str[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

string add_prefix(string name, string prefix)
{
    if (starts_with(name, prefix))
        return name;
    return prefix + name;
}

string remove_prefix(string name, string prefix) {
    if (starts_with(name, prefix))
        return name.substr(prefix.size());
    return name;
}

string base_name(string name)
{
    if (starts_with(name, "sfx_"))
    {
        name += "_";  // add _ to end of name so e.g. sfx_pistol returns pistol, while sfx_pistol_reload returns pistol
        // find second '_'
        for (int i = 4; i < name.length(); i++)
            if (name[i] == '_')
                return substrE(name, 4, i);
    } else
    {
        // failure to pass conditions -> return name, assume name is the base already
        if (name.size() < 6)
            return name;  // must contain something + _0000
        string suffix = name.substr(name.length() - 5);
        if (suffix[0] != '_')
            return name;
        for (int i = 1; i < 5; i++)  // suffix must be all digits
            if (!is_digit(suffix[i]))
                return name;
            // Is animation
        // trim off _0000 (back numbers)
        string prefix = name.substr(0, name.length() - 5);
        // trim off l_ and back_
        prefix = remove_prefix(prefix, "l_");
        prefix = remove_prefix(prefix, "back_");
        return prefix;
    }
    return name;
}
