#ifndef TTT_H
#define TTT_H

#include <stdbool.h>

/*
<row>

1   0 | 1 | 2
    --|---|--
2   3 | 4 | 5
    --|---|--
3   6 | 7 | 8

    a   b   c  <column>
*/
struct Game{
    int board[9];
    int player_piece;
    int computer_piece;
};

extern bool is_player_turn;

/*allow the player to select Naughts or Crosses
**creates the game struct*/
int setup_game(struct Game *game);

/*print out a board given an array of pieces
**(0 = empty, 1 = player, 2 = computer)*/
void print_board(int *board);

/*Get the players move from standard input (chess notation e.g. a1)
**Rows = {1, 2, 3}, Cols = {a, b, c} with a1 at bottom left
**Return an int from 0:9 representing the square*/
int get_player_move(struct Game *game);

/*Make the given move for the given piecetype on the given board
**Return the given board*/
int make_move(int move, int pieceindex, int *board);

/*Clears a square on the board
**Useful for computing different positions when searching*/
void unmake_move(int move, int *board);

/*Get the computer move
**Do it randomly for now*/
int get_computer_move(struct Game *game);

/*Recursively search the minmax tree to find the most optimal move
**int is 1 if this is the turn for naughts, 2 if for crosses*/
int minimax(int *board, int depth, bool maximising_player, int max_player_piece);

/*Optimized minimax algorithm using alpha beta pruning*/
int alphabeta(int *board, int depth, int alpha, int beta, bool maximising_player, int max_player_piece);

/*Evaluate a board, based on the number of possible winning lines - number of winning lines for the opponent
**int is 1 if this is the turn for naughts, 2 if for crosses*/
int evaluate(int *board, int player);

/*Check the current state of the game
**If someone wins, return 1
**If it is a draw, return 2
**If the game is not yet over, return 0*/
int check_board(struct Game *game, int most_recent_move);

/*Check if the game is won for the winning piece*/
int check_for_win(int *board, int winning_piece);

/*Check if the game is in stalemate*/
int check_for_stalemate(int *board);

#endif