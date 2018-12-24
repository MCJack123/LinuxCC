#ifndef SETTINGSAPI_HPP
#define SETTINGSAPI_HPP
#include <string>
#include <vector>
#include <jsoncpp/json/json.h>

using std::string;

class SettingsAPI {
    Json::Value root;
public:
    SettingsAPI();
    ~SettingsAPI();
    void set(string key, Json::Value value);
    Json::Value get(string key, Json::Value def = Json::nullValue);
    void unset(string key);
    void clear();
    std::vector<string> getNames();
    bool load(string path);
    bool save(string path);
};

extern SettingsAPI settings;

#endif