#include "Game.h"

#include <random>

/*==================================================================================================
 *										HELPERS
 *=================================================================================================*/

/*
 * Select random item start - end range
 */
namespace {
template <typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template <typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

short randVal() { return (random() % 4) < 3 ? 2 : 4; }

struct point {
    int x, y;
};

}  // anonymous namespace

Game::Game() { resetBoard(); }

int Game::at(uint x, uint y) const { return board_[x][y]; }

bool Game::gameOver() const { return gameOver_; }

void Game::resetBoard() {
    // reset each value
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            board_[i][j] = 0;

    placeRandomNum();
    gameOver_ = false;
}

void Game::gameOverCheck() {
    bool moovementPossbile = false;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            // if any cell is 0 that means a shift is possible
            if (board_[x][y] == 0) {
                moovementPossbile = true;
                break;
            }

            // if any two neigbhouring cells have the same non-zero value
            // they can be merged sa a shift is possible
            if (y != 3)  // no point in checking down if were at the bottom
                if (board_[x][y] == board_[x][y + 1]) {
                    moovementPossbile = true;
                    break;
                }
            if (x !=
                3)  // no point in checking right if we're at the right edge
                if (board_[x][y] == board_[x + 1][y]) {
                    moovementPossbile = true;
                    break;
                }
        }
        if (moovementPossbile)
            break;
    }
    gameOver_ = !moovementPossbile;
}

void Game::placeRandomNum() {
    static std::vector<::point> freeSpaces;
    freeSpaces.clear();
    // collect coordinates of all free spaces
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (board_[x][y] == 0)
                freeSpaces.push_back({ x, y });

    if (freeSpaces.empty())
        return;

    // select random pair of coordinates & place a random 2 or 4 value there
    auto p           = *select_randomly(freeSpaces.begin(), freeSpaces.end());
    board_[p.x][p.y] = randVal();
}

/*==================================================================================================
 *										MOOVING
 *=================================================================================================*/

bool Game::moveHorizontaly(int dir) {
    bool moovementHappend = false;

    /*
     * if direction is left,
     * we start from x = 1 (one from far left edge),
     * going to right most edge x = 3,
     * so the step is +1 aka -dir
     * and when moving a cell to the leftdd
     *
     * if direction is right
     * we start from x = 2 (one from far right edge),
     * going to left most edge x = 0,
     * so the step is -1 aka value of dir arg
     */
    int s    = (dir == LEFT) ? 1 : 2;
    int end  = (dir == LEFT) ? 4 : -1;
    int oend = (dir == LEFT) ? 0 : 3;

    for (int y = 0; y < 4; y++)
        for (int x = s; x != end; x += -dir)  // if left go to right
        {
            if (board_[x][y] != 0 && board_[x + dir][y] == 0) {
                // If left go to left.
                // Because loop checks for nx+dir index of array
                // end of this loop (oend) cant be like regular end and must not
                // contain invalid index, so if nx is 0 or 3 nx+dir is -1 or 4,
                // therefore loop only while nx is not 0 or 3;
                for (int nx = x; nx != oend && board_[nx + dir][y] == 0;
                     nx += dir) {
                    moovementHappend    = true;
                    board_[nx + dir][y] = board_[nx][y];
                    board_[nx][y]       = 0;
                }
            }
        }

    return moovementHappend;
}

bool Game::moveVerticaly(int dir) {
    // works the same way as moveHorizontaly but on y axis
    bool moovementHappend = false;

    int s    = (dir == UP) ? 1 : 2;
    int end  = (dir == UP) ? 4 : -1;
    int oend = (dir == UP) ? 0 : 3;

    for (int x = 0; x < 4; x++)
        for (int y = s; y != end; y -= dir) {
            if (board_[x][y] != 0 && board_[x][y + dir] == 0) {
                for (int ny = y; ny != oend && board_[x][ny + dir] == 0;
                     ny += dir) {
                    moovementHappend = true;
                    board_[x][ny + dir] += board_[x][ny];
                    board_[x][ny] = 0;
                }
            }
        }

    return moovementHappend;
}

/*==================================================================================================
 *										MERGING
 *=================================================================================================*/
bool Game::mergeHorizontaly(int dir) {
    bool mergeHappend = false;

    int s   = (dir == LEFT) ? 1 : 2;
    int end = (dir == LEFT) ? 4 : -1;

    // from top to botom
    for (int y = 0; y < 4; y++)
        for (int x = s; x != end; x -= dir) {
            // if neigbouring cells have value & that value is same, merge them
            // to left cell
            if (board_[x][y] != 0 && board_[x + dir][y] == board_[x][y]) {
                mergeHappend = true;
                board_[x + dir][y] *= 2;
                board_[x][y] = 0;
            }
        }
    return mergeHappend;
}

bool Game::mergeVerticaly(int dir) {
    bool mergeHappend = false;

    int s   = (dir == UP) ? 1 : 2;
    int end = (dir == UP) ? 4 : -1;

    for (int x = 0; x < 4; x++)
        for (int y = s; y != end; y -= dir) {
            // if neigbouring cells have value & that value is same, merge them
            // to left cell
            if (board_[x][y] != 0 && board_[x][y + dir] == board_[x][y]) {
                mergeHappend = true;
                board_[x][y + dir] *= 2;
                board_[x][y] = 0;
            }
        }
    return mergeHappend;
}

/*==================================================================================================
 *										SHIFTING
 *=================================================================================================*/

void Game::shiftLeft() {
    bool movementHappend = moveHorizontaly(LEFT);
    bool mergeHappend    = mergeHorizontaly(LEFT);
    bool changesHappend  = (movementHappend || mergeHappend);

    // in case of a merge, gaps are possible
    // so this makes sure no gaps are left
    // eg. [2,2,2,2] -> merge left -> [4,0,4,0];
    if (mergeHappend)
        moveHorizontaly(LEFT);

    // if action caused movement or merge
    // place a random 2 or 4 on a random free space and check for gameOver
    if (changesHappend) {
        placeRandomNum();
        gameOverCheck();
    }
}

void Game::shiftRight() {
    bool movementHappend = moveHorizontaly(RIGHT);
    bool mergeHappend    = mergeHorizontaly(RIGHT);
    bool changesHappend  = (movementHappend || mergeHappend);

    if (mergeHappend)
        moveHorizontaly(RIGHT);

    if (changesHappend) {
        placeRandomNum();
        gameOverCheck();
    }
}

void Game::shiftUp() {
    bool movementHappend = moveVerticaly(UP);
    bool mergeHappend    = mergeVerticaly(UP);
    bool changesHappend  = (movementHappend || mergeHappend);

    if (mergeHappend)
        moveVerticaly(UP);

    if (changesHappend) {
        placeRandomNum();
        gameOverCheck();
    }
}

void Game::shiftDown() {
    bool movementHappend = moveVerticaly(DOWN);
    bool mergeHappend    = mergeVerticaly(DOWN);
    bool changesHappend  = (movementHappend || mergeHappend);

    if (mergeHappend)
        moveVerticaly(DOWN);

    if (changesHappend) {
        placeRandomNum();
        gameOverCheck();
    }
}

