#include "TextGeneration.h"
#include "Word.h"
extern vector<Word> words;

TextGenerator::TextGenerator()
{
    // Some more common contractions
    contractions = {"aren't", "can't", "couldn't", "could've", "didn't", "doesn't", "don't",
        "hadn't", "hasn't", "haven't", "he'd", "he'll", "he's", "I'd", "I'll",
        "I'm", "I've", "isn't", "it'd", "it'll", "it's", "let's", "ma'am",
        "might've", "mustn't", "must've", "ol'",
        "she'd", "she'll", "she's", "shouldn't", "should've",
        "that'd", "that's", "there'll", "there's", "they'd", "they'll",
        "they're", "they've", "'twas", "wasn't", "we'd", "we'll", "we're", "we've",
        "weren't", "what's", "where'd", "where's",
        "who'd", "who's", "who've", "why'd", "won't", "wouldn't", "would've",
        "you'd", "you'll", "you're", "you've", "-"};
}

string TextGenerator::get_ending()
{
    float rf = rand();
    if (rf < 0.8)
        return ".";
    else if (rf < 0.9)
        return "?";
    else
        return "!";
}

string TextGenerator::get_insert()
{
    float rf = rand();
    if (rf < 0.7)
        return ",";
    else if (rf < 0.9)
        return ";";
    else
        return ":";
}

string TextGenerator::get_number()
{
    int len = rand_int(1, 4);
    string str = "";
    for (int i = 0; i < len; i++) 
        str += to_string(rand_int(0, 9));
    if (punctuation)
    {
        float tot_words = 100;  // Probability is r_int out of tot_words
        int r_int = rand_int(1, tot_words);
        if (r_int <= 2)
            str = "$" + str;
        else if (r_int <= 4)
            str = "-" + str;
        else if (r_int <= 6)
            str += "%";
    }
    return str;
}

void TextGenerator::set_punctuation(bool on)
{
    punctuation = on;
}

void TextGenerator::set_numbers(bool on)
{
    numbers = on;
}

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
    // Very first word
    if (punctuation && generated_chars.empty())
        needs_capital = true;
    
    float tot_words = 100;  // Probability is r_int out of tot_words
    int r_int = rand_int(1, tot_words);
    const int p_contract = 5, p_end = 7, p_insert = 5, p_wrap = 2;  // 5 in 100 (p in tot_words)
    
    string return_word;
    if (numbers && r_int <= 5) // insert random number
    {
        needs_capital = false;
        return get_number();
    } else if (punctuation && r_int <= p_contract)  // Return random contraction 
    {
        return_word = contractions[rand_int(0, contractions.size() - 1)];
        if (needs_capital)
        {
            return_word[0] = toupper(return_word[0]);
            needs_capital = false;
        }
        return return_word;
    } else  // Return from word list
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
        return_word = word_list[list][cur_idx];
        // Add punctuation
        if (punctuation)
        {
            bool next_capital = false;
            if (r_int <= p_contract + p_end)  
            {
                return_word += get_ending();
                next_capital = true;
            } else if (r_int <= p_contract + p_end + p_insert)  
            {
                return_word += get_insert();
            } else if (r_int <= p_contract + p_end + p_insert + p_wrap)
            {
                if (!needs_capital)  // Not a starting word
                {
                    if (rand() <= 0.6)
                        return_word = "\"" + return_word + "\"";
                    else 
                        return_word = "(" + return_word + ")";
                }
            }
            if (needs_capital)
            {
                return_word[0] = toupper(return_word[0]);
            }
            
            needs_capital = next_capital;
        }
        return return_word;
    }
}

string TextGenerator::list_name()
{
    return list;
}

void TextGenerator::generate_text(int buffer_space)
{
    while (generated_chars.size() - empty_i <= buffer_space)
    {
        words.push_back(get_word());
        generated_chars += words.back().word + " ";
    }
}
