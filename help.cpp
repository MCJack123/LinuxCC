#include "help.hpp"
#include "commands.hpp"
#include <sstream>

HelpAPI help;

string HelpAPI::path() {
    string prefix = (manPath == "" ? "" : "MANPATH='" + manPath + "' ");
    return commands.pexec(prefix + "man --path");
}

void HelpAPI::setPath(string path) {
    manPath = path;
}

string HelpAPI::lookup(string topic) {
    string prefix = (manPath == "" ? "" : "MANPATH='" + manPath + "' ");
    return commands.pexec(prefix + "man '" + topic + "' | cat");
}

std::vector<string> HelpAPI::topics() {
    string prefix = (manPath == "" ? "" : "MANPATH='" + manPath + "' ");
    string entries = commands.pexec(prefix + "man -k . | awk '{print $1}'");
    std::stringstream ss(entries);
    std::vector<string> retval;
    string s;
    while (std::getline(ss, s)) retval.push_back(s);
    return retval;
}

std::vector<string> HelpAPI::completeTopic(string topicPrefix) {
    string prefix = (manPath == "" ? "" : "MANPATH='" + manPath + "' ");
    string entries = commands.pexec(prefix + "man -k '^" + topicPrefix + "' | grep '^" + topicPrefix + "' | awk '{print $1}'");
    std::stringstream ss(entries);
    std::vector<string> retval;
    string s;
    while (std::getline(ss, s)) retval.push_back(s);
    return retval;
}