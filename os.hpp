#ifndef OSAPI_HPP
#define OSAPI_HPP
#include <string>
#include <vector>
#include <list>
#include <map>
#include <typeinfo>

using std::string;

enum {
    PARAM_TYPE_NIL = 0,
    PARAM_TYPE_BOOL,
    PARAM_TYPE_INT,
    PARAM_TYPE_DOUBLE,
    PARAM_TYPE_STRING
};

class event_t {
public:
    string event;
    int types[5];
    void* values[5];
    event_t() {}
    event_t(string ev): event(ev) {}
    bool getBool(int p);
    int getInt(int p);
    double getDouble(int p);
    string getString(int p);
    event_t setBool(int p, bool v);
    event_t setInt(int p, int v);
    event_t setDouble(int p, double v);
    event_t setString(int p, string v);
    template <typename T>
    event_t setValue(int p, T v);
};

class OSAPI {
    string label;
    std::list<event_t> events;
    std::map<int, time_t> timers;
    std::map<int, double> alarms;
    void sighandler(int sig);
public:
    OSAPI();
    string version() {return "LinuxCC 1.8";}
    int getComputerID() {return 0;}
    string getComputerLabel() {return label;}
    void setComputerLabel(string l) {label = l;}
    bool run(std::map<string, string> env, string path, string args = "");
    event_t pullEvent(string targetEvent = "");
    event_t pullEventRaw(string targetEvent = "");
    template<typename ... Args>
    void queueEvent(string event, Args... params);
    void queueEvent(event_t ev);
    int clock();
    int startTimer(int timeout);
    void cancelTimer(int id);
    double time();
    void sleep(int timeout);
    int day();
    int setAlarm(double time);
    int cancelAlarm(int id);
    void shutdown();
    void reboot();
    void runLoop();
};

extern OSAPI os;

#endif