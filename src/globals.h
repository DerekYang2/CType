#ifndef _GLOBALSH_
#define _GLOBALSH_
#include "raylibcustom.h"
#include <fstream>

#define shader_path "./fonts/sdf.fs"
// Window Variables
extern const int windowWidth, windowHeight;
extern int gameScreenWidth, gameScreenHeight;

// GLOBAL VARS
extern unordered_set<int> scene_ids[2];
extern Vector2 mouse;
#define LEFT_ALIGN 0
#define RIGHT_ALIGN 1
#define CENTER_ALIGN 2
#define TOP_ALIGN 0
#define BOTTOM_ALIGN 1

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

// Init mt19937 with random seed
extern mt19937 rng;
extern Font font, line_font; // Font: UI font, line_font: font for the line of text
extern float font_spacing, line_spacing;

// COLORS
extern const Color main_color;
#define add_rgb(c, v) Color(c.r + v, c.g + v, c.b + v, 255)

extern char convertKey(int key);
extern int convertChar(char c);
extern void DrawTextAlign(char c, float x, float y, float font_size, Color col = BLACK, int alignment = LEFT_ALIGN, int y_align = TOP_ALIGN, bool is_line = false);
extern void DrawTextAlign(string str, float x, float y, float font_size, Color col = BLACK, int x_align = LEFT_ALIGN, int y_align = TOP_ALIGN, bool is_line = false);
extern void DrawText(string font_name, string text, float x, float y, float font_size, Color col = BLACK);
extern void DrawTextCenter(string font_name, string text, float x, float y, float font_size, Color col = BLACK);
extern void DrawLabel(string text, int x, int y, int fontSize, Color label_col, Color text_col = WHITE);
extern void DrawLabel(string font_name, string text, int x, int y, int fontSize, Color label_col, Color text_col = WHITE, bool rounded = true);
extern void DrawRectangleBoth(int x, int y, int width, int height, Color color, float strokeWidth, Color strokeColor = BLACK);
extern Rectangle formatRect(Rectangle r);  // make rectangle width and height positive
extern char shiftChar(char original);

extern bool starts_with(std::string str, std::string prefix);
extern string add_prefix(string name, string prefix);
extern string remove_prefix(string name, string prefix);
extern string base_name(string name);


#define rand_int(min, max) (uniform_int_distribution<>(min, max)(rng))
#define randf(min, max) (uniform_real_distribution<>(min, max)(rng))
#define rand() (uniform_real_distribution<double>(0.0, 1.0)(rng))

extern vector<string> split_string(string str, char delimiter);
extern string fileText(string file_path);
extern void writeText(string file_path, string str);

#endif