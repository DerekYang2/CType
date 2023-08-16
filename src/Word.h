#pragma once
#include <string>
struct Word {
    int idx = 0;
    bool space_status; // 1 = good, 0 = bad/pending
    bool skipped = 0;
public:
    std::string word, typed; // word, user typed
    Word();
    Word(std::string w);
    void reset();
    int length();
    bool correct();
    void add(char c);
    void pop();
    int contribution();
    bool current_correct(); // if current typed is prefix of word
    void seal_word(bool correct_space);  // updates space_status
    void unseal_word();  // restarts space_status (sets to pending)
};