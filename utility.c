
#include <stdio.h>
#include <stdint.h>
#include "utility.h"
#define BOARD_SIZE 64

typedef uint64_t Bitboard;

Bitboard red_pieces;
Bitboard black_pieces;
Bitboard kings;

void initializeBoard(){
black_pieces = 0x55AA550000000000;
red_pieces =    0x0000000000AA55AA;
kings =            0x0000000000000000;
blank =            0x0000000000000000;
}

void PrintBoard(Bitboard redBoard, Bitboard blackBoard, Bitboard kingBoard){  // Loop over 8x8 board, but only print when on playable positions (even row + odd col, or odd row + even col)
    int row, col;
    int bit_position = 0; // Index for bitboard

    for (row = 0; row < 8; row++) {
        for (col = 0; col < 8; col++) {
            // Check if the current position is playable (dark squares/ with a dot on them)
            if ((row + col) % 2 == 1) {
                // for each position in the bitboard, first check if only a red piece is in a position, then only black, then red and king, then black and king.
                if (redBoard & (1ULL << bit_position) && !(blackBoard & (1ULL << bit_position)) && !(kingBoard &(1ULL << bit_position))) {
                    printf("%s%c%s", " ",'r', " ");
                }
                else if (blackBoard & (1ULL << bit_position) && !(redBoard & (1ULL << bit_position)) && !(kingBoard &(1ULL << bit_position))){
                    printf("%s%c%s"," ",'b', " ");
                }
                else if (redBoard & kingBoard &(1ULL << bit_position)){
                    printf("%s%c%s" , " " , 'R', " ");
                }
                else if (blackBoard & kingBoard &(1ULL << bit_position)){
                    printf("%s%c%s", " ", 'B', " ");
                }
                else{
                    printf(" . ");
                }
                bit_position++;
            } else {
                // Non-playable positions (white tiles)
                printf("%s", "   ");
                bit_position++;
            }
        }
        printf("\n");
    }
}


void PrintBlankBoard(Bitboard blankBoard){
    int row, col;
    int bit_position = 0; // This function is to show the player what number each tile corresponds to by printing the tile numbers.

    for (row = 0; row < 8; row++) {
        for (col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                printf("%s%d%s", " ",bit_position, " ");
                bit_position++;
            } else {
                printf("%s", "   ");
                bit_position++;
            }
        }
        printf("\n");
    }
}


int is_playable_tile(int position) {
    int row = position / 8; // if row is even and column is odd, playable position. if row is odd and column is even, playable position.
    int col = position % 8;
    return (row % 2 == 0 && col % 2 == 1) || (row % 2 == 1 && col % 2 == 0);
}
void make_move(Bitboard *player, Bitboard *opponent, Bitboard *kings, int from, int to, int redTurn) {
    int is_king = *kings & (1ULL << from);
    *player &= ~(1ULL << from); // Clear the from position
    *player |= (1ULL << to);    // Set the to position

    if (is_king) {
        *kings &= ~(1ULL << from); // Clear the from position in kings
        *kings |= (1ULL << to);    // Set the to position in kings
    }
    check_promotion(player, opponent, kings, from, to, redTurn);
}
// Helper function to check if the destination square is empty
int is_square_empty(Bitboard red_pieces, Bitboard black_pieces, int square) {
    return !((red_pieces | black_pieces) & (1ULL << square));
}

// Helper function to check if a piece is a king
int is_king(Bitboard kings, int square) {
    return (kings >> square) & 1;
}

// Helper function to check if the piece at a given position belongs to the opponent
int is_opponent_piece(Bitboard opponent_pieces, int square) {
    return (opponent_pieces >> square) & 1;
}

void set_bit(Bitboard *board, int position) {
    *board |= (1ULL << position);
}

void clear_bit(Bitboard *board, int position) {
    *board &= ~(1ULL << position);
}

void toggle_bit(Bitboard *board, int position) {
    *board ^= (1ULL << position);
}

int get_bit(Bitboard board, int position) {
    return (board >> position) & 1;
}

