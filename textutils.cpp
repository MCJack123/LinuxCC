#include "textutils.hpp"
#include "term.hpp"
#include "math.hpp"
#include "os.hpp"
#include <unistd.h>
#include <typeinfo>
#include <sstream>
#include <iomanip>

TextutilsAPI textutils;

int tabulate(std::vector<string> v, int x, int y) {
    int max = 0;
    for (int i = 0; i < v.size() && i < y + term.getSize().second; i++) {
        term.setCursorPos(x, y + i);
        term.write(v[i]);
        if (v[i].size() > max) max = v[i].size();
    }
    return x + max + 1;
}

void TextutilsAPI::slowWrite(string text, int rate) {
    for (char c : text) {
        term.write(string(&c, 1));
        usleep(1000000 / rate);
    }
}

string TextutilsAPI::formatTime(double time, bool hour24) {
    int hour = math.floor(time);
    if (hour < 6) hour += 18;
    else hour -= 6;
    int minute = ((int)(time * 1000) % 1000) / (1000.0/60.0);
    if (hour24) return string(std::to_string(hour) + ":" + std::to_string(minute));
    else {
        bool pm = hour >= 12;
        int newHour = hour % 12;
        if (hour == 0) hour = 12;
        return string(std::to_string(newHour) + ":" + std::to_string(minute) + (pm ? " PM" : " AM"));
    }
}

template <typename T, typename ... Args>
void TextutilsAPI::tabulate(T first, Args... args) {
    if (typeid first == typeid int) term.setTextColor(first);
    else {
        int y = term.getCursorPos().second;
        _tabulate(::tabulate(first, 0, y), y, args...);
    }
}

template<typename T, typename ... Args>
void _tabulate(int x, int y, T first, Args... args) {
    int next = x;
    if (typeid first == typeid int) term.setTextColor(first);
    else next = ::tabulate(first, x, y);
    _tabulate(next, y, args...);
}

template<typename T>
void _tabulate(int x, int y, T first) {
    if (typeid first == typeid int) term.setTextColor(first);
    else ::tabulate(first, x, y);
}

template <typename T, typename ... Args>
void TextutilsAPI::pagedTabulate(T first, Args... args) {
    // soon(tm)
}

int TextutilsAPI::pagedPrint(string text, int freeLines) {
    std::stringstream ss(text);
    string s;
    int lines;
    for (lines = 0; lines < freeLines; lines++) {
        if (!std::getline(ss, s)) return lines;
        print(s);
    }
    while (true) {
        if (!std::getline(ss, s)) return lines;
        print(s);
        lines++;
        if (!std::getline(ss, s)) return lines;
        print(s);
        lines++;
        term.write("Press any key to continue");
        os.pullEvent("key");
        term.clearLine();
    }
}

template <typename T>
string TextutilsAPI::serialize(T value) {
    Json::Value v(value);
    std::stringstream ss;
    ss << v;
    return ss.str();
}

Json::Value TextutilsAPI::unserialize(string value) {
    Json::Value v;
    std::stringstream ss(value);
    ss >> v;
    return v;
}

string TextutilsAPI::urlEncode(string value) {
    // stolen shamelessly from stackoverflow (credit to xperroni)
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char) c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}