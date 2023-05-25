#include "Word.h"


Word::Word(std::string w) {
    word = w;
    typed = "";
    idx = 0;
}

int Word::length()
{
    return word.size();
}

bool Word::correct()
{
    return word == typed;
}

void Word::add(char c)
{
    idx++;
    typed += c;
}

void Word::pop()
{
    idx--;
    typed.pop_back();
}