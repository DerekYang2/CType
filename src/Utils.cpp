#include "Utils.h"
// Draw text using Font
// NOTE: chars spacing is NOT proportional to fontSize
void DrawTextEx(Font font, string text, Vector2 position, float fontSize, float spacing, Color tint, float line_spacing)
{
    if (font.texture.id == 0) font = GetFontDefault();  // Security check in case of not valid font

    int size = text.length();
    
    int textOffsetY = 0;            // Offset between lines (on linebreak '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    float scaleFactor = fontSize/font.baseSize;         // Character quad scaling factor

    for (int i = 0; i < size;)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepointNext(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        if (codepoint == '\n')
        {
            // NOTE: Fixed line spacing of 1.5 line-height
            // TODO: Support custom line spacing defined by user
            textOffsetY += (int)(font.baseSize  * line_spacing * scaleFactor);
            textOffsetX = 0.0f;
        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                DrawTextCodepoint(font, codepoint, (Vector2){ position.x + textOffsetX, position.y + textOffsetY }, fontSize, tint);
            }

            if (font.glyphs[index].advanceX == 0) textOffsetX += ((float)font.recs[index].width*scaleFactor + spacing);
            else textOffsetX += ((float)font.glyphs[index].advanceX*scaleFactor + spacing);
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }
}

// Measure string size for Font
Vector2 MeasureTextEx(Font font, string text, float fontSize, float spacing, float line_spacing)
{
    Vector2 textSize = { 0 };

    if ((font.texture.id == 0) || (text.empty())) return textSize;

    int size = text.length();       // Get size in bytes of text
    int tempByteCounter = 0;        // Used to count longer text line num chars
    int byteCounter = 0;

    float textWidth = 0.0f;
    float tempTextWidth = 0.0f;     // Used to count longer text line width

    float textHeight = (float)font.baseSize;
    float scaleFactor = fontSize/(float)font.baseSize;

    int letter = 0;                 // Current character
    int index = 0;                  // Index position in sprite font

    for (int i = 0; i < size;)
    {
        byteCounter++;

        int next = 0;
        letter = GetCodepointNext(&text[i], &next);
        index = GetGlyphIndex(font, letter);

        i += next;

        if (letter != '\n')
        {
            if (font.glyphs[index].advanceX != 0) textWidth += font.glyphs[index].advanceX;
            else textWidth += (font.recs[index].width + font.glyphs[index].offsetX);
        }
        else
        {
            if (tempTextWidth < textWidth) tempTextWidth = textWidth;
            byteCounter = 0;
            textWidth = 0;
            textHeight += ((float)font.baseSize*line_spacing); 
        }

        if (tempByteCounter < byteCounter) tempByteCounter = byteCounter;
    }

    if (tempTextWidth < textWidth) tempTextWidth = textWidth;

    textSize.x = tempTextWidth*scaleFactor + (float)((tempByteCounter - 1)*spacing); // Adds chars spacing to measure
    textSize.y = textHeight*scaleFactor;

    return textSize;
}

void DrawTextAlign(char c, float x, float y, float font_size, Color col, int x_align, int y_align)
{
    return DrawTextAlign(string(1, c), x, y, font_size, col, x_align, y_align);
}

void DrawTextAlign(string str, float x, float y, float font_size, Color col, int x_align, int y_align)
{
    Vector2 text_size = MeasureTextEx(str, font_size);
    float txtw = text_size.x;
    float txth = text_size.y;

    float x_offset = 0;
    float y_offset = 0;

    // Handle x-alignment
    if (x_align == CENTER)
    {
        x_offset = -txtw / 2;
    }
    else if (x_align == RIGHT)
    {
        x_offset = -txtw;
    }

    // Handle y-alignment
    if (y_align == CENTER)
    {
        y_offset = -txth / 2;
    }
    else if (y_align == BOTTOM)
    {
        y_offset = -txth;
    }

    // Draw the text
    DrawTextEx(font, str, { x + x_offset, y + y_offset }, font_size, font_size / font_spacing, col, 1.0f);
}

void DrawText(string text, float x, float y, float font_size, Color col)
{
    DrawTextEx(font, text.c_str(), {x, y}, font_size, font_size/font_spacing, col, 1.0f);
}

