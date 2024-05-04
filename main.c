#include <stdio.h>
#include "ttt.h"

#define LINE_BUFFER 16

const char pieces[3] = {' ', 'O', 'X'};
const char rows[3] = {'1', '2', '3'};
const char cols[3] = {'a', 'b', 'c'};
const size_t board_size = 3;
const int turn = 1;


void print_board(struct Game *game){
    int *board = game->board;

    printf("<row>\n\n");
    printf("3   %c | %c | %c\n", pieces[board[0]], pieces[board[1]], pieces[board[2]]);
    printf("    --|---|--\n");
    printf("2   %c | %c | %c\n", pieces[board[3]], pieces[board[4]], pieces[board[5]]);
    printf("    --|---|--\n");
    printf("1   %c | %c | %c\n", pieces[board[6]], pieces[board[7]], pieces[board[8]]);
    printf("\n    a   b   c    <col>\n");
    return;
}


int get_player_move(struct Game *game) {
    //read players move from stdin
    char line[LINE_BUFFER] = {0};
    while (fgets(line, sizeof(line), stdin) != NULL) {
        printf("Your move: ");
        
        //validate input
        char char0 = line[0];
        char char1 = line[1];
        char newline = line[2];

        int col;
        int row;

        // get col
        if (char0 == 'a') {
            col = 0;
        } else if (char0 == 'b') {
            col = 1;
        } else if (char0 = 'c') {
            col = 2;
        } else {
            printf("error in column input\n");
            continue;
        }

        // get row
        if (char1 == '1') {
            row = 2;
        } else if (char1 == '2') {
            row = 1;
        } else if (char1 = '3') {
            row = 0;
        } else {
            printf("error in row input\n");
            continue;
        }

        // check for newline
        if (newline != '\n') {
            printf("please only give two characters\n");
            continue;
        }

        // get the index of the square given row and column coords
        int square = row*3 + col;

        // check that the given square is not occupied
        int *board = game->board;
        if (board[square] != 0) {
            printf("this tile is occupies\n");
            continue;
        }

        return square;
    }
    // standard input was closed
    return -1;
}


int get_computer_move(struct Game *game){
    int *board = game->board;
    //do it randomly
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) {
            return i;
        }
    }

    return -1;
}


int make_move(int move, int pieceindex, struct Game *game){
    int *board = game->board;
    board[move] = pieceindex;
    return 0;
}

int main() {

    int player_piece = 1;
    int computer_piece = 2;
    struct Game game = {{0}, player_piece, computer_piece};

    int player_move;
    int computer_move;
    while (1) {
        //player move
        player_move = get_player_move(&game);
        if (player_move < 0) {
            printf("Error in reading from stdin\n");
            return -1;
        }
        
        make_move(player_move, game.player_piece, &game);
        print_board(&game);
        
        //computer move
        computer_move = get_computer_move(&game);
        make_move(computer_move, game.computer_piece, &game);
        print_board(&game);
    }
}