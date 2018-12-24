#ifndef TERMAPI_HPP
#define TERMAPI_HPP
#include <string>
#include <utility>
#include <ncurses.h>
#include "colors.hpp"
#undef scroll

using std::string;

class Terminal {
protected:
    friend class WindowAPI;
    WINDOW * win = stdscr;
    int textColor = colors.white;
    int backgroundColor = colors.black;
public:
    Terminal();
    void write(string text) {wprintw(win, text.c_str()); wrefresh(win);}
    void blit(string text, string fg, string bg);
    void clear() {wclear(win); wrefresh(win);}
    void clearLine();
    std::pair<int, int> getCursorPos();
    void setCursorPos(int x, int y) {wmove(win, y, x); wrefresh(win);}
    void setCursorBlink(bool blink) {curs_set(blink); wrefresh(win);}
    bool isColor() {return has_colors();}
    std::pair<int, int> getSize() {return std::make_pair(COLS, LINES);}
    void scroll(int lines) {wscrl(win, lines); wrefresh(win);}
    void setTextColor(int color);
    int getTextColor() {return textColor;}
    void setBackgroundColor(int color);
    int getBackgroundColor() {return backgroundColor;}
};

class TermAPI {
    Terminal * currentTerm;
public:
    TermAPI();
    ~TermAPI();
    void write(string text) {currentTerm->write(text);}
    void blit(string text, string fg, string bg) {currentTerm->blit(text, fg, bg);}
    void clear() {currentTerm->clear();}
    void clearLine() {currentTerm->clearLine();}
    std::pair<int, int> getCursorPos() {return currentTerm->getCursorPos();}
    void setCursorPos(int x, int y) {currentTerm->setCursorPos(x, y);}
    void setCursorBlink(bool blink) {currentTerm->setCursorBlink(blink);}
    bool isColor() {return currentTerm->isColor();}
    std::pair<int, int> getSize() {return currentTerm->getSize();}
    void scroll(int lines) {currentTerm->scroll(lines);}
    Terminal redirect(Terminal target) {
        Terminal old = *currentTerm;
        currentTerm = &target;
        return old;
    }
    Terminal * current() {return currentTerm;}
    Terminal * native();
    void setTextColor(int color) {currentTerm->setTextColor(color);}
    int getTextColor() {return currentTerm->getTextColor();}
    void setBackgroundColor(int color) {currentTerm->setBackgroundColor(color);}
    int getBackgroundColor() {return currentTerm->getBackgroundColor();}
};

extern TermAPI term;
extern void write(string text);
extern void print(string text);

#endif