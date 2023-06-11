#ifndef _GLOBALSH_
#define _GLOBALSH_
#include "raylibcustom.h"
#include "Theme.h"
#include <fstream>
#include "Word.h"
#define shader_path "./fonts/sdf.fs"
// Window Variables
extern int windowWidth, windowHeight;
extern int gameScreenWidth, gameScreenHeight;

// GLOBAL VARS
extern unordered_set<int> scene_ids[5];
extern int scene;
#define START 0
#define TEST 1
#define END 2

extern Vector2 mouse;
#define LEFT 0
#define RIGHT 1
#define CENTER 2
#define TOP 0
#define BOTTOM 1

extern int globalFrame; // Frames printed in the game scene
extern unordered_map<string, Texture> textureOf;
extern int empty_i;  // position of the first empty position in generated_chars
extern string generated_chars;
/**
 * STATUS:
 * first = 0 -> store correct char
 * first = 1 -> store incorrect char, typed
 * first = 2 -> store incorrect char, not typed
 * first = 3 -> store incorrect char, extra typed
*/
#define CORRECT 0
#define INCORRECT 1
#define MISSING 2
#define EXTRA 3
#define FUTURE 4
extern vector<pair<char, char>> char_status;
extern vector<Word> words;
extern int word_i;
// Init mt19937 with random seed
extern mt19937 rng;
extern Font font; // Font: UI font
extern float font_spacing;
extern Shader shader;
extern Vector2 char_dimension[CHAR_MAX + 1];

// COLORS
#define add_rgb(c, v) Color((c).r + v, (c).g + v, (c).b + v, 255)
#define multiply_rgb(c, v) Color((c).r * v, (c).g * v, (c).b * v, (c).a)

extern bool IsKeyPressed();
extern char convertKey(int key);
extern int convertChar(char c);
extern Vector2 MeasureTextEx(string str, float font_size);
extern void DrawTextAlign(char c, float x, float y, float font_size, Color col = BLACK, int alignment = LEFT, int y_align = TOP);
extern void DrawTextAlign(string str, float x, float y, float font_size, Color col = BLACK, int x_align = LEFT, int y_align = TOP);
extern void DrawText(string text, float x, float y, float font_size, Color col = BLACK);
extern void DrawTextCenter(string text, float x, float y, float font_size, Color col = BLACK);
extern void DrawLabel(string text, int x, int y, int fontSize, Color label_col, Color text_col = WHITE);
extern void DrawLabel(string text, int x, int y, int fontSize, Color label_col, Color text_col = WHITE, bool rounded = true);
extern void DrawRectangleBoth(int x, int y, int width, int height, Color color, float strokeWidth, Color strokeColor = BLACK);
extern void DrawRectangleAlign(Rectangle r, Color col, int x_align = LEFT, int y_align = TOP);
extern void DrawRectangleAlign(float x, float y, float width, float height, Color col, int x_align = LEFT, int y_align = TOP);
extern void DrawRectangleRoundedAlign(Rectangle r, float roundness, int segments, Color col, int x_align, int y_align);
extern void DrawRectangleRoundedAlign(float x, float y, float width, float height, float roundness, int segments, Color col, int x_align, int y_align);
extern void DrawCircleSector(float x, float y, float r, float start_angle, float end_angle, Color col);
extern void DrawRing(float cx, float cy, float innerRadius, float outerRadius, Color color); // Draw ring
extern void DrawRing(float cx, float cy, float innerRadius, float outerRadius, float startAngle, float endAngle, Color color); // Draw ring
extern string convertSeconds(float seconds, int max_time);
extern Rectangle formatRect(Rectangle r);  // make rectangle width and height positive
extern char shiftChar(char original);

extern bool starts_with(std::string str, std::string prefix);
extern string add_prefix(string name, string prefix);
extern string remove_prefix(string name, string prefix);
extern string base_name(string name);
extern Font load_font(string path);

#define rand_int(min, max) (uniform_int_distribution<>(min, max)(rng))
#define randf(min, max) (uniform_real_distribution<>(min, max)(rng))
#define rand() (uniform_real_distribution<double>(0.0, 1.0)(rng))

extern vector<string> split_string(string str, char delimiter);
extern string fileText(string file_path);
extern void writeText(string file_path, string str);

Vector2 operator+(const Vector2& a, const Vector2& b);
Vector3 operator+(const Vector3& a, const Vector3& b);
Vector4 operator+(const Vector4& a, const Vector4& b);
Vector2& operator+=(Vector2& a, const Vector2& b);
Vector3& operator+=(Vector3& a, const Vector3& b);
Vector4& operator+=(Vector4& a, const Vector4& b);
Matrix operator+(const Matrix& left, const Matrix& right);
Matrix& operator+=(Matrix& left, const Matrix& right);
Color operator+(const Color& a, const Color& b);
Color& operator+=(Color& a, const Color& b);

Vector2& operator-(Vector2& a);
Vector3& operator-(Vector3& a);
Vector2 operator-(const Vector2& a, const Vector2& b);
Vector3 operator-(const Vector3& a, const Vector3& b);
Vector4 operator-(const Vector4& a, const Vector4& b);
Vector2& operator-=(Vector2& a, const Vector2& b);
Vector3& operator-=(Vector3& a, const Vector3& b);
Vector4& operator-=(Vector4& a, const Vector4& b);
Matrix operator-(const Matrix& left, const Matrix& right);
Matrix& operator-=(Matrix& left, const Matrix& right);
Color operator-(const Color& a, const Color& b);
Color& operator-=(Color& a, const Color& b);

Vector2 operator*(const Vector2& a, float b);
Vector2 operator*(float b, const Vector2& a);
Vector3 operator*(const Vector3& a, float b);
Vector3 operator*(float b, const Vector3& a);
Matrix operator*(const Matrix& left, const Matrix& right);
Matrix& operator*=(Matrix& left, const Matrix& right);
Vector2& operator*=(Vector2& a, const float b);
Vector3& operator*=(Vector3& a, const float b);
Color operator*(const Color& a, const Color& b);
Color& operator*=(Color& a, const Color& b);
Color operator*(const Color& a, const float b);
Color& operator*=(Color& a, const float b);

Vector2 operator/(const Vector2& a, const float b);
Vector3 operator/(const Vector3& a, const float b);
Vector2& operator/=(Vector2& a, const float b);
Vector3& operator/=(Vector3& a, const float b);
Color operator/(const Color& a, const Color& b);
Color& operator/=(Color& a, const Color& b);
Color operator/(const Color& a, const float b);
Color& operator/=(Color& a, const float b);

// Equality operator overloads
//
// Integer equality
bool operator==(const Color& c1, const Color& c2);

// Float equality for Vector2 and Vector3
//
//Comparing float values requires care.  Choose EQUALITY_OPERATOR_SIMPLE, EQUALITY_OPERATOR_KNUTH, or neither in the #defines at the top of the file
#ifdef EQUALITY_OPERATOR_SIMPLE
bool operator==(const Vector2& a, const Vector2& b);
bool operator==(const Vector3& a, const Vector3& b);
bool operator==(const Vector4& a, const Vector4& b);
#endif // EQUALITY_OPERATOR_SIMPLE

#ifdef EQUALITY_OPERATOR_KNUTH
//Takes a conservative approach and only affirms that two vectors are equal if all of their respective components are equal within machine precision.
#include <limits>
#include <cmath>
bool operator==(const Vector2& a, const Vector2& b);
bool operator==(const Vector3& a, const Vector3& b);
bool operator==(const Vector4& a, const Vector4& b);
#endif // EQUALITY_OPERATOR_KNUTH

#endif