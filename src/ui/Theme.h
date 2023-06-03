#pragma once
#include "raylibcustom.h"
/**
 * main color for graph, toggled buttons, etc
 * sub color is color of untyped and untogged buttons
 * text generally = main color for dark theme, but is usually black/gray for light theme
*/
struct Theme {
    Color
        background,
        main,
        sub,
        text,
        error,
        error_extra;
};

extern Theme theme;