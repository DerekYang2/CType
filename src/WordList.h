#pragma once
#include "globals.h"
extern const string DICTIONARY_FOLDER;
extern unordered_map<string, vector<string>> word_list;
extern void init_all_dictionaries();
extern void init_dictionary_names();
extern void load_dictionary(string list_name); // List name should have spaces instead of underscores