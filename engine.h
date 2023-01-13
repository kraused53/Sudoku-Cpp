#ifndef ENGINE_H
    #define ENGINE_H

    #include <ncurses.h>

    #include "bitmasks.h"
    #include "windows.h"

    /*** Function prototypes ***/
    uint8_t game_solved(uint8_t *board);
    void place_piece(uint8_t *board, WINDOW *message_window, WINDOW *game);
    void remove_piece(uint8_t *board, WINDOW *message_window, WINDOW *game);
#endif