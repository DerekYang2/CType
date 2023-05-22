#pragma once
#include <string>
struct Word {
    bool correct = false;
    int idx = 0;
public:
    std::string word; // word
    Word(std::string w);
    bool is_correct();
    int index();  // current empty index
        
};