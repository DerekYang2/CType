#include "Word.h"


Word::Word(std::string w) {
    word = w;
}

bool Word::is_correct() {
    return correct;
}

int Word::index()
{
    return idx;
}
