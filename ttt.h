#ifndef TTT_H
#define TTT_H

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

/*allow the player to select Naughts or Crosses
**creates the game struct*/
int setup_game();

/*print out a board given an array of pieces
**(0 = empty, 1 = player, 2 = computer)*/
void print_board(struct Game *game);

/*Get the players move from standard input (chess notation e.g. a1)
**Rows = {1, 2, 3}, Cols = {a, b, c} with a1 at bottom left
**Return an int from 0:9 representing the square*/
int get_player_move(struct Game *game);

/*Get the computer move
**Do it randomly for now*/
int get_computer_move(struct Game *game);

/*Make the given move for the given piecetype on the given board
**Return the given board*/
int make_mode(int move, int *pieceindex, struct Game *game);

/*Check the current state of the game
**If someone wins, return that player val (1 for player, 2 for computer)
**If it is a draw, return -1
**If the game is not yet over, return 0*/
int check_board(struct Game *game);

#endif