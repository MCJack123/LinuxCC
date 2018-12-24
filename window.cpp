#include "window.hpp"

WindowAPI window;

void Window::reposition(int x, int y, int width, int height) {
    mvwin(win, y, x);
    if (width != -1) {
        if (height != -1) wresize(win, height, width);
        else wresize(win, getmaxy(win) - getbegy(win), width);
    } else if (height != -1) wresize(win, height, getmaxx(win) - getbegx(win));
}

Window WindowAPI::create(Terminal parentTerm, int x, int y, int width, int height, bool visible) {
    Window w(getbegx(parentTerm.win) + x, getbegy(parentTerm.win) + y, width, height);
    w.setVisible(visible);
    return w;
}