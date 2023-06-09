#include "raylibcustom.h"
#include <string>
#include <set>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <map>

namespace fs = std::filesystem;
using namespace std;
size_t get_filesize(string path)
{
    streampos begin,end;
    ifstream myfile(path, ios::binary);
    begin = myfile.tellg();
    myfile.seekg (0, ios::end);
    end = myfile.tellg();
    myfile.close();
    return end - begin;
}

vector<string> recompile;

bool starts_with(std::string str, std::string prefix)
{
    if (str.size() < prefix.size()) {
        return false;
    }
    for (int i = 0; i < prefix.size(); ++i) {
        if (str[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

bool should_recompile(string name)
{
    for (const string &s : recompile)
        if ((name) == (s))
            return true;
    return false;
}


void export_raw_font(string path_name)
{
    // Loading file to memory
    unsigned int fileSize = 0;
    unsigned char* fileData = LoadFileData(("./fonts/" + path_name + ".ttf").c_str(), &fileSize);
    ofstream ofs;
    ofs.open("./export/assets/" + path_name + ".h", ofstream::out | ofstream::trunc);  //header file
    ofs << "#pragma once\n";
    ofs << "#define " << path_name << "_size " << fileSize << '\n';
    ofs << "static unsigned char " + path_name + "_DATA[] = {";
    for (int i = 0; i < fileSize - 1; i++)
    {
        ofs << (int)fileData[i] << ",";
    }
    ofs << (int)fileData[fileSize - 1] << "};\n";
    ofs.close();
}

int main()
{
    // Comment out if not using
    export_raw_font("RobotoMono");
    
    ofstream ofs;
    ofs.open("./src/ResourceInit.h", ofstream::out | ofstream::trunc);  //header file

    string direc_path = "./export/assets";
    set<string> hfiles;
    map<string, int> file_size;
    set<pair<string, string>> imagefiles;
    size_t total_bytes = 0;


    for (const auto& file : fs::directory_iterator(direc_path))
    {
        string filepath = file.path().filename().string();
        string name = file.path().stem().string();
        string ext;
        for (int i = filepath.length() - 1; i >= 0; i--)
        {
            if (filepath[i] == '.')
            {
                ext = filepath.substr(i);
                break;
            }
        }
        // cout << name << ext << " : " << get_filesize(file.path()) << '\n';
        //ofs << name << " | ext: " << ext << '\n';
        if (ext == ".h") hfiles.insert(name);
        if (ext == ".png" || ext == ".jpg") imagefiles.insert({ name, ext });
    }


    // re_export prompt
    string input_str;
    do
    {
        cout << "base recompile path (or `x`): ";
        cin >> input_str;
        if (input_str != "x")
            recompile.push_back(input_str);
    } while (input_str != "x");

    string compile_str = "COMPILED: ";
    
    ofs << "#pragma once\n";

    // FULL_COMPILE (BAKES RESOURCES INTO EXE) ----------------------------------------------------------------------

    for (auto& [name, ext] : imagefiles)  //search for new images with no header file
    {
        string totPath;
        string finalPath = direc_path + "/" + name + ".h";
        if (!hfiles.contains(name) || should_recompile(name))  //create h file for new image
        {
            totPath = direc_path + "/" + name + ext;
            Image img = LoadImage(totPath.c_str());
            ExportImageAsCode(img, finalPath.c_str());
            UnloadImage(img);
            // print out export
            compile_str += name + " | ";
        }
        ofs << "#include \"" + name + ".h\"\n";

        // calculate .h file size
        file_size[name] = get_filesize(finalPath);
        //cout << name << " : " << file_size[name] << '\n';
        total_bytes += file_size[name];
    }
    
    ofs << endl;
    
    // load images macro 
    ofs << "#define init_raw_data \\\n";
    ofs << "Image _img = {0}; \\\n";

    for (auto& [name, ext] : imagefiles)  //.h file macro include for each image
    {
        string NAME = name;
        transform(NAME.begin(), NAME.end(), NAME.begin(), ::toupper);
        ofs << "_img = { 0 }; \\\n";
        ofs << "_img.format = " + NAME + "_FORMAT; \\\n";
        ofs << "_img.width = " + NAME + "_WIDTH; \\\n";
        ofs << "_img.height = " + NAME + "_HEIGHT; \\\n";
        ofs << "_img.data = " + NAME + "_DATA; \\\n";
        ofs << "_img.mipmaps = 1; \\\n";
        ofs << "textureOf[\"" + name + "\"] = LoadTextureFromImage(_img); \\\n";
    }
    
    ofs << ";\n";  //something random at the end of macro
    ofs << endl;
    // END EXPORTER -------------------------------------------------------------------------------------
    ofs.close();
    cout << endl;
    cout << compile_str << endl;
}