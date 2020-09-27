#include "tui.h"

#include <sstream>
#include <stdexcept>

namespace tui {

void initTerminal() {
    initscr();
    noecho();
    curs_set(0);
    raw();
    if (!has_colors())
        throw std::runtime_error("no color support");
    start_color();
    use_default_colors();
}

void endTerminal() { endwin(); }

void printAt(uint x, uint y, const std::string& str) {
    mvprintw(y, x, str.c_str());
}

void colorPrintAt(int index, uint x, uint y, const std::string& str) {
    attron(COLOR_PAIR(index));
    mvprintw(y, x, str.c_str());
    attroff(COLOR_PAIR(index));
}

void clear() { erase(); }

void clearLine(int py) {
    ::move(py, 0);
    clrtoeol();
}

point termSize() {
    point p;
    getmaxyx(stdscr, p.y, p.x);
    return p;
}

void render() { refresh(); }

int input() { return getch(); }

void setColorPair(int index, int fg, int bg) { init_pair(index, fg, bg); }

}  // namespace tui
