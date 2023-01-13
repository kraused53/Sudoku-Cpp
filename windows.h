#ifndef WINDOWS_H
    #define WINDOWS_H

    #include <ncurses.h>
    #include "bitmasks.h"

    // Main Window Settings
    #define MAIN_WINDOW_HEIGHT      21
    #define MAIN_WINDOW_WIDTH       42
    #define MAIN_START_Y            (LINES - MAIN_WINDOW_HEIGHT) / 2
    #define MAIN_START_X            (COLS - MAIN_WINDOW_WIDTH) / 2
    // Message Window Settings
    #define MESSAGE_WINDOW_HEIGHT   4
    #define MESSAGE_WINDOW_WIDTH    27
    #define MESSAGE_START_Y         MAIN_START_Y + 2
    #define MESSAGE_START_X         MAIN_START_X + 7
    // Board Window Settings
    #define BOARD_WINDOW_HEIGHT     11
    #define BOARD_WINDOW_WIDTH      23
    #define BOARD_START_Y           MAIN_START_Y + 7
    #define BOARD_START_X           MAIN_START_X + 10

    /*** Function Prototypes ***/
    // Main Window
    WINDOW *create_main_window(void);
    // Message Window
    WINDOW *create_message_window(void);
    void clear_message_window(WINDOW *m);
    // Board Window
    WINDOW *create_board_window(uint8_t *board);
    void update_board_window(uint8_t *board, WINDOW *b);
#endif