#pragma once
#include "WordList.h"

class TextGenerator {
    vector<string> contractions;
    string list;
    bool punctuation = false;
    bool needs_capital = false;
    int idx, midpoint, len;
    string get_ending();
    string get_insert();
    public:
    TextGenerator();
    void set_punctuation(bool on);
    void set_list(string list_name);
    string get_word();
    string list_name();
    void generate_text(int buffer_space);
};

extern TextGenerator text_gen;