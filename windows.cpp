#include "windows.h"

WINDOW *create_new_window(int h, int w, int sy, int sx, int bdr);

/*** <--- MAIN WINDOW FUNCTIONS ---> ***/
/*
 *
 * Name:
 *      create_main_window
 * 
 * Inputs:
 *      None
 * 
 * Outputs:
 *      WINDOW *
 *          This function returns a pointer to the newly created main window.
 * 
 * Description:
 *        This function creates a new nCurses window that will be used as the 
 *      canvas for the rest of the program.
 * 
 */
WINDOW *create_main_window(void) {
    WINDOW *local_main = create_new_window(
        MAIN_WINDOW_HEIGHT,
        MAIN_WINDOW_WIDTH,
        MAIN_START_Y,
        MAIN_START_X,
        1
    );

    mvwprintw(local_main, 0,17,"<Sudoku>");
    mvwprintw(local_main, 20, 4, "<(");
    wattron(local_main, COLOR_PAIR(2));
    wprintw(local_main, "Q");
    wattroff(local_main, COLOR_PAIR(2));
    wprintw(local_main, ")uit>");
    mvwprintw(local_main, 20, 15, "<(");
    wattron(local_main, COLOR_PAIR(2));
    wprintw(local_main, "P");
    wattroff(local_main, COLOR_PAIR(2));
    wprintw(local_main, ")lace>");
    mvwprintw(local_main, 20, 28, "<(");
    wattron(local_main, COLOR_PAIR(2));
    wprintw(local_main, "R");
    wattroff(local_main, COLOR_PAIR(2));
    wprintw(local_main, ")emove>");

    wrefresh(local_main);
    return local_main;
}


/*** <--- MESSAGE WINDOW FUNCTIONS ---> ***/
WINDOW *create_message_window(void) {
    WINDOW *local_messages = create_new_window(
        MESSAGE_WINDOW_HEIGHT,
        MESSAGE_WINDOW_WIDTH,
        MESSAGE_START_Y,
        MESSAGE_START_X,
        2
    );

    mvwprintw(local_messages, 1, 1, "    Welcome To Sudoku    ");
    mvwprintw(local_messages, 2, 1, " Please Make A Selection ");

    wrefresh(local_messages);
    return local_messages;
}


/*
 *
 * Name:
 *      clear_message_window
 * 
 * Inputs:
 *      WINDOW *message_window
 *          A pointer to the message window.
 * 
 * Outputs:
 *      None
 *          
 * Description:
 *      This function clears the message window.
 * 
 */
void clear_message_window(WINDOW *m) {
    mvwprintw(m, 1, 1, "                         ");
    mvwprintw(m, 2, 1, "                         ");
}

/*** <--- BOARD WINDOW FUNCTIONS ---> ***/
/*
 *
 * Name:
 *      create_board_window
 * 
 * Inputs:
 *      None
 * 
 * Outputs:
 *      WINDOW *
 *          This function returns a pointer to the newly created board window.
 * 
 * Description:
 *        This function creates a new nCurses window that will display the game board.
 * 
 */
WINDOW *create_board_window(uint8_t *board) {
    WINDOW *local_board = create_new_window(
        BOARD_WINDOW_HEIGHT,
        BOARD_WINDOW_WIDTH,
        BOARD_START_Y,
        BOARD_START_X,
        0
    );

    update_board_window(board, local_board);
    wrefresh(local_board);
    return local_board;
}

