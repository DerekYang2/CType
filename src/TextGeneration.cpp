#include "TextGeneration.h"
#include "Word.h"
extern vector<Word> words;


void TextGenerator::set_list(string list_name)
{
    list = list_name;
    len = word_list[list].size();
    midpoint = (len + 1) / 2;  // ceiling division by 2
    // range 1: [0, midpoint), range 2: [midpoint, len)
    idx = 0;
    // initial shuffle
    shuffle(word_list[list].begin(), word_list[list].end(), rng);
    // update longest word
    max_word_length = 0;
    for (auto& word : word_list[list])
        max_word_length = max(max_word_length, (int)word.length());
}

string TextGenerator::get_word()
{
    if (idx == 0) // entering range 1, reshuffle [0, midpoint)
    {
        shuffle(word_list[list].begin(), word_list[list].begin() + midpoint, rng);
    }
    if (idx == midpoint)  // entering range 2, reshuffle [midpoint, len)
    {
        shuffle(word_list[list].begin() + midpoint, word_list[list].end(), rng);
    }
    int cur_idx = idx;
    idx = (idx + 1) % len;  // update idx
    return word_list[list][cur_idx];
}

void TextGenerator::generate_text(int buffer_space)
{
    while (generated_chars.size() - empty_i <= buffer_space)
    {
        words.push_back(get_word());
        generated_chars += words.back().word + " ";
    }
}
