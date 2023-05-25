#include "TextGeneration.h"
#include "Word.h"
extern vector<Word> words;

void generate_text(int buffer_space, string list_name)
{
    while (generated_chars.size() - empty_i <= buffer_space)
    {
        words.push_back(Word(get_random_word(list_name)));
        generated_chars += words.back().word + " ";
    }
}