void DrawTextCenter(string text, float x, float y, float font_size, Color col)
{
    Vector2 textSize = MeasureTextEx(font, text.c_str(), font_size, font_size / font_spacing);
    DrawText(text, x - textSize.x / 2, y - textSize.y / 2, font_size, col);
}

void DrawLabel(string text, int x, int y, int fontSize, Color label_col, Color text_col)
{
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, fontSize / 10);
    textSize.y /= 1.5;
    float padding = fontSize / 5;
    DrawRectangle(x, y, textSize.x + 2*padding, textSize.y + 2*padding, label_col);
    DrawText(text, x + padding, y + padding, fontSize, text_col);
}

void DrawLabel(string text, int x, int y, int fontSize, Color label_col, Color text_col, bool rounded)
{
    Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, fontSize / font_spacing);
    float padding = fontSize / 5;
    if (rounded)
        DrawRectangleRounded(Rectangle(x, y, textSize.x + 2 * padding, textSize.y + 2 * padding), 0.15, 5, label_col);
    else
        DrawRectangle(x, y, textSize.x + 2 * padding, textSize.y + 2 * padding, label_col);
    DrawTextEx(font, text.c_str(), {x + padding, y + padding}, fontSize, fontSize/font_spacing, text_col);
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

float MeasureFontSize(string text, float width, float height)
{
    float fontSize = 0;
    int maxFit = 2;
    float padding = 0;
    while (true)  //find the max font size that fits
    {
        Vector2 textSize = MeasureTextEx(font, text, maxFit, maxFit / font_spacing);
        if (textSize.x + 2 * padding <= width && textSize.y + 2 * padding <= height)
        {
            fontSize = maxFit;
        } else
            break;
        maxFit++;
    }
    return fontSize;
}

Color hexToColor(string hex_str)
{
    if (hex_str.size() == 3)  // Shorthand hex format 
    {
        hex_str = hex_str.substr(0, 1) + hex_str.substr(0, 1) + hex_str.substr(1, 1) + hex_str.substr(1, 1) + hex_str.substr(2, 1) + hex_str.substr(2, 1);
    }
    size_t hex_int = std::stol(hex_str, nullptr, 16);
    Color rgbColor;
    rgbColor.r = ((hex_int >> 16) & 0xFF);  // Extract the RR byte
    rgbColor.g = ((hex_int >> 8) & 0xFF);   // Extract the GG byte
    rgbColor.b = ((hex_int) & 0xFF);        // Extract the BB byte
    rgbColor.a = 255;
    return rgbColor;
}

float luma(Color color)
{
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b; // per ITU-R BT.709
}

string trim(string str, bool front, bool back)
{
    int strBegin = str.find_first_not_of(" ");
    if (strBegin == std::string::npos)
        return ""; // no content
    if (!front) strBegin = 0;
    int strEnd = str.find_last_not_of(" ");
    if (!back) strEnd = str.size() - 1;
    int strRange = strEnd - strBegin + 1;
    return str.substr(strBegin, strRange);
}

