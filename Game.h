#ifndef __2048_GAME_H__
#define __2048_GAME_H__

#include <stdlib.h>

/*
 * Game class containing state and all the logic needed for 2048 game
 *
 * Game initalization:
 *  1. set whole board to all zeros
 *  2. place 2 or 4 on a random place
 *
 * Game Loop:
 * 	1. player shifts cells
 * 	2. if there are one or more free spaces, put 2 or 4 on a random free
 *     space
 * 	3. check if any movement or merging of cells is possible 
 * 	   (if not, game over)
 * 	4. repeat
 */
class Game {
public:
    Game();

    bool gameOver() const;
    int  at(uint x, uint y) const;
    void resetBoard();

    void shiftLeft();
    void shiftRight();
    void shiftUp();
    void shiftDown();

private:
    static constexpr int LEFT  = -1;  // decrease x
    static constexpr int RIGHT = 1;   // increase x
    static constexpr int UP    = -1;  // decrease y
    static constexpr int DOWN  = 1;   // increase y

    /*
     * any shifting of cells is achieved with 2 actions, move & merge
     * move:
     * 		- moves all cells as far to dir as possible
     * 		  eg. [0,2,0,2] -> moveLeft -> [2,2,0,0]
     * merge:
     * 		- merges cells with the same value neighbouring to dir
     * 		  eg. [0,2,2,0] -> mergeLeft-> [0,4,0,0];
     *
     * shifting algorithm:
     * 		move  - move all cells in one direction as far as possible so
     * they are all neigbouring merge - merge same value neighbours if merge
     * happend, move again, (because in case of mergin 2 cels become 1 and some
     * gaps may exist)
     *
     * this way we solve problems such as :  [2,2,4,x] shiftin left becomes 
     * [8,x,x,x], when in most common * implementation is [4,4,x,x] aka new 
     * cells (higher value cells created in * shifts) should not merge with 
     * cells with same value that have existed * before the shift 
     * action * eg: 2,2,4,x -> shift left  -> 4,4,x,x 2,2,4,x ->
     * shift right -> x,x,4,4 2,2,4,x -> shift right -> x,x,4,4
     */
    bool moveHorizontaly  (int dir);
    bool moveVerticaly    (int dir);
    bool mergeHorizontaly (int dir);
    bool mergeVerticaly   (int dir);

    /*
     * game is over when no shifting in no direction can't happen
     */
    void gameOverCheck();

    /*
     * choose a free space to put a 2 or a 4
     */
    void placeRandomNum();

    bool gameOver_;
    short board_[4][4];
};

#endif
