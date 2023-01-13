#include "engine.h"

uint8_t valid_move(uint8_t row, uint8_t col, uint8_t num, uint8_t *board, WINDOW *message_window);
uint8_t *clear_selection(uint8_t row, uint8_t col, uint8_t *board);
uint8_t get_usr_input(void);

/*
 *
 * Name:
 *      game_solved
 * 
 * Inputs:
 *      uint8_t *board
 *          An array of 81 elements containing the current board state.
 * 
 * Outputs:
 *      int
 *          0 -> Game is not solved
 *          1 -> Game is solved (win condition)
 * 
 * Description:
 *          Checks to see if the given game board is full. Since every 
 *      placement is validated when it is generated, it can be assumed 
 *      that a full board is also a valid board.
 * 
 */
uint8_t game_solved(uint8_t *board) {
    // For every cell in the board
    for(uint8_t index = 0; index < 81; index++) {
        // If an empty space is found
        if((board[index] & NUMBER_MASK) == 0) {
            return 0;
        }
    }

    // If no empty spaces found
    return 1;
}


/*
 *
 * Name:
 *      place_piece
 * 
 * Inputs:
 *      uint8_t *board
 *          An array of 81 elements containing the current board state.
 *      WINDOW *message_window
 *          Pointer to the message window.
 *      WINDOW *game
 *          Pointer to window containing game board.
 * 
 * Outputs:
 *      void
 * 
 * Description:
 *          Propts the user to select a cell and a number to place. If the
 *      user's move is valid, update the game board. Otherwise, 
 *      return.
 * 
 */
void place_piece(uint8_t *board, WINDOW *message_window, WINDOW *game) {
    uint8_t row, col, num, index;
    /*** Prompt user for row ***/
    // Update message screen
    clear_message_window(message_window);
    mvwprintw(message_window, 1, 1, " > Select A Row          ");
    mvwprintw(message_window, 2, 1, "      Valid: 1 -> 9      ");
    wrefresh(message_window);
    // Clear previous row/col selections
    update_board_window(board, game);
    
    // -'0' Converts an int char into an int int: 
    //      '0'-'0' = 0
    //      '5'-'0' = 5
    //      '9'-'0' = 9
    // The extra -1 is because the user interface has rows/cols 
    //  from 1->9 and internal board access as 0->8
    row = get_usr_input() - '0' - 1;

    // Highlight row
    for(index = 0; index < 9; index++) {
        board[9*row+index] |= ROW_SELECT;
    }
    update_board_window(board, game);

    /*** Prompt user for column ***/
    clear_message_window(message_window);
    mvwprintw(message_window, 1, 1, " > Select A Column       ");
    mvwprintw(message_window, 2, 1, "      Valid: 1 -> 9      ");
    wrefresh(message_window);
    
    // -'0' Converts an int char into an int int: 
    //      '0'-'0' = 0
    //      '5'-'0' = 5
    //      '9'-'0' = 9
    // The extra -1 is because the user interface has rows/cols 
    //  from 1->9 and internal board access as 0->8
    col = get_usr_input() - '0' - 1;
    
    // Highlight col
    for(index = 0; index < 9; index++) {
        board[9*index+col] |= COL_SELECT;
    }
    update_board_window(board, game);

    /*** Check if the space is empty ***/
    if((board[9*row+col] & NUMBER_MASK) != 0) {
        clear_message_window(message_window);
        mvwprintw(message_window, 1, 1, "  ! Cell Is Not Empty !  ");
        mvwprintw(message_window, 2, 1, " Please Make A Selection ");
        wrefresh(message_window);
        
        for(index = 0; index < 9; index++) {
            board[9*row+index] &= ~ROW_SELECT;
            board[9*index+col] &= ~COL_SELECT;
        }

        return;
    }

    /*** Prompt user for number ***/
    clear_message_window(message_window);
    mvwprintw(message_window, 1, 1, " > Select A Number       ");
    mvwprintw(message_window, 2, 1, "      Valid: 1 -> 9      ");
    wrefresh(message_window);
    
    // -'0' Converts an int char into an int int: 
    //      '0'-'0' = 0
    //      '5'-'0' = 5
    //      '9'-'0' = 9
    num = get_usr_input() - '0';

    /*** Check for valif move ***/
    /*** If move is valid, place and return board ***/
    if(!valid_move(row, col, num, board, message_window)) {
        for(index = 0; index < 9; index++) {
            board[9*row+index] &= ~ROW_SELECT;
            board[9*index+col] &= ~COL_SELECT;
        }
        return;
    }
    board[9*row+col] = num;
    clear_message_window(message_window);
    mvwprintw(message_window, 1, 1, "  Cell Has Been Filled!  ");
    mvwprintw(message_window, 2, 1, " Please Make A Selection ");
    wrefresh(message_window);

    /*** Otherwise, return board without updating ***/
    for(index = 0; index < 9; index++) {
        board[9*row+index] &= ~ROW_SELECT;
        board[9*index+col] &= ~COL_SELECT;
    }
    update_board_window(board, game);
    return;
}


/*
 *
 * Name:
 *      remove_piece
 * 
 * Inputs:
 *      uint8_t *board
 *          An array of 81 elements containing the current board state.
 *      WINDOW *message_window
 *          Pointer to the message window.
 *      WINDOW *game
 *          Pointer to window containing game board.
 * 
 * Outputs:
 *      uint8_t *
 *          Returns the updated board
 * 
 * Description:
 *          Propts the user to select a cell and a number to place. If the
 *      cell was not an initially known value, remove it and return the boardl
 * 
 */
