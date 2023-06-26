#include "UserData.h"

string data_path = "./data/user_data.json";
RSJresource data_json;

void load_user_data()
{
    RSJresource default_json(
    "{ \n"
    "    'dictionary' : 'english', \n"
    "    'test time' : 30, \n"
    "    'test history': [\n"
    //"    { wpm: 0.0, raw: 0.0, accuracy: 0.0, consistency: 0.0, characters: '0/0/0/0', test type: 'test' },\n"
    "    ]\n,"
    "}  \n");
    // test insert 
    default_json["test history"][0] = RSJresource("{ wpm: 0.0, raw: 0.0, accuracy: 0.0, consistency: 0.0, characters: '0/0/0/0', test type: 'test' }");
    default_json["test history"][1] = RSJresource("{ wpm: 100.0, raw: 0.0, accuracy: 0.0, consistency: 0.0, characters: '0/0/1/0', test type: 'test2' }");
    default_json.print();

    if (!FileExists(data_path.c_str()))
    {
        writeText(data_path, default_json.as_str());
    }
    // read 
    data_json = RSJresource(readFile(data_path));
    vector<RSJresource> history_array = data_json["test history"].as_array();
/*     cout << "LEN: " << history_array.size() << endl;
    for (auto& res : history_array)
    {
        cout << "WPM: " << res["wpm"].as<double>() << endl;                     
    } */
}

void write_user_data()
{
    writeText(data_path, data_json.as_str());
}