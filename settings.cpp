#include "settings.hpp"
#include <fstream>

SettingsAPI settings;

SettingsAPI::SettingsAPI() {
    std::ifstream in("~/.config/linuxcc.json");
    in >> root;
    in.close();
}

SettingsAPI::~SettingsAPI() {
    std::ofstream out("~/.config/linuxcc.json");
    out << root;
    out.close();
}

void SettingsAPI::set(string key, Json::Value value) {
    root[key] = value;
}

Json::Value SettingsAPI::get(string key, Json::Value def) {
    if (root.isMember(key)) return root[key];
    return def;
}

void SettingsAPI::unset(string key) {
    root.removeMember(key);
}

void SettingsAPI::clear() {
    root = Json::Value(Json::objectValue);
}

std::vector<string> SettingsAPI::getNames() {
    return root.getMemberNames();
}

bool SettingsAPI::load(string path) {
    std::ifstream in(path.c_str());
    in >> root;
    in.close();
}

bool SettingsAPI::save(string path) {
    std::ofstream out(path.c_str());
    out << root;
    out.close();
}