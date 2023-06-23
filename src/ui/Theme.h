#pragma once
#include "raylibcustom.h"
/**
 * main color for graph, toggled buttons, etc
 * sub color is color of untyped and untogged buttons
 * text generally = main color for dark theme, but is usually black/gray for light theme
*/
class Theme {
    public:
    Color
        background,
        background_shade,
        main,
        caret,
        sub,
        text,
        error,
        error_extra;
    Theme(Color background, Color main, Color caret, Color sub, Color text, Color error, Color error_extra);
};

extern Theme theme;