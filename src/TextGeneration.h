#pragma once
#include "WordList.h"

struct TextGenerator {
    string list;
    int idx, midpoint, len;
    void set_list(string list_name);
    string get_word();
    void generate_text(int buffer_space);
};

extern TextGenerator text_gen;