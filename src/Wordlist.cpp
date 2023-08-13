#include "WordList.h"
#include "jsonparser.h"
#include <filesystem>
const string DICTIONARY_FOLDER = "languages";

unordered_map<string, vector<string>> word_list({
{
    "english",
    {"the", "be", "of", "and", "a", "to", "in", "he", "have", "it", "that", "for", "they", "I", "with", "as", "not", "on", "she", "at", "by", "this", "we", "you", "do", "but", "from", "or", "which", "one", "would", "all", "will", "there", "say", "who", "make", "when", "can", "more", "if", "no", "man", "out", "other", "so", "what", "time", "up", "go", "about", "than", "into", "could", "state", "only", "new", "year", "some", "take", "come", "these", "know", "see", "use", "get", "like", "then", "first", "any", "work", "now", "may", "such", "give", "over", "think", "most", "even", "find", "day", "also", "after", "way", "many", "must", "look", "before", "great", "back", "through", "long", "where", "much", "should", "well", "people", "down", "own", "just", "because", "good", "each", "those", "feel", "seem", "how", "high", "too", "place", "little", "world", "very", "still", "nation", "hand", "old", "life", "tell", "write", "become", "here", "show", "house", "both", "between", "need", "mean", "call", "develop", "under", "last", "right", "move", "thing", "general", "school", "never", "same", "another", "begin", "while", "number", "part", "turn", "real", "leave", "might", "want", "point", "form", "off", "child", "few", "small", "since", "against", "ask", "late", "home", "interest", "large", "person", "end", "open", "public", "follow", "during", "present", "without", "again", "hold", "govern", "around", "possible", "head", "consider", "word", "program", "problem", "however", "lead", "system", "set", "order", "eye", "plan", "run", "keep", "face", "fact", "group", "play", "stand", "increase", "early", "course", "change", "help", "line"}
},
});

void init_all_dictionaries()
{
    namespace fs = std::filesystem;
    for (const auto& file : fs::directory_iterator(DICTIONARY_FOLDER))
    {
        if (file.is_directory()) continue;
        string filepath = file.path().string();
        string extension = file.path().extension().string();
        string name = file.path().stem().string();
        // Replace all '_' with ' ' 
        if (extension == ".json")
        {
            RSJresource dictionary(readFile(filepath));
            replace(name.begin(), name.end(), '_', ' ');
            word_list[name] = dictionary["words"].as_vector<string>();
        }
    }
}

void init_dictionary_names()
{
    namespace fs = std::filesystem;
    for (const auto& file : fs::directory_iterator(DICTIONARY_FOLDER))
    {
        if (file.is_directory()) continue;
        string filepath = file.path().string();
        string extension = file.path().extension().string();
        string name = file.path().stem().string();
        if (extension == ".json")
        {
            // Replace all '_' with ' ' 
            replace(name.begin(), name.end(), '_', ' ');
            word_list[name].clear();
        }
    }
}

void load_dictionary(string list_name)
{
    if (word_list[list_name].size() > 0)  // Already loaded
        return;
    string file_name = list_name;
    // Return all ' ' to '_'
    replace(file_name.begin(), file_name.end(), ' ', '_');
    string file_path = DICTIONARY_FOLDER + "/" + file_name + ".json";
    RSJresource dictionary(readFile(file_path));
    word_list[list_name] = dictionary["words"].as_vector<string>();
}
