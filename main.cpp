#include <iostream>
#include <ncurses.h>

#include "generator.h"
#include "windows.h"
#include "bitmasks.h"
#include "engine.h"

bool is_empty(uint8_t *board);
void print_board(uint8_t *board);

int main(void) {
    // Allocate memory for board
    uint8_t *board = (uint8_t *)calloc(81, sizeof(uint8_t));
    if(board == NULL) {
        std::cout << "Allocation Failed\n";
    }
    generate_board(board);
    if((board[0] == '!') || is_empty(board)) {
        std::cout << "Board Generation Failed\n";
        free(board);
        return EXIT_FAILURE;
    }

    for(uint8_t index = 0; index < 81; index++) {
        if(board[index] != 0) {
            board[index] |= ORIGINAL_BITMASK;
        }
    }

    uint8_t game_over = 0;
    uint8_t usr;

    // Start nCurses
    initscr();
    // Enable color functionality
    start_color();
    // Disable line buffering so that the user does not need to use the enter key
    raw();
    // Disable user input echo
    noecho();
    // Disable cursor
    curs_set(0);

    // Create color pairs
    init_pair(1, COLOR_GREEN, COLOR_BLACK);     // Menu Text
    init_pair(2, COLOR_RED, COLOR_BLACK);       // Menu Text <Option Characters>
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);   // Colors for initial numbers
    init_pair(4, COLOR_BLUE, COLOR_WHITE);      // Colors for row/col selection
    init_pair(5, COLOR_BLACK, COLOR_YELLOW);    // Colors for row & col intersection
    init_pair(6, COLOR_WHITE, COLOR_BLACK);     // Board Colors

    refresh();

    // Define windows
    WINDOW *main_window = create_main_window();
    WINDOW *message_window = create_message_window();
    WINDOW *game_window = create_board_window(board);

    
    while(!game_over) {
        usr = getch();
        switch(usr) {
            case 'q':
            case 'Q':
                // Quit
                game_over = 1;
                break;
            case 'p':
            case 'P':
                // Place
                place_piece(board, message_window, game_window);
                if(game_solved(board)) {
                    game_over = 1;
                    mvwprintw(message_window, 1, 1, " Board Solved! Way To Go ");
                    mvwprintw(message_window, 2, 1, "      Press Any Key      ");
                    wrefresh(message_window);
                    getch();
                }
                break;
            case 'r':
            case 'R':
                // Remove
                remove_piece(board, message_window, game_window);
                break;
            default:
                break;
        }
    }

    mvwprintw(message_window, 1, 1, "  Thank You For Playing  ");
    mvwprintw(message_window, 2, 1, "  Press Any Key To Exit  ");
    wrefresh(message_window);
    getch();

    endwin();
    return EXIT_SUCCESS;
}

bool is_empty(uint8_t *board) {
    for(uint8_t index = 0; index < 81; index++) {
        if(board[index] != 0) {
            return false;
        }
    }

    return true;
}

void print_board(uint8_t *board) {
    for(uint8_t index = 0; index < 81; index++) {
        if(index != 0) {
            if(index%27 == 0) {
                std::cout << "\n\n";
            }else if(index%9 == 0) {
                std::cout << "\n";
            }else if(index%3 == 0) {
                std::cout << "  ";
            }
        }
        std::cout << +board[index] << ' ';
    }
    std::cout << "\n";
}