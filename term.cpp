#include "term.hpp"
#include "math.hpp"

TermAPI term;
Terminal native;
Terminal * TermAPI::native() {return &::native;}
void write(string text) {term.write(text);}
void print(string text) {term.write(text + "\n");}

Terminal::Terminal() {
    if (has_colors()) {
        start_color();
        init_color(14, 1000, 500, 0);
    }
}

void Terminal::blit(string text, string fg, string bg) {
    int len = math.min(text.size(), fg.size(), bg.size());
    for (int i = 0; i < len; i++) {
        setTextColor(fg[i]);
        setBackgroundColor(bg[i]);
        write(text.substr(i, 1));
    }
}

void Terminal::clearLine() {
    setCursorPos(getCursorPos().second, 0);
    wclrtoeol(win);
    wrefresh(win);
}

std::pair<int, int> Terminal::getCursorPos() {
    std::pair<int, int> retval;
    getyx(win, retval.second, retval.first);
    return retval;
}

void Terminal::setTextColor(int color) {
    init_pair((colors.ansiMap[backgroundColor] << 4) | colors.ansiMap[color], colors.ansiMap[color], colors.ansiMap[backgroundColor]);
    wattroff(win, COLOR_PAIR((colors.ansiMap[backgroundColor] << 4) | colors.ansiMap[textColor]));
    wattron(win, COLOR_PAIR((colors.ansiMap[backgroundColor] << 4) | colors.ansiMap[color]));
    textColor = color;
}

void Terminal::setBackgroundColor(int color) {
    init_pair((colors.ansiMap[color] << 4) | colors.ansiMap[textColor], colors.ansiMap[textColor], colors.ansiMap[color]);
    wattroff(win, COLOR_PAIR((colors.ansiMap[backgroundColor] << 4) | colors.ansiMap[textColor]));
    wattron(win, COLOR_PAIR((colors.ansiMap[color] << 4) | colors.ansiMap[textColor]));
    backgroundColor = color;
}

TermAPI::TermAPI() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    currentTerm = &::native;
}

TermAPI::~TermAPI() {
    echo();
    nodelay(stdscr, FALSE);
    keypad(stdscr, FALSE);
    endwin();
}