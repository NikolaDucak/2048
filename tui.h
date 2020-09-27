#ifndef __SFB_TUI_NCURSES_H__
#define __SFB_TUI_NCURSES_H__

#include <ncurses.h>
#include <string>

/*
 * A wrapper around ncurses lib functionality for more readable names &
 * some additional features such as std::string for const char * args
 */
namespace tui {

using uint = unsigned int;

struct point {
    int x, y;
};

void initTerminal();
void endTerminal();
point termSize();

int input();
void render();
void clear();

void printAt(uint x, uint y, const std::string& str);
void colorPrintAt(int index, uint x, uint y, const std::string& str);
void clearLine(int py);

void setColorPair(int index, int fg, int bg);

}  // namespace tui

#endif
