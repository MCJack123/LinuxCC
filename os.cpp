#include "os.hpp"
#include "commands.hpp"
#include "coroutine.hpp"
#include "term.hpp"
#include "math.hpp"
#include <sys/sysinfo.h>
#include <ncurses.h>
#include <signal.h>

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

event_t event_t::setBool(int p, bool v) {
    types[p] = PARAM_TYPE_BOOL;
    values[p] = malloc(sizeof(bool));
    *(bool*)(values[p]) = v;
    return *this;
}

event_t event_t::setInt(int p, int v) {
    types[p] = PARAM_TYPE_INT;
    values[p] = malloc(sizeof(int));
    *(int*)(values[p]) = v;
    return *this;
}

event_t event_t::setDouble(int p, double v) {
    types[p] = PARAM_TYPE_DOUBLE;
    values[p] = malloc(sizeof(double));
    *(double*)(values[p]) = v;
    return *this;
}

event_t event_t::setString(int p, string v) {
    types[p] = PARAM_TYPE_STRING;
    values[p] = malloc(sizeof(string));
    *(string*)(values[p]) = v;
    return *this;
}

template <typename T>
event_t event_t::setValue(int p, T v) {
    types[p] = typeid(T).hash_code;
    values[p] = malloc(sizeof(T));
    *(T*)(values[p]) = v;
    return *this;
}

OSAPI::OSAPI() {
    signal(SIGINT, this->sighandler);
    signal(SIGWINCH, this->sighandler);
}

void OSAPI::sighandler(int sig) {
    if (sig == SIGINT) {
        queueEvent(event_t("terminate"));
    } else if (sig == SIGWINCH) queueEvent(event_t("term_resize"));
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
        if (retval.event == "terminate") {term.~TermAPI(); exit(0);}
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

int OSAPI::setAlarm(double time) {
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

#if NCURSES_MOUSE_VERSION > 1
#define NCURSES_MOUSE_UNMASK(y,m) (math.log((64*y)/m)/5*math.log(2))
#else
#define NCURSES_MOUSE_UNMASK(y,m) (math.log((64*y)/m)/6*math.log(2))
#endif

void OSAPI::runLoop() {
    int lastch;
    while (true) {
        // key, key_up, char
        int ch = getch();
        if (ch != lastch && ch != KEY_MOUSE) {
            if (lastch != 0) queueEvent(event_t("key_up").setInt(0, lastch));
            if (ch != 0) {
                queueEvent(event_t("key").setInt(0, ch).setBool(1, false));
                if (ch >= ' ' && ch <= '~') queueEvent(event_t("char").setString(0, string((char*)&ch, 1)));
            }
        }
        // paste: incompatible
        // timer
        time_t now = ::time(0);
        std::vector<int> erase;
        for (std::map<int, time_t>::iterator it = timers.begin(); it != timers.end(); it++)
            if (it->second == now) {
                queueEvent(event_t("timer").setInt(0, it->first));
                erase.push_back(it->first);
            }
        for (int e : erase) timers.erase(e);
        // alarm
        double tnow = time();
        erase.clear();
        for (std::map<int, double>::iterator it = alarms.begin(); it != alarms.end(); it++) 
            if (it->second == tnow) {
                queueEvent(event_t("alarm").setInt(0, it->first));
                erase.push_back(it->first);
            }
        for (int e : erase) alarms.erase(e);
        // mouse
        if (ch == KEY_MOUSE) {
            MEVENT event;
            if (getmouse(&event) == OK) {
                if (event.bstate & NCURSES_BUTTON_CLICKED) 
                    queueEvent(event_t("mouse_click")
                        .setInt(0, NCURSES_MOUSE_UNMASK(event.bstate, NCURSES_BUTTON_CLICKED))
                        .setInt(1, event.x).setInt(2, event.y)
                    );
                else if (event.bstate & NCURSES_BUTTON_PRESSED) 
                    queueEvent(event_t("mouse_click")
                        .setInt(0, NCURSES_MOUSE_UNMASK(event.bstate, NCURSES_BUTTON_PRESSED))
                        .setInt(1, event.x).setInt(2, event.y)
                    );
                else if (event.bstate & NCURSES_BUTTON_RELEASED) 
                    queueEvent(event_t("mouse_up")
                        .setInt(0, NCURSES_MOUSE_UNMASK(event.bstate, NCURSES_BUTTON_RELEASED))
                        .setInt(1, event.x).setInt(2, event.y)
                    );
            }
        }
        // that should be it
        lastch = ch;
    }
}