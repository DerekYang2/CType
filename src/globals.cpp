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
    DrawTextEx(font, str.c_str(), { x + x_offset, y + y_offset }, font_size, font_size / font_spacing, col);
}

void DrawText(string text, float x, float y, float font_size, Color col)
{
    DrawTextEx(font, text.c_str(), {x, y}, font_size, font_size/font_spacing, col);
}

void DrawTextCenter(string text, float x, float y, float font_size, Color col)
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
    return MeasureTextEx(font, str.c_str(), font_size, font_size / font_spacing);
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

// overloads
//Addition overloads: componentwise addition
Vector2 operator+(const Vector2& a, const Vector2& b) {
return Vector2Add(a,b);
}

Vector3 operator+(const Vector3& a, const Vector3& b) {
return Vector3Add(a,b);
}

Vector4 operator+(const Vector4& a, const Vector4& b) {
return (Vector4){a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};
}

Vector2& operator+=(Vector2& a, const Vector2& b) {
    a=a+b;
return a;
}

Vector3& operator+=(Vector3& a, const Vector3& b) {
    a=a+b;
return a;
}

Vector4& operator+=(Vector4& a, const Vector4& b) {
    a=a+b;
return a;
}

Matrix operator+(const Matrix& left, const Matrix& right) {
return MatrixAdd(left,right);
}

Matrix& operator+=(Matrix& left, const Matrix& right) {
    left=MatrixAdd(left,right);
return left;
}

Color operator+(const Color& a, const Color& b) {
    int red,green,blue,alpha;  //To ensure that addition of a+b does not result in components greater than unsigned char's max of 255, we upconvert to int, then check the sum, and cast back to unsigned char
    Color c;
    red=(int)a.r+(int)b.r;
    green=(int)a.g+(int)b.g;
    blue=(int)a.b+(int)b.b;
    alpha=(int)a.a+(int)b.a;

    c.r=(unsigned char)((red>255)?255:red);
    c.g=(unsigned char)((green>255)?255:green);
    c.b=(unsigned char)((blue>255)?255:blue);
    c.r=(unsigned char)((alpha>255)?255:alpha);
return c;
}

Color& operator+=(Color&a, const Color& b) {
    a=a+b;
return a;
}

//Negation: Unary Minus operator
Vector2& operator-(Vector2& a) {
    a=Vector2Negate(a);
return a;
}

Vector3& operator-(Vector3& a) {
    a=Vector3Negate(a);
return a;
}

//Since Quaternion is a Vector4 typedef, I did not provide a negation for Vector4 to avoid confusion with QuaternionInvert();

//Subtraction overloads: componentwise subtraction
Vector2 operator-(const Vector2& a, const Vector2& b) {
return Vector2Subtract(a,b);
}

Vector3 operator-(const Vector3& a, const Vector3& b) {
return Vector3Subtract(a,b);
}

Vector4 operator-(const Vector4& a, const Vector4& b) {
return (Vector4){a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};
}

Vector2& operator-=(Vector2& a, const Vector2& b) {
    a=a-b;
return a;
}

Vector3& operator-=(Vector3& a, const Vector3& b) {
    a=a-b;
return a;
}

Vector4& operator-=(Vector4& a, const Vector4& b) {
    a=a-b;
return a;
}

Matrix operator-(const Matrix& left, const Matrix& right) {
return MatrixSubtract(left,right);
}

Matrix& operator-=(Matrix& left, const Matrix& right) {
    left=MatrixSubtract(left,right);
return left;
}

Color operator-(const Color& a, const Color& b) {
    int red,green,blue,alpha;  //Convert to int, clamp to min zero, convert back to unsigned char
    Color c;
    red=(int)a.r-(int)b.r;
    green=(int)a.g-(int)b.g;
    blue=(int)a.b-(int)b.b;
    alpha=(int)a.a-(int)b.a;

    c.r=(unsigned char)((red<0)?0:red);
    c.g=(unsigned char)((green<0)?0:green);
    c.b=(unsigned char)((blue<0)?0:blue);
    c.r=(unsigned char)((alpha<0)?0:alpha);
return c;
}

Color& operator-=(Color&a, const Color& b) {
    a=a-b;
return a;
}

//Multiplication overload only provides for multiplying a vector by a scalar.   Vector * Vector is not overloaded to avoid confusion whether one intends a dot product, cross product, etc.
Vector2 operator*(const Vector2& a, float b) {
return Vector2Scale(a,b);
}
Vector2 operator*(float b, const Vector2& a)
{
return Vector2Scale(a,b);
}

