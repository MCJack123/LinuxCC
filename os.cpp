#include "os.hpp"
#include "commands.hpp"
#include "coroutine.hpp"
#include <sys/sysinfo.h>

OSAPI os;

bool event_t::getBool(int p) {
    if (types[p] != PARAM_TYPE_BOOL) throw std::invalid_argument((string("Event ") + event + " parameter " + std::to_string(p) + " is not a bool").c_str());
    return *(bool*)values[p];
}

int event_t::getInt(int p) {
    if (types[p] != PARAM_TYPE_INT) throw std::invalid_argument((string("Event ") + event + " parameter " + std::to_string(p) + " is not an int").c_str());
    return *(int*)values[p];
}

double event_t::getDouble(int p) {
    if (types[p] != PARAM_TYPE_DOUBLE) throw std::invalid_argument((string("Event ") + event + " parameter " + std::to_string(p) + " is not a double").c_str());
    return *(double*)values[p];
}

string event_t::getString(int p) {
    if (types[p] != PARAM_TYPE_STRING) throw std::invalid_argument((string("Event ") + event + " parameter " + std::to_string(p) + " is not a string").c_str());
    return *(string*)values[p];
}

bool OSAPI::run(std::map<string, string> env, string path, string args = "") {
    string exports = "";
    for (std::map<string, string>::iterator it = env.begin(); it != env.end(); it++)
        exports += "export " + it->first + "=" + it->second + "; ";
    return commands.exec(exports + path + " " + args) != 127;
}

event_t OSAPI::pullEvent(string targetEvent = "") {
    event_t retval;
    do {
        while (events.size() < 1) coroutine.yield();
        retval = events.front();
        if (retval.event == "terminate") exit(0);
        events.pop_front();
    } while (targetEvent != "" || retval.event != targetEvent);
    return retval;
}

event_t OSAPI::pullEventRaw(string targetEvent = "") {
    event_t retval;
    do {
        while (events.size() < 1) coroutine.yield();
        retval = events.front();
        events.pop_front();
    } while (targetEvent != "" || retval.event != targetEvent);
    return retval;
}

template<typename T, typename ... Args>
void pushParams(event_t ev, int start, T first, Args... args) {
    switch (typeid T) {
        case typeid bool:
            ev.types[start] = PARAM_TYPE_BOOL;
            break;
        case typeid int:
            ev.types[start] = PARAM_TYPE_INT;
            break;
        case typeid double:
            ev.types[start] = PARAM_TYPE_DOUBLE;
            break;
        case typeid string:
            ev.types[start] = PARAM_TYPE_STRING;
            break;
        default:
            ev.types[start] = (typeid T).hash_code();
            break;
    }
    ev.values[start] = (void*)&first;
    pushParams(ev, start + 1, args...);
}

template<typename T>
void pushParams(event_t ev, int start, T first) {
    switch (typeid T) {
        case typeid bool:
            ev.types[start] = PARAM_TYPE_BOOL;
            break;
        case typeid int:
            ev.types[start] = PARAM_TYPE_INT;
            break;
        case typeid double:
            ev.types[start] = PARAM_TYPE_DOUBLE;
            break;
        case typeid string:
            ev.types[start] = PARAM_TYPE_STRING;
            break;
        default:
            ev.types[start] = (typeid T).hash_code();
            break;
    }
    ev.values[start] = (void*)&first;
}

template<typename ... Args>
void OSAPI::queueEvent(string event, Args... params) {
    event_t ev;
    pushParams(ev, 0, params...);
    events.push_back(ev);
}

void OSAPI::queueEvent(event_t ev) {
    events.push_back(ev);
}

int OSAPI::clock() {
    return ::clock();
}

int OSAPI::startTimer(int timeout) {
    int i = 0;
    for (; timers.find(i) != timers.end(); i++) ;
    timers[i] = ::time(0) + timeout;
    return i;
}

void OSAPI::cancelTimer(int id) {
    if (timers.find(id) != timers.end()) timers.erase(id);
}

double OSAPI::time() {
    double dtime = (::time(0) % 24*60*60) / 3.6;
    if (dtime >= 18000) dtime -= 18000;
    else dtime += 6000;
    return dtime / 1000;
}

void OSAPI::sleep(int timeout) {
    int t = startTimer(timeout);
    event_t ev = pullEvent("timer");
    while (ev.getInt(0) != t) ev = pullEvent("timer");
}

int OSAPI::day() {
    time_t t = ::time(0);
    struct tm * tm = localtime(&t);
    return tm->tm_yday;
}

int OSAPI::setAlarm(int time) {
    int i = 0;
    for (; alarms.find(i) != alarms.end(); i++) ;
    alarms[i] = time;
    return i;
}

int OSAPI::cancelAlarm(int id) {
    if (alarms.find(id) != alarms.end()) alarms.erase(id);
}

void OSAPI::shutdown() {
    exit(0);
}

void OSAPI::reboot() {
    exit(0);
}