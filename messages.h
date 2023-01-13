#ifndef MESSAGES_H
    #define MESSAGES_H

    #include <ncurses.h>

    // Define messages
    #define WELCOME_MESSAGE     "    Welcome To Sudoku    "
    #define SELECTION_MESSAGE   " Please Make A Selection "
    #define EMPTY_MESSAGE       "                         "

    /*** Function Prototypes ***/
    void clear_message_window(WINDOW *m);
#endif