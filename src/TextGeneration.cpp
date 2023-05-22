#include "TextGeneration.h"

void generate_text(int buffer_space, string list_name) {
    while (generated_chars.size() - empty_i <= buffer_space)
    {
        generated_chars += get_random_word(list_name) + " ";
    }
}
