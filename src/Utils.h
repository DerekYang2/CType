#pragma once
#include "raylibcustom.h"
#include <fstream>
#include "Theme.h"

extern int GetCodepointNext(const char* text, int* codepointSize);

extern Font font;
extern float font_spacing;

// COLORS
#define add_rgb(c, v) Color((c).r + v, (c).g + v, (c).b + v, 255)
#define darkenColor(c, v) Color((c).r * (1.0f - v), (c).g * (1.0f - v), (c).b * (1.0f - v), (c).a)
#define lightenColor(c, v) Color((c).r + v * (255 - (c).r), (c).g + v * (255 - (c).g), (c).b + v * (255 - (c).b), (c).a)
#define LEFT 0
#define RIGHT 1
#define CENTER 2
#define TOP 0
#define BOTTOM 1

// override draw text ex with custom spacing
extern void DrawTextEx(Font font, string text, Vector2 position, float fontSize, float spacing, Color tint, float line_spacing = 1.f);
extern Vector2 MeasureTextEx(Font font, string text, float fontSize, float spacing, float line_spacing = 1.f);

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
extern float MeasureFontSize(string str, float width, float height = INT_MAX);

extern bool starts_with(std::string str, std::string prefix);
extern string add_prefix(string name, string prefix);
extern string remove_prefix(string name, string prefix);
extern Font load_font(string path);
extern vector<string> directory_files(string directory_path, string extension);

#define rand_int(min, max) (uniform_int_distribution<>(min, max)(rng))
#define randf(min, max) (uniform_real_distribution<>(min, max)(rng))
#define rand() (uniform_real_distribution<double>(0.0, 1.0)(rng))

extern vector<string> split_string(string str, char delimiter);
extern string fileText(string file_path);
extern void writeText(string file_path, string str);