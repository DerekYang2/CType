#pragma once
#include "raylibcustom.h"
/**
 * main color for graph, already toggled buttons
 * button/toggle hovers = text color
 * setting bar hover = text color
 * setting bar divider rectangle = background
 * setting bar unselected color = sub color
 * setting bar selected color = main color
 * setting bar background = sub alt color
 * text highlights (input box) =  main color
 * text input button unhovered = sub alt
 * text input button hover = text color 
 * text input box rectangle = sub alt
 * text input box description + title = sub color
 * test time = main color
*/
class Theme {
    public:
    Color
        background,
        sub_alt,
        main,
        caret,
        sub,
        text,
        error,
        error_extra;
    Theme(Color background, Color main, Color caret, Color sub, Color text, Color error, Color error_extra);
};

extern Theme theme;