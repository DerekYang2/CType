#pragma once
#include <string> 
#include <unordered_map>
#include <vector>
#include "globals.h"
using namespace std;
extern unordered_map<string, vector<string>> word_list;
string get_random_word(string list_name);