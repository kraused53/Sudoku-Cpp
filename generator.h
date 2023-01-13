#ifndef GENERATOR_H
    #define GENERATOR_H

    #include <iostream>
    // Used to generate random numbers
    #include <cstdlib>
    #include <ctime>

    #include "bitmasks.h"

    // Number of cells to clear when generating
    #define NUM_TO_REMOVE 30 

    // Function prototypes
    void generate_board(uint8_t *board);
    void open_board_file(uint8_t *board, char *path_to_file);
    void print_board(uint8_t *board);
#endif