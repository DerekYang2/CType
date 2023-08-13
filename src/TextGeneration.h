#pragma once
#include "WordList.h"
/**
 * TODO: Handle punctuation in words and sentences
 * TODO: Maybe unlaod old word lists?
*/
class TextGenerator {
    vector<string> contractions;
    string list;
    bool punctuation = false;
    bool numbers = false;
    bool needs_capital = false;
    int idx, midpoint, len;
    string get_ending();
    string get_insert();
    string get_number();
    public:
    TextGenerator();
    void set_punctuation(bool on);
    void set_numbers(bool on);
    void set_list(string list_name);
    void add_sentence(string sentence);
    string list_name();
    void generate_text(int buffer_space);
    bool get_punctuation(); 
    bool get_numbers();
};

extern TextGenerator text_gen;