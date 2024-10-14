#include <stdio.h>
#include <stdint.h>
#ifndef UTILITY_H
#define UTILITY_H

#define BOARD_SIZE 64 // I used a 64 bit bitboard because it seemed more intuitive to me.
// many problems arose when I tried working with a 32 bit bitboard.

typedef uint64_t Bitboard;

Bitboard red_pieces;
Bitboard black_pieces;
Bitboard kings;
Bitboard blank;

extern void set_bit(Bitboard *board, int position);
extern void clear_bit(Bitboard *board, int position);
extern void toggle_bit(Bitboard *board, int position);
extern void initializeBoard();
extern void PrintBoard(Bitboard redBoard, Bitboard blackBoard, Bitboard kingBoard);
extern void make_move(Bitboard *player, Bitboard *kings, int from, int to);
extern void check_promotion(Bitboard *player, Bitboard *kings, int square, int is_red_turn);
int can_move(Bitboard player, Bitboard opponent, Bitboard kings, int from, int to, int is_red_turn);
int is_square_empty(Bitboard red_pieces, Bitboard black_pieces, int square);
int get_bit(Bitboard board, int position);
int is_king(Bitboard kings, int square);
int is_opponent_piece(Bitboard opponent_pieces, int square);
int try_move(Bitboard *player, Bitboard *opponent, Bitboard *kings, int from, int to, int is_red_turn);

#endif
