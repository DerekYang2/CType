#ifndef _RAYLIBCUSTOM_H_
#define _RAYLIBCUSTOM_H_
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <math.h>
#include <deque>
#include <unordered_map>
#include <random>
#include <unordered_set>
#include <vector>
#include <chrono>
#include <functional>
#include <list>
#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
using namespace std;

#define sgn(x) (((x)>0) - ((x)<0))
#define rgb(...) Color(__VA_ARGS__, 255)
#define rad(x) (0.0174532925199432957692222222222222222222f * (x))
#define deg(x) (57.295779513082320876846364344191097757053f * (x))
#define ceil_div(num, denom) (((num) + (denom) - 1) / (denom))
#define unix_time std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())
#define rgba(r, g, b, a) Color(r, g, b, 255*(a))
#define cosa(x) (cosf(rad(x)))
#define sina(x) (sinf(rad(x)))
#define t_s(x) (to_string(x))
#define square(x) ((x) * (x))
#define hypot(x, y) sqrt(square(x) + square(y))
#define ColorNewAlpha(c, a) (Color(c.r, c.g, c.b, a))
// String funcs ------------------------------------------------------------------

#define is_digit(c) ((c) >= '0' && (c) <= '9')
// inclusive boundaries
#define substrI(str, start, end) str.substr(start, (end) - (start) + 1)  
// exclusive boundaries
#define substrE(str, start, end) str.substr(start, (end) - (start))

#define PRINT_VECTORS_WITH_PARENTHESES
//#define PRINT_VECTORS_BY_COMPONENT

//#define EQUALITY_OPERATOR_SIMPLE
#define EQUALITY_OPERATOR_KNUTH

#endif