void update_board_window(uint8_t *board, WINDOW *b) {
    wmove(b, 0, 0);

    // For each item in the board array
    for(uint8_t index = 0; index < 81; index++) {
        // Draw empty board
        // Skip if index zero (This would otherwise trigger all of the % checks for this index)
        if(index != 0) {
            // Every 3 lines (27 indexs)
            if((index % 27) == 0) {
                waddch(b, '\n');
                // For each space in this row
                for(uint8_t col_index = 0; col_index < 9; col_index++) {
                    // Check the first board item in this column.
                    //  If this item has the COL_SELECT flag set, add color
                    if(board[col_index] & COL_SELECT) {
                        wattron(b, COLOR_PAIR(4));
                        waddch(b, ACS_HLINE);
                        waddch(b, ACS_HLINE);
                        wattroff(b, COLOR_PAIR(4));
                    }else {
                        // If col is not selected, print with window default colors
                        waddch(b, ACS_HLINE);
                        waddch(b, ACS_HLINE);
                    }

                    // If current col index is 2 or 5, add an extra = to match vertical divider
                    if((col_index == 2) || (col_index == 5)) {
                        waddch(b, ACS_PLUS);
                        waddch(b, ACS_HLINE);
                    }
                }
                // Move to next line
                waddch(b, '\n');
            // If at end of row and not at end of block
            }else if((index % 9) == 0) {
                waddch(b, '\n');
            // Every three spaces except at the end of a row
            }else if((index % 3) == 0) {
                // If row is selected
                if(board[index] & ROW_SELECT) {
                    // Print divider with row select color
                    wattron(b, COLOR_PAIR(4));
                    waddch(b, ACS_VLINE);
                    waddch(b, ' ');
                    wattroff(b, COLOR_PAIR(4));
                }else {
                    // Print divider with default color
                    waddch(b, ACS_VLINE);
                    waddch(b, ' ');
                }
            }
        }
        // Fill board values
        
        // If cell is not empty
        if(board[index] != 0) {
            // If either the row OR column are selected
            if((board[index] & ROW_SELECT) || (board[index] & COL_SELECT)) {
                // If both the row AND column are selected
                if((board[index] & ROW_SELECT) && (board[index] & COL_SELECT)) {
                    // Apply cell selection color
                    wattron(b, COLOR_PAIR(5));
                    // If selected cell is empty
                    if((board[index] & NUMBER_MASK) == 0) {
                        // Print an empty space
                        wprintw(b, "  ");
                    }else {
                        // If cell is not empty, print contents of cell
                        wprintw(b, "%d ", (board[index] & NUMBER_MASK));
                    }
                    wattroff(b, COLOR_PAIR(5));
                }else {
                    // If cell is in selected row or column
                    // Apply row/col select color
                    wattron(b, COLOR_PAIR(4));
                    // If selected cell is empty
                    if((board[index] & NUMBER_MASK) == 0) {
                        // Print an empty space
                        wprintw(b, "  ");
                    }else {
                        // If cell is not empty, print contents of cell
                        wprintw(b, "%d ", (board[index] & NUMBER_MASK));
                    }
                    wattroff(b, COLOR_PAIR(4));
                }
            }else if(board[index] & ORIGINAL_BITMASK) {
                // If current cell was filled from initial board setup
                // Apply original number color
                wattron(b, COLOR_PAIR(3));
                // print cell value
                wprintw(b, "%d ", (board[index] & NUMBER_MASK));
                wattroff(b, COLOR_PAIR(3));
            }else {
                // print cell value
                wprintw(b, "%d ", (board[index] & NUMBER_MASK));
            }
        }else {
            // If cell is empty, print a space
            // Print an empty space
            wprintw(b, "  ");
        }
    }
    wrefresh(b);
}

/*** INTERNAL USE ONLY ***/


/*
 *
 * Name:
 *      create_new_window
 * 
 * Inputs:
 *      int height
 *          The height of the new window.
 *      int width
 *          The width of the new window.
 *      int starty
 *          Y starting position of the new window.
 *      int startx
 *          X starting position of the new window.
 *      int bdr
 *          Select which border patern to aply to the new window
 * 
 * Outputs:
 *      WINDOW *
 *          This function returns a pointer to the newly created window.
 * 
 * Description:
 *      This function creates a new nCurses window using the given information
 *      and returns a pointer to it.
 * 
 */
WINDOW *create_new_window(int h, int w, int sy, int sx, int bdr) {
    WINDOW *local_w;

    local_w = newwin(h, w, sy, sx);
    if(bdr == 1) {
        wattron(local_w, COLOR_PAIR(1));
        wborder(
            local_w, 
            ACS_VLINE, 
            ACS_VLINE, 
            ACS_HLINE, 
            ACS_HLINE, 
            ACS_ULCORNER, 
            ACS_URCORNER, 
            ACS_LLCORNER, 
            ACS_LRCORNER
        );
        wattroff(local_w, COLOR_PAIR(1));
    }else if(bdr == 2) {
        wattron(local_w, COLOR_PAIR(1));
        wborder(
            local_w, 
            '|', 
            '|', 
            '-', 
            '-',
            '+', 
            '+', 
            '+', 
            '+'
        );
        wattroff(local_w, COLOR_PAIR(1));
    }

    wrefresh(local_w);

    return local_w;
}


