#pragma once
#include <string>
struct Word {
    int idx = 0;
public:
    std::string word, typed ; // word, user typed
    Word(std::string w);
    int length();
    bool correct();
    void add(char c);
    void pop();
};