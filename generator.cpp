#include "generator.h"

bool unused_in_box(uint8_t *board, uint8_t row, uint8_t col, uint8_t num) {
    // print current block
    uint8_t row_cnt, col_cnt;
    for(row_cnt = 0; row_cnt < 3; row_cnt++) {
        for(col_cnt = 0; col_cnt < 3; col_cnt++) {
            if(board[9*(row+row_cnt)+(col+col_cnt)] == num) {
                return false;
            }
        }
    }

    return true;
}

bool unused_in_row(uint8_t *board, uint8_t row, uint8_t num) {
    for(uint8_t index = 0; index < 9; index++) {
        if((board[9*row+index]) == num) {
            return false;
        }
    }
    return true;
}

bool unused_in_col(uint8_t *board,uint8_t col, uint8_t num) {
    for(uint8_t index = 0; index < 9; index++) {
        if((board[9*index+col]) == num) {
            return false;
        }
    }
    return true;
}

uint8_t random_gen(uint8_t max_val) {
    return ((rand()%max_val) + 1);
}

bool check_if_safe(uint8_t *board, uint8_t row, uint8_t col, uint8_t num) {
    return (
        unused_in_row(board, row, num) &
        unused_in_col(board, col, num) &
        unused_in_box(board, row-row%3, col-col%3, num)
    );
}

void fill_box(uint8_t *board, uint8_t row, uint8_t col) {
    int num;
    for(uint8_t row_cnt = 0; row_cnt < 3; row_cnt++) {
        for(uint8_t col_cnt = 0; col_cnt < 3; col_cnt++) {
            do {
                num = random_gen(9);
            }while(!unused_in_box(board, row, col, num));
            board[9*(row+row_cnt)+(col+col_cnt)] = num;
        }
    }
}

void fill_diagonal(uint8_t *board) {
    fill_box(board, 0, 0);
    fill_box(board, 3, 3);
    fill_box(board, 6, 6);
}

bool finish_board(uint8_t *board, uint8_t row, uint8_t col) {

    if((row == 8) && (col == 9)) {
        return true;
    }

    if(col == 9) {
        row++;
        col = 0;
    }

    if(board[9*row+col] > 0) {
        return finish_board(board, row, col+1);
    }

    for(int num = 1; num <= 9; num++) {
        if(check_if_safe(board, row, col, num)) {
            
            board[9*row+col] = num;

            if(finish_board(board, row, col+1)) {
                return true;
            }
        }

        board[9*row+col] = 0;
    }

    return false;
}

void remove_digits(uint8_t *board) {
    uint8_t cnt = NUM_TO_REMOVE;
    uint8_t index;
    while(cnt != 0) {
        index = random_gen(81) - 1;

        if(board[index] != 0) {
            board[index] = 0;
            cnt--;
        }
    }
}

void generate_board(uint8_t *board) {
    // Generate random seed
    srand(time(NULL));
    // Fill diagonals
    fill_diagonal(board);
    // Fill rest
    if(!finish_board(board, 0, 0)) {
        board[0] = '!';
        return;
    }

    // Remove digits
    remove_digits(board);
}


void open_board_file(uint8_t *board, char *path_to_file) {
    /*** Attempt to open and read the given file ***/
    // Create an empty file pointer
    FILE *input_file;
    // Attempt to open the given file to read
    input_file = fopen(path_to_file, "r");
    // If there was an error, fopen() will return NULL
    if(input_file == NULL) {
        std::cout << "There was an error opening the given file.\n";
        // Return before filling array, will trigger error in main()
        return;
    }

    /*** Check to see if given file is the correct length ***/
    // Move file pointer to the end of the file
    fseek(input_file, 0, SEEK_END);
    // Calling ftell when the file pointer is at the end of the file, it will 
    //      return the size (in bytes) of the file.
    // A valid game file will have exactly 89 bytes: 81 numbers and 8 '\n' 
    if(ftell(input_file) != 89) {
        std::cout << "The given file is not a valid game file.\n";
        fclose(input_file);
        return;
    }
    // Return the file pointer to the head of the file
    fseek(input_file, 0, SEEK_SET);

    int8_t read_char = 0;
    uint8_t index = 0;

    // Step through the file a single byte at a time
    while(index != 81) {
        read_char = fgetc(input_file);
        // Skip the 8 '\n' characters
        if((read_char != '\n') && (read_char != 255)) {
            // Make sure that the read byte is an ASCII number
            if((read_char >= '0') && (read_char <= '9')) {
                // If the read byte is '0'
                if(read_char == '0') {
                    // Store the zero in the board array
                    //  Do NOT set original value bitmask
                    board[index] = 0;
                }else {
                    // Otherwise...
                    //  Store the given value in the board
                    //  DO SET the original value bitmask
                    board[index] = (read_char - '0') | ORIGINAL_BITMASK;
                }
            }else {
                // Found an invalid character
                std::cout << "The given file contains invalid character: " << +read_char <<"\n";
                fclose(input_file);
                board[0] = '!';
                return;
            }
            // Advance the index
            index++;
        }
    }
    // Close the file
    fclose(input_file);
}