int bit_addition(int a, int b) {
    int carry;
    while (b != 0) {
        carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

int bit_subtraction(int a, int b) {
    int borrow;
    while (b != 0) {
        borrow = (~a) & b;
        a = a ^ b;
        b = borrow << 1;
    }
    return a;
}

int bit_multiplication(int a, int b) {
    int result = 0;
    while (b != 0) {
        if (b & 1) {
            result = bit_addition(result, a);
        }
        a <<= 1;
        b >>= 1;
    }
    return result;
}

int bit_division(int a, int b) {
    if (b == 0) {
        printf("Error: Cannot divide by zero.\n");
        return 0;
    }
    int quotient = 0;
    int remainder = 0;
    for (int i = 31; i >= 0; i--) {
        remainder = (remainder << 1) | ((a >> i) & 1);
        if (remainder >= b) {
            remainder = bit_subtraction(remainder, b);
            quotient |= (1 << i);
        }
    }
    return quotient;
}

int has_valid_moves(Bitboard player, Bitboard opponent, Bitboard kings, int is_red_turn) {
    for (int i = 0; i < 64; i++) {
        if (get_bit(player, i)) {
            // Try all possible moves (diagonal directions)
            int directions[] = {7, 9, -7, -9}; // every non jump move is either 7 or 9 away from the current piece.
            int directions2[] = {14,18,-14,-18}; // every jump move is either 14 or 18 away from current piece
            for (int j = 0; j < 4; j++) {
                int to = i + directions[j];
                int to2 = i + directions2[j]; // then it checks if a move exists for jumping or not jumping.
                if (to >= 0 && to < 64) {
                    if (can_move(player, opponent, kings, i, to, is_red_turn)) {
                        return 1;
                    }
                }
                if (to2 >= 0 && to2 < 64) {
                    if (can_move(player, opponent, kings, i, to, is_red_turn)) {
                        return 1;
                    }
            }
        }
    }
    return 0;
}
}
// check promotion checks
void check_promotion(Bitboard *player, Bitboard *kings, int from, int to, int is_red_turn) {
    if ((is_red_turn && (square >= 56)) || (!is_red_turn && (square <= 7))) {
        *kings |= (1ULL << square);
    }
}


// can move is essentially the same as try move but without updating the boards. It is used for checking win condition.
int can_move(Bitboard player, Bitboard opponent, Bitboard kings, int from, int to, int is_red_turn) {
    int from_col = from % 8;
    int to_col = to % 8;
    int from_row = from / 8;
    int to_row = to / 8;

    // Determine if the piece is a king
    int king = kings & (1ULL << from);

    // Compute move direction
    int row_diff = to_row - from_row;  // Difference in rows (vertical movement)
    int col_diff = to_col - from_col;  // Difference in columns (horizontal movement)

    // Regular pieces can only move forward
    if (!king) {
        if (is_red_turn && row_diff != 1) {
            return 0;
        }
        if (!is_red_turn && row_diff != -1) {
            return 0;
        }
    }

    // Check for valid move (1 space diagonal move)
    if ((row_diff == 1 || row_diff == -1) && (col_diff == 1 || col_diff == -1)) {
        if (is_square_empty(player, opponent, to)) {
            return 1;  // Move is valid
        }
    }

    // Check for valid jump (2 spaces diagonal move)
    if ((row_diff == 2 || row_diff == -2) && (col_diff == 2 || col_diff == -2)) {
        int jumped_row = (from_row + to_row) / 2;
        int jumped_col = (from_col + to_col) / 2;
        int jumped_square = jumped_row * 8 + jumped_col;

        // Check if opponent's piece is in the middle and the destination is empty
        if (is_opponent_piece(opponent, jumped_square) && is_square_empty(player, opponent, to)) {
            return 1;  // Jump is valid
        }
    }
    return 0;  // Move is not valid
}


// Try to make a move if it's valid
int try_move(Bitboard *player, Bitboard *opponent, Bitboard *kings, int from, int to, int is_red_turn) {
    // Check if destination is empty
    if (!is_square_empty(*player, *opponent, to)) {
        printf("Invalid move: destination square is not empty.\n");
        return 0;
    } // check if from space is empty
    if(is_square_empty(*player, *opponent, from)){
        printf("Invalid move: no piece at from position.\n");
    }

    int from_col = (from % 8); // for use later, checks column / row of inputs.
    int to_col = (to % 8);
    int from_row = (from / 8);
    int to_row = (to / 8);

    if (!is_playable_tile(from) || !is_playable_tile(to)) {
    printf("Invalid move: You can only move to black tiles.\n"); //explained earlier
    return 0;
}
    // Determine if the piece is a king
    int king = is_king(*kings, from);

    // Compute move direction
    int row_diff = (to_row) - (from_row);  // Difference in rows (vertical movement)
    int col_diff = (to_col) - (from_col);  // Difference in columns (horizontal movement)

    // Check for valid non-capturing move (1 space diagonal move)
    if ((row_diff == 1 || row_diff == -1) && (col_diff == 1 || col_diff == -1)) {
        make_move(player, opponent, from, to, is_red_turn);
        return 1;  // Move is valid if column diff / row diff is 1 or -1.
    }
if (!king) {
        if (is_red_turn && row_diff < 1) {
            printf("Invalid move: Red pieces can only move forward.\n");
            return 0;
        }
        if (!is_red_turn && row_diff > -1) {
            printf("Invalid move: Black pieces can only move forward.\n");
            return 0;
        }
    }
    // Check for valid jump (2 spaces diagonal move)
    if ((row_diff == 2 || row_diff == -2) && (col_diff == 2 || col_diff == -2)) {
        int jumped_row = (from_row + to_row) / 2;
        int jumped_col = (from_col + to_col) / 2;
        int jumped_square = jumped_row * 8 + jumped_col;
        printf("Jumped square: %d\n", jumped_square);
        // Check if opponent's piece is in the middle
        if (is_opponent_piece(*opponent, jumped_square)) {
            // Clear the opponent's jumped piece
            *opponent &= ~(1ULL << jumped_square);
            make_move(player, opponent, from, to, is_red_turn);
            return 2;  // Piece was captured, then it returns 2 to let main know to give this player another turn.
        }
    }
    printf("Invalid move: not a valid move or jump.\n");
    return 0;  // Move is not valid
}