void remove_piece(uint8_t *board, WINDOW *message_window, WINDOW *game) {
    uint8_t index, col, row;

    // Clear previous row/col selections
    update_board_window(board, game);

    /*** Prompt user for row ***/
    // Update message screen
    clear_message_window(message_window);
    mvwprintw(message_window, 1, 1, " > Select A Row          ");
    mvwprintw(message_window, 2, 1, "      Valid: 1 -> 9      ");
    wrefresh(message_window);
    // Clear previous row/col selections
    update_board_window(board, game);
    
    row = get_usr_input() - '0' - 1;

    // Highlight row
    for(index = 0; index < 9; index++) {
        board[9*row+index] |= ROW_SELECT;
    }
    update_board_window(board, game);

    /*** Prompt user for column ***/
    clear_message_window(message_window);
    mvwprintw(message_window, 1, 1, " > Select A Column       ");
    mvwprintw(message_window, 2, 1, "      Valid: 1 -> 9      ");
    wrefresh(message_window);
    
    col = get_usr_input() - '0' - 1;
    
    // Highlight col
    for(index = 0; index < 9; index++) {
        board[9*index+col] |= COL_SELECT;
    }
    update_board_window(board, game);

    // Check for initial value flag
    if(board[9*row+col] & ORIGINAL_BITMASK) {
        clear_message_window(message_window);
        mvwprintw(message_window, 1, 1, " ! Not A Player Number ! ");
        mvwprintw(message_window, 2, 1, " Please Make A Selection ");
        wrefresh(message_window);
        for(index = 0; index < 9; index++) {
            board[9*row+index] &= ~ROW_SELECT;
            board[9*index+col] &= ~COL_SELECT;
        }
        return;
    }

    board[9*row+col] &= ~NUMBER_MASK;
    update_board_window(board, game);
    clear_message_window(message_window);
    mvwprintw(message_window, 1, 1, "  Cell Has Been Cleared  ");
    mvwprintw(message_window, 2, 1, " Please Make A Selection ");
    wrefresh(message_window);
    for(index = 0; index < 9; index++) {
        board[9*row+index] &= ~ROW_SELECT;
        board[9*index+col] &= ~COL_SELECT;
    }
    return;
}


/***--- INTERNAL USE ONLY FUNCTIONS ---***/


/*
 *
 * Name:
 *      valid_move
 * 
 * Inputs:
 *      uint8_t row
 *          Selected row
 *      uint8_t col
 *          Selected column
 *      uint8_t num
 *          Number to place
 *      uint8_t *board
 *          Current game board
 * 
 * Outputs:
 *      uint8_t
 *          1 -> Valid move
 *          0 -> Invalid move
 * 
 * Description:
 *          Propts the user to select a cell and a number to place. If the
 *      user's move is valid, update and return the game board. Otherwise, 
 *      return the original game board.
 * 
 */
uint8_t valid_move(uint8_t row, uint8_t col, uint8_t num, uint8_t *board, WINDOW *message_window) {
    uint8_t row_cnt, col_cnt;

    /*** Check Row ***/
    for(col_cnt = 0; col_cnt < 9; col_cnt++) {
        if((board[9*row+col_cnt] & NUMBER_MASK) == num) {
            clear_message_window(message_window);
            mvwprintw(message_window, 1, 1, "Error: Row");
            mvwprintw(message_window, 2, 2, "> Make a selection");
            wrefresh(message_window);
            return 0;
        }
    }

    /*** Check Col ***/
    for(row_cnt = 0; row_cnt < 9; row_cnt++) {
        if((board[9*row_cnt+col] & NUMBER_MASK) == num) {
            clear_message_window(message_window);
            mvwprintw(message_window, 1, 1, "Error: col");
            mvwprintw(message_window, 2, 2, "> Make a selection");
            wrefresh(message_window);
            return 0;
        }
    }

    /*** Check Box ***/
    uint8_t box_x, box_y;
    // To see if the given number is already in the cell's box,
    //      We first need to find the first position in the box.
    //      The possible columns are 0, 3 and 6. Using the fact 
    //    that intiger division is always rounded down, we can 
    //    find the correct column with: (cell_col/3)*3
    box_x = (col/3)*3;
    // The same math applies to the row as well
    box_y = (row/3)*3;

    // We now have the top right corner of the box that contains
    //  the given cell. So, we need to check each of the three 
    //  columns of the box in each row of the cell.

    // For each row in box
    for(row_cnt = 0; row_cnt < 3; row_cnt++) {
        // For each column in that row
        for(col_cnt = 0; col_cnt < 3; col_cnt++) {
            if((board[9*(box_y+row_cnt)+(box_x+col_cnt)] & NUMBER_MASK) == num) {
                clear_message_window(message_window);
                mvwprintw(message_window, 1, 1, "Error: box");
                mvwprintw(message_window, 2, 2, "> Make a selection");
                wrefresh(message_window);
                return 0;
            }
        }
    }
    return 1;
}

uint8_t *clear_selection(uint8_t row, uint8_t col, uint8_t *board) {
    uint8_t index;
    // Clear row
    for(index = 0; index < 9; index++) {
        board[9*row+index] &= ~ROW_SELECT;
    }
    // Clear Col
    for(index = 0; index < 9; index++) {
        board[9*index+col] &= ~COL_SELECT;
    }

    return board;
}

uint8_t get_usr_input(void) {
    uint8_t valid = 0;
    uint8_t usr;
    // Loop until the user gives a valid number
    while(!valid) {
        // Get user input
        usr = getch();
        // Check that it is a number from 1 to 9
        if((usr >= '0') && (usr <= '9')) {
            valid = 1;
        }
    }

    return usr;
}