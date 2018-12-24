#ifndef TEXTUTILSAPI_HPP
#define TEXTUTILSAPI_HPP
#include <string>
#include <vector>
#include <jsoncpp/json/json.h>

using std::string;

class TextutilsAPI {
public:
    void slowWrite(string text, int rate = 20);
    void slowPrint(string text, int rate = 20) {slowWrite(text + "\n", rate);}
    string formatTime(double time, bool hour24 = false);
    template <typename T, typename ... Args>
    void tabulate(T first, Args... args);
    template <typename T, typename ... Args>
    void pagedTabulate(T first, Args... args);
    int pagedPrint(string text, int freeLines = 0);
    template <typename T>
    string serialize(T value);
    Json::Value unserialize(string value);
    template<typename T>
    string serializeJson(T value, bool unquote = false) {return serialize(value);}
    string urlEncode(string value);
};

extern TextutilsAPI textutils;

#endif