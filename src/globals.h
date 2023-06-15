#ifndef _GLOBALSH_
#define _GLOBALSH_
#include "raylibcustom.h"
#include "Theme.h"
#include <fstream>
#include "Word.h"
#include "Utils.h"
#define shader_path "./fonts/sdf.fs"
// Window Variables
extern int windowWidth, windowHeight;
extern int gameScreenWidth, gameScreenHeight;
extern bool mouse_focus;
// GLOBAL VARS
extern unordered_set<int> scene_ids[5];
extern int scene;
#define START 0
#define TEST 1
#define END 2

extern Vector2 mouse;

extern int globalFrame; // Frames printed in the game scene
extern unordered_map<string, Texture> textureOf;
extern int empty_i;  // position of the first empty position in generated_chars
extern string generated_chars;
extern int max_word_length;
/**
 * 
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