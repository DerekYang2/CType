#include "Word.h"
#include <iostream>
Word::Word()
{
    word = "";
    typed = "";
    idx = 0;
    space_status = false;
    skipped = false;
}
Word::Word(std::string w)
{
    word = w;
    typed = "";
    idx = 0;
    space_status = false;
    skipped = false;
}

void Word::reset()
{
    typed = "";
    idx = 0;
    space_status = false;
    skipped = false;
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

int Word::contribution()
{
    if (!current_correct()) return 0;
    
    return typed.size() + space_status;
}

void Word::seal_word(bool correct_space)
{
    space_status = correct_space;
    if (!correct_space) skipped = true;
}

void Word::unseal_word()
{
    space_status = 0;
    skipped = false;
}

bool Word::current_correct() 
{
    if (word.size() < typed.size() || skipped)
    {
        return false;
    }
    for (int i = 0; i < typed.size(); ++i)
    {
        if (word[i] != typed[i]) {
            return false;
        }
    }
    return true;
}