Vector3 operator*(const Vector3& a, float b)
{
return Vector3Scale(a,b);
}

Vector3 operator*(float b, const Vector3& a)
{
return Vector3Scale(a,b);
}

Matrix operator*(const Matrix& left, const Matrix& right) {
return MatrixMultiply(left,right);
}

Matrix& operator*=(Matrix& left, const Matrix& right) {
    left=MatrixMultiply(left,right);
return left;
}

Vector2& operator*=(Vector2& a, const float b) {
    a=a*b;
return a;
}

Vector3& operator*=(Vector3& a, const float b) {
    a=a*b;
return a;
}

//I'm not sure if the following is useful, but here it is anyway for completeness
Color operator*(const Color& a, const Color& b) {
    int red,green,blue,alpha;
    Color c;
    red=(int)a.r*(int)b.r;
    green=(int)a.g*(int)b.g;
    blue=(int)a.b*(int)b.b;
    alpha=(int)a.a*(int)b.a;

    c.r=(unsigned char)( ( (red>255)?255:(red<0)?0:red) ); //Nested ternary clamps return between 0 and 255
    c.g=(unsigned char)( ( (green>255)?255:(green<0)?0:green) );
    c.b=(unsigned char)( ( (blue>255)?255:(blue<0)?0:blue) );
    c.r=(unsigned char)( ( (alpha>255)?255:(alpha<0)?0:alpha) );
return c;
}

Color& operator*=(Color&a, const Color& b) {
    a=a*b;
return a;
}

Color operator*(const Color& a, const float b) {
    float red,green,blue,alpha;  //We cast to float, clamp, then recast back to unsigned char
    Color c;
    red=(float)a.r*b;
    green=(float)a.g*b;
    blue=(float)a.b*b;
    alpha=(float)a.a*b;

    c.r=(unsigned char)( ( (red>255)?255:(red<0)?0:red) ); //Nested ternary clamps return between 0 and 255
    c.g=(unsigned char)( ( (green>255)?255:(green<0)?0:green) );
    c.b=(unsigned char)( ( (blue>255)?255:(blue<0)?0:blue) );
    c.r=(unsigned char)( ( (alpha>255)?255:(alpha<0)?0:alpha) );
return c;
}

Color& operator*=(Color&a, const float b) {
    a=a*b;
return a;
}

//Division overload: Merely scalar multiplication by the reciprocal, with a Divide-By-Zero check.
Vector2 operator/(const Vector2& a, const float b) {
    if (b==0.0) {
        std::cerr<<"Division by zero error."<<std::endl;
        throw std::domain_error("Division by zero error");
    }
    float recip=1.0/b;
return a*recip;
}

Vector3 operator/(const Vector3& a, const float b) {
    if (b==0.0) {
        std::cerr<<"Division by zero error."<<std::endl;
        throw std::domain_error("Division by zero error");
    }
    float recip=1.0/b;
return a*recip;
}

Vector2& operator/=(Vector2& a, const float b) {
    a=a/b;
return a;
}

Vector3& operator/=(Vector3& a, const float b) {
    a=a/b;
return a;
}

//I'm not sure if the following is useful either, but here it is for completeness
Color operator/(const Color& a, const Color& b) {
    float red,green,blue,alpha;
    Color c;
    red=(float)a.r/(float)b.r;
    green=(float)a.g/(float)b.g;
    blue=(float)a.b/(float)b.b;
    alpha=(float)a.a/(float)b.a;

    c.r=(unsigned char)( ( (red>255)?255:(red<0)?0:red) ); //Nested ternary clamps return between 0 and 255
    c.g=(unsigned char)( ( (green>255)?255:(green<0)?0:green) );
    c.b=(unsigned char)( ( (blue>255)?255:(blue<0)?0:blue) );
    c.r=(unsigned char)( ( (alpha>255)?255:(alpha<0)?0:alpha) );
return c;
}

Color& operator/=(Color&a, const Color& b) {
    a=a/b;
return a;
}

Color operator/(const Color& a, const float b) {
    float red,green,blue,alpha;  //We cast to float, clamp, then recast back to unsigned char
    Color c;
    red=(float)a.r/b;
    green=(float)a.g/b;
    blue=(float)a.b/b;
    alpha=(float)a.a/b;

    c.r=(unsigned char)( ( (red>255)?255:(red<0)?0:red) ); //Nested ternary clamps return between 0 and 255
    c.g=(unsigned char)( ( (green>255)?255:(green<0)?0:green) );
    c.b=(unsigned char)( ( (blue>255)?255:(blue<0)?0:blue) );
    c.r=(unsigned char)( ( (alpha>255)?255:(alpha<0)?0:alpha) );
return c;
}

