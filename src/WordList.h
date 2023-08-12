#pragma once
#include "globals.h"
extern const string DICTIONARY_FOLDER;
extern unordered_map<string, vector<string>> word_list;
extern vector<string> dictionary_names;
extern void init_all_dictionaries();
extern void init_dictionary_names();
extern void load_dictionary(string file_name);