#include "Game.h"
#include "tui.h"

// a struct that holds bottom right coords of term;
static tui::point termSize;
// width of each cell in chars
static constexpr int cellW = 6;
// width & height of board
static constexpr int boardDim = cellW * 3;
// Height of row of cells in lines
static constexpr int rowH = boardDim / 3;
// top left of game board on terminal
// calculated later
static int boardX = 0;
static int boardY = 0;

/*
 * for a given value of cell (eg. 2, 4, .., 2048) return ncurses index
 * of color pair for that value.
 */
int getColorPairForNum(int num);

/*
 * Print game board.
 */
void printBoard(const Game& g);

/*
 * Sets up nccurses, colors for cells, and termSize.
 */
void setupTerminal();

/*
 * A loop that processes input & prints game board
 * until game over.
 */
void gameLoop(Game& g);

/*
 * Replaying a game after game over until player doesn't want to.
 */
void replayLoop(Game& g);

/*
 * Calculate position coords of the game board on the terminal.
 */
void calculateTermSize();

int main() {
    static Game g;

    setupTerminal();
    replayLoop(g);
    return 0;
}

void setupTerminal() {
    tui::initTerminal();
    atexit(tui::endTerminal);
    calculateTermSize();

    // cell collors
    tui::setColorPair(20, COLOR_BLACK,  -1);           // 0
    tui::setColorPair(1, COLOR_YELLOW,  -1);           // 2
    tui::setColorPair(2, COLOR_YELLOW,  -1);           // 4
    tui::setColorPair(3, COLOR_CYAN,    -1);           // 8
    tui::setColorPair(4, COLOR_GREEN,   -1);           // 16
    tui::setColorPair(5, COLOR_BLUE,    -1);           // 32
    tui::setColorPair(6, COLOR_MAGENTA, -1);           // 64
    tui::setColorPair(7, COLOR_RED,     -1);           // 128
    tui::setColorPair(8, COLOR_WHITE,   -1);           // 256
    tui::setColorPair(9, COLOR_WHITE,  COLOR_BLACK);   // 512
    tui::setColorPair(10, COLOR_GREEN, COLOR_BLACK) ;  // 1024
    tui::setColorPair(11, COLOR_BLUE,  COLOR_BLACK);   // 2048
}

void gameLoop(Game& g) {
    while (not g.gameOver()) {
        printBoard(g);
        tui::render();
        switch (tui::input()) {
            case 'Q': std::exit(0);
            case 'R': g.resetBoard(); break;

            case 'w':
            case 'k':
            case KEY_UP: g.shiftUp(); break;

            case 's':
            case 'j':
            case KEY_DOWN: g.shiftDown(); break;

            case 'a':
            case 'h':
            case KEY_LEFT: g.shiftLeft(); break;

            case 'd':
            case 'l':
            case KEY_RIGHT: g.shiftRight(); break;

            case 'r':
            case KEY_RESIZE:
                calculateTermSize();
                tui::clear();
                break;
        }
    }
}

void replayLoop(Game& g) {
    static const std::string msg { "Game Over! Play again? (y/n)" };
    while (true) {
        gameLoop(g);
        tui::printAt(termSize.x / 2 - msg.size() / 2, 2, msg);
    }
}

void printBoard(const Game& g) {
    int py = boardY;
    for (int y = 0; y < 4; y++) {
        int px = boardX;
        tui::clearLine(py);
        for (int x = 0; x < 4; x++) {
            auto c = getColorPairForNum(g.at(x, y));
            tui::colorPrintAt(c, px, py, std::to_string(g.at(x, y)));
            px += cellW;  // move to position of next cell
        }
        py += rowH;  // move to position of next row
    }
}

int getColorPairForNum(int num) {
    static int colorIndicies[][2] = {
        { 2, 1 },   { 4, 2 },     { 8, 3 },     { 16, 4 },
        { 32, 5 },  { 64, 6 },    { 128, 7 },   { 256, 8 },
        { 512, 9 }, { 1024, 10 }, { 2048, 11 },
    };

    if (num == 0)
        return 20;
    if (num > 2048)
        return 11;

    for (int i = 0; i < 10; i++)
        if (colorIndicies[i][0] == num)
            return colorIndicies[i][1];

    return 0;
}

void calculateTermSize() {
    termSize = tui::termSize();
    boardY   = termSize.y / 2 - boardDim / 2;
    boardX   = termSize.x / 2 - boardDim / 2;
}
