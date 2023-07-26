#pragma once
#include "raylibcustom.h"
#include <filesystem>
#include "Utils.h"

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

extern const string THEMES_FOLDER;

class Theme {
    public:
    Color
        background,
        main,
        caret,
        sub,
        sub_alt,
        text,
        error,
        error_extra;
    Theme();
    Theme(Color background, Color main, Color caret, Color sub, Color sub_alt, Color text, Color error, Color error_extra);
};

extern Theme theme;
extern unordered_map<string, Theme> theme_map;
extern void fetch_themes();
extern void init_theme(string theme_name);