bool replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool IsKeyPressed()
{
    return IsKeyPressed(KEY_APOSTROPHE) ||
           IsKeyPressed(KEY_COMMA) ||
           IsKeyPressed(KEY_MINUS) ||
           IsKeyPressed(KEY_PERIOD) ||
           IsKeyPressed(KEY_SLASH) ||
           IsKeyPressed(KEY_ZERO) ||
           IsKeyPressed(KEY_ONE) ||
           IsKeyPressed(KEY_TWO) ||
           IsKeyPressed(KEY_THREE) ||
           IsKeyPressed(KEY_FOUR) ||
           IsKeyPressed(KEY_FIVE) ||
           IsKeyPressed(KEY_SIX) ||
           IsKeyPressed(KEY_SEVEN) ||
           IsKeyPressed(KEY_EIGHT) ||
           IsKeyPressed(KEY_NINE) ||
           IsKeyPressed(KEY_SEMICOLON) ||
           IsKeyPressed(KEY_EQUAL) ||
           IsKeyPressed(KEY_A) ||
           IsKeyPressed(KEY_B) ||
           IsKeyPressed(KEY_C) ||
           IsKeyPressed(KEY_D) ||
           IsKeyPressed(KEY_E) ||
           IsKeyPressed(KEY_F) ||
           IsKeyPressed(KEY_G) ||
           IsKeyPressed(KEY_H) ||
           IsKeyPressed(KEY_I) ||
           IsKeyPressed(KEY_J) ||
           IsKeyPressed(KEY_K) ||
           IsKeyPressed(KEY_L) ||
           IsKeyPressed(KEY_M) ||
           IsKeyPressed(KEY_N) ||
           IsKeyPressed(KEY_O) ||
           IsKeyPressed(KEY_P) ||
           IsKeyPressed(KEY_Q) ||
           IsKeyPressed(KEY_R) ||
           IsKeyPressed(KEY_S) ||
           IsKeyPressed(KEY_T) ||
           IsKeyPressed(KEY_U) ||
           IsKeyPressed(KEY_V) ||
           IsKeyPressed(KEY_W) ||
           IsKeyPressed(KEY_X) ||
           IsKeyPressed(KEY_Y) ||
           IsKeyPressed(KEY_Z) ||
           IsKeyPressed(KEY_LEFT_BRACKET) ||
           IsKeyPressed(KEY_BACKSLASH) ||
           IsKeyPressed(KEY_RIGHT_BRACKET) ||
           IsKeyPressed(KEY_GRAVE) ||
           IsKeyPressed(KEY_SPACE);
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

Vector2 MeasureTextEx(string str, float font_size)
{
    return MeasureTextEx(font, str, font_size, font_size / font_spacing);
}

void DrawRectangleBoth(int x, int y, int width, int height, Color color, float strokeWidth, Color strokeColor)
{
    DrawRectangle(x, y, width, height, color);
    DrawRectangleLinesEx(Rectangle(x, y, width, height), strokeWidth, strokeColor);
}

void DrawRectangleAlign(Rectangle r, Color col, int x_align, int y_align)
{
    float x_offset = 0;
    float y_offset = 0;

    // Handle x-alignment
    if (x_align == CENTER)
    {
        x_offset = -r.width / 2;
    }
    else if (x_align == RIGHT)
    {
        x_offset = -r.width;
    }

    // Handle y-alignment
    if (y_align == CENTER)
    {
        y_offset = -r.height / 2;
    }
    else if (y_align == BOTTOM)
    {
        y_offset = -r.height;
    }
    DrawRectangle(r.x + x_offset, r.y + y_offset, r.width, r.height, col);
}

void DrawRectangleAlign(float x, float y, float width, float height, Color col, int x_align, int y_align)
{
    float x_offset = 0;
    float y_offset = 0;

    // Handle x-alignment
    if (x_align == CENTER)
    {
        x_offset = -width / 2;
    }
    else if (x_align == RIGHT)
    {
        x_offset = -width;
    }

    // Handle y-alignment
    if (y_align == CENTER)
    {
        y_offset = -height / 2;
    }
    else if (y_align == BOTTOM)
    {
        y_offset = -height;
    }
    DrawRectangle(x + x_offset, y + y_offset, width, height, col);
}

void DrawRectangleRoundedAlign(Rectangle r, float roundness, int segments, Color col, int x_align, int y_align)
{
    float x_offset = 0;
    float y_offset = 0;

    // Handle x-alignment
    if (x_align == CENTER)
    {
        x_offset = -r.width / 2;
    }
    else if (x_align == RIGHT)
    {
        x_offset = -r.width;
    }

    // Handle y-alignment
    if (y_align == CENTER)
    {
        y_offset = -r.height / 2;
    }
    else if (y_align == BOTTOM)
    {
        y_offset = -r.height;
    }
    DrawRectangleRounded({r.x + x_offset, r.y + y_offset, r.width, r.height}, roundness, segments, col);
}


void DrawRectangleRoundedAlign(float x, float y, float width, float height, float roundness, int segments, Color col, int x_align, int y_align)
{
    return DrawRectangleRoundedAlign({ x, y, width, height }, roundness, segments, col, x_align, y_align);
}

void DrawRectangleRoundedLinesAlign(Rectangle r, float roundness, int segments, float lineThick, Color col, int x_align, int y_align)
{
    float x_offset = 0;
    float y_offset = 0;

    // Handle x-alignment
    if (x_align == CENTER)
    {
        x_offset = -r.width / 2;
    }
    else if (x_align == RIGHT)
    {
        x_offset = -r.width;
    }

    // Handle y-alignment
    if (y_align == CENTER)
    {
        y_offset = -r.height / 2;
    }
    else if (y_align == BOTTOM)
    {
        y_offset = -r.height;
    }
    DrawRectangleRoundedLines({r.x + x_offset, r.y + y_offset, r.width, r.height}, roundness, segments, lineThick, col);
}

void DrawRectangleRoundedLinesAlign(float x, float y, float width, float height, float roundness, int segments, float lineThick, Color col, int x_align, int y_align)
{
    return DrawRectangleRoundedLinesAlign({ x, y, width, height }, roundness, segments, lineThick, col, x_align, y_align);
}

void DrawCircleSector(float x, float y, float r, float start_angle, float end_angle, Color col)
{
    int segments = (abs(start_angle - end_angle) / 360) * (2 * PI * r / 3);  // 3 pixels per iteration
    DrawCircleSector({ x, y }, r, start_angle, end_angle, segments, col);
}

void DrawRing(float cx, float cy, float innerRadius, float outerRadius, Color color)
{
    int segments = (2 * PI * outerRadius / 2);  // 2 pixels per iteration
    DrawRing({ cx, cy }, innerRadius, outerRadius, 0, 360, segments, color);
}

void DrawRing(float cx, float cy, float innerRadius, float outerRadius, float startAngle, float endAngle, Color color)
{
    int segments = (abs(startAngle - endAngle) / 360) * (2 * PI * outerRadius / 2);  // 2 pixels per iteration
    DrawRing({ cx, cy }, innerRadius, outerRadius, startAngle, endAngle, segments, color);
}

string convertSeconds(float seconds, int max_time)
{
    bool h_include = (max_time >= 3600); // need to include hours 
    bool m_include = (max_time >= 60); // need to include minutes
    int s = round(seconds);
    int h = s / 3600;
    int m = (s / 60) % 60;
    s %= 60;
    
    if (!h_include && !m_include)  // have seconds pad by space, not zeroes 
        return TextFormat("%2d", s);
    else
        return string(h_include ? TextFormat("%02d:", h) : "") + (m_include ? TextFormat("%02d:", m) : "") + TextFormat("%02d", s);
}

string format_time(string str)
{
    float seconds = stoi(str);
    float max_time = seconds;
    bool h_include = (max_time >= 3600); // need to include hours 
    bool m_include = (max_time >= 60); // need to include minutes
    int s = round(seconds);
    int h = s / 3600;
    int m = (s / 60) % 60;
    s %= 60;
    
    if (!h_include && !m_include)  
        return to_string(s); 
    else
        return string(h_include ? TextFormat("%02d:", h) : "") + (m_include ? TextFormat("%02d:", m) : "") + TextFormat("%02d", s);
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

string readFile(string file_path)
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
        ofstream ofs;
        ofs.open(file_path, ofstream::out | ofstream::trunc);  //header file

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

Font load_font(string path)
{
    // FONT LOADING -----------------------------------------------
    string full_path = path;

    // Loading file to memory
    unsigned int fileSize = 0;
    unsigned char* fileData = LoadFileData(full_path.c_str(), &fileSize);

    // SDF font generation from TTF font
    Font ret_font = { 0 };
    const float font_base = 64;
    ret_font.baseSize = font_base;
    ret_font.glyphCount = 95;
    // Parameters > font size: 16, no glyphs array provided (0), glyphs count: 0 (defaults to 95)
    ret_font.glyphs = LoadFontData(fileData, fileSize, font_base, 0, 0, FONT_SDF);
    // Parameters > glyphs count: 95, font size: 16, glyphs padding in image: 0 px, pack method: 1 (Skyline algorythm)
    Image atlas = GenImageFontAtlas(ret_font.glyphs, &ret_font.recs, 95, font_base, 0, 1);
    ret_font.texture = LoadTextureFromImage(atlas);
    UnloadImage(atlas);
    UnloadFileData(fileData);      // Free memory from loaded file

    SetTextureFilter(ret_font.texture, TEXTURE_FILTER_BILINEAR);    // Required for SDF font  
    return ret_font;
}

vector<string> directory_files(string directory_path, string extension)
{
    FilePathList dir_files = LoadDirectoryFilesEx(directory_path.c_str(), extension.c_str(), false);
    vector<string> file_paths;
    for (int i = 0; i < dir_files.count; i++)
        file_paths.push_back(string(dir_files.paths[i]));
    return file_paths;
}
