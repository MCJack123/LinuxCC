#ifndef WINDOWAPI_HPP
#define WINDOWAPI_HPP
#include "term.hpp"

class Window : Terminal {
    std::pair<int, int> winCursor;
public:
    Window(int x, int y, int width, int height) {win = newwin(y, x, height, width);}
    void setCursorPos(int x, int y) {wmove(win, y, x); wrefresh(win); winCursor = std::make_pair(x, y);}
    std::pair<int, int> getCursorPos() {return winCursor;}
    void setVisible(bool visible) {} // broken for now
    void redraw() {redrawwin(win); wrefresh(win);}
    void restoreCursor() {wmove(win, winCursor.second, winCursor.first); wrefresh(win);}
    std::pair<int, int> getPosition() {return std::make_pair(getbegx(win), getbegy(win));}
    void reposition(int x, int y, int width = -1, int height = -1);
};

class WindowAPI {
public:
    Window create(Terminal parentTerm, int x, int y, int width, int height, bool visible = true);
};

extern WindowAPI window;

#endif