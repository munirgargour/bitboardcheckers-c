#include <stdio.h>
#include <stdint.h>
#include "utility.h"
#define BOARD_SIZE 64

int main() {
    initializeBoard();
    int redturn = 1;  // 1 for red's turn, 0 for black's turn
    int from, to;

    while (1) {
        // Print the current board state
        printf("Tile number reference:\n");
        PrintBlankBoard(blank);
        printf("Current Board:\n");
        PrintBoard(red_pieces, black_pieces, kings);


        // Get user input for the move
        if (redturn) {
            printf("Red's turn. Enter move (from to e.g. 19 28 ): ");
        } else {
            printf("Black's turn. Enter move (from to e.g. 44 33): ");
        }
        scanf("%d %d", &from, &to);
        // Validate and try to make the move
        int move_success;
        if (redturn) {              // if it's red turn, move success is defined as whether a valid move from red is attempted. if a move from black is attempted, it's invalid and vice versa.
            move_success = try_move(&red_pieces, &black_pieces, &kings, from, to, redturn);
        } else {
            move_success = try_move(&black_pieces, &red_pieces, &kings, from, to, redturn);
        }
        // If the move is valid, switch turns
        if (move_success == 1 && redturn == 1) {
            redturn = 0;  // if move is successful on red turn (and no capture is done) then change to black turn.
        }
        else if (move_success == 1 && redturn == 0){
            redturn = 1; // vice versa.
        }
        else if(move_success == 2){
                printf("Piece captured! Continue to move ");
                continue; // if move success = 2, meaning move was successful and a capture was made, then it stays your turn.
        }
         else {
            printf("Invalid move. Try again.\n");
        }
    }

    return 0;
}