Color& operator/=(Color&a, const float b) {
    a=a/b;
return a;
}


// Equality operator overloads
//
// Integer equality
bool operator==(const Color& c1, const Color& c2) {
    if ( (c1.r==c2.r) && (c1.g==c2.g) && (c1.b==c2.b) && (c1.a==c2.a) ) return true;
return false;
}

// Float equality for Vector2 and Vector3
//
//Comparing float values requires care.  Choose EQUALITY_OPERATOR_SIMPLE, EQUALITY_OPERATOR_KNUTH, or neither in the #defines at the top of the file
#ifdef EQUALITY_OPERATOR_SIMPLE
bool operator==(const Vector2& a, const Vector2& b) {
    if ( (a.x==b.x) && (a.y==b.y)) return true;
return false;
}

bool operator==(const Vector3& a, const Vector3& b) {
    if ( (a.x==b.x) && (a.y==b.y) && (a.z==b.z)) return true;
return false;
}

bool operator==(const Vector4& a, const Vector4& b) {
    if ( (a.x==b.x) && (a.y==b.y) && (a.z==b.z) && (a.w==b.w)) return true;
return false;
}
#endif // EQUALITY_OPERATOR_SIMPLE

#ifdef EQUALITY_OPERATOR_KNUTH
//Takes a conservative approach and only affirms that two vectors are equal if all of their respective components are equal within machine precision.
#include <limits>
#include <cmath>
bool operator==(const Vector2& a, const Vector2& b){
    bool XIsEqual=false;
    bool YIsEqual=false;
    XIsEqual = std::fabs(a.x - b.x) <= ( (std::fabs(a.x) > std::fabs(b.x) ? std::fabs(b.x) : std::fabs(a.x)) * std::numeric_limits<float>::epsilon() );
    YIsEqual = std::fabs(a.y - b.y) <= ( (std::fabs(a.y) > std::fabs(b.y) ? std::fabs(b.y) : std::fabs(a.y)) * std::numeric_limits<float>::epsilon() );
return (XIsEqual && YIsEqual);
}

bool operator==(const Vector3& a, const Vector3& b){
    bool XIsEqual=false;
    bool YIsEqual=false;
    bool ZIsEqual=false;
    XIsEqual = std::fabs(a.x - b.x) <= ( (std::fabs(a.x) > std::fabs(b.x) ? std::fabs(b.x) : std::fabs(a.x)) * std::numeric_limits<float>::epsilon() );
    YIsEqual = std::fabs(a.y - b.y) <= ( (std::fabs(a.y) > std::fabs(b.y) ? std::fabs(b.y) : std::fabs(a.y)) * std::numeric_limits<float>::epsilon() );
    ZIsEqual = std::fabs(a.z - b.z) <= ( (std::fabs(a.z) > std::fabs(b.z) ? std::fabs(b.z) : std::fabs(a.z)) * std::numeric_limits<float>::epsilon() );
return (XIsEqual && YIsEqual && ZIsEqual);
}

bool operator==(const Vector4& a, const Vector4& b){
    bool XIsEqual=false;
    bool YIsEqual=false;
    bool ZIsEqual=false;
    bool WIsEqual=false;
    XIsEqual = std::fabs(a.x - b.x) <= ( (std::fabs(a.x) > std::fabs(b.x) ? std::fabs(b.x) : std::fabs(a.x)) * std::numeric_limits<float>::epsilon() );
    YIsEqual = std::fabs(a.y - b.y) <= ( (std::fabs(a.y) > std::fabs(b.y) ? std::fabs(b.y) : std::fabs(a.y)) * std::numeric_limits<float>::epsilon() );
    ZIsEqual = std::fabs(a.z - b.z) <= ( (std::fabs(a.z) > std::fabs(b.z) ? std::fabs(b.z) : std::fabs(a.z)) * std::numeric_limits<float>::epsilon() );
    WIsEqual = std::fabs(a.w - b.w) <= ( (std::fabs(a.w) > std::fabs(b.w) ? std::fabs(b.w) : std::fabs(a.w)) * std::numeric_limits<float>::epsilon() );
return (XIsEqual && YIsEqual && ZIsEqual && WIsEqual);
}
#endif // EQUALITY_OPERATOR_KNUTH