#ifndef HELPAPI_HPP
#define HELPAPI_HPP
#include <string>
#include <vector>

using std::string;

class HelpAPI {
    string manPath;
public:
    string path();
    void setPath(string path);
    string lookup(string topic);
    std::vector<string> topics();
    std::vector<string> completeTopic(string topicPrefix);
};

extern HelpAPI help;

#endif