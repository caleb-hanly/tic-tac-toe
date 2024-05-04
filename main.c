#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "ttt.h"

#define LINE_BUFFER 16

const char pieces[3] = {' ', 'O', 'X'};
const char rows[3] = {'1', '2', '3'};
const char cols[3] = {'a', 'b', 'c'};
bool is_player_turn;


int setup_game(struct Game *game) {
    char line[LINE_BUFFER];
    while (1) {
        // read in stdin
        printf("Do you want to go first (y/n)\n");
        if (fgets(line, LINE_BUFFER, stdin) == NULL) {
            break;
        }

        // validate input
        if (line[1] != '\n') {
            printf("please type exactly one character\n");
            continue;
        }

        if (line[0] != 'y' && line[0] != 'n') {
            printf("please respond with only 'y' or 'n'\n");
            continue;
        } else if (line[0] == 'y') {
            game->player_piece = 1;
            game->computer_piece = 2;
            is_player_turn = true;
        } else {
            game->player_piece = 2;
            game->computer_piece = 1;
            is_player_turn = false;
        }
        return 0;
    }
    return -1;
}


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
    while (1) {
        printf("Your move: ");

        if(fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
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
        } else if (char0 == 'c') {
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
        } else if (char1 == '3') {
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
            printf("this tile is occupied\n");
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


int check_for_win(struct Game *game, int most_recent_move) {
    int *board = game->board;
    int board_size = 9;
    
    // as only the last move can win, we only check the prev move
    int winning_piece = board[most_recent_move];
    printf("Winning Piece %d\n", winning_piece);

    // check verticals
    if (most_recent_move < 3) {
        // check below
        if ((board[most_recent_move + 3] == winning_piece) && (board[most_recent_move + 6] == winning_piece)) {
            return 1;
        }
    } else if (most_recent_move < 6) {
        //check above and below
        if ((board[most_recent_move + 3] == winning_piece) && (board[most_recent_move - 3] == winning_piece)) {
            return 1;
        }
    } else {
        //check above
        if ((board[most_recent_move - 3] == winning_piece) && (board[most_recent_move - 6] == winning_piece)) {
            return 1;
        }
    }

    // check horizontals
    if (most_recent_move % 3 == 0) {
        // check right
        if ((board[most_recent_move + 1] == winning_piece) && (board[most_recent_move + 2] == winning_piece)) {
            return 1;
        }
    } else if (most_recent_move % 3 == 1) {
        //check left and right
        if ((board[most_recent_move + 1] == winning_piece) && (board[most_recent_move - 1] == winning_piece)) {
            return 1;
        }
    } else {
        //check left
        if ((board[most_recent_move - 1] == winning_piece) && (board[most_recent_move - 2] == winning_piece)) {
            return 1;
        }
    }

    // check both diagonals (0->4->8) and (2->4->6)
    if ((board[0] == winning_piece) && (board[4] == winning_piece) && (board[8] == winning_piece)) {
        return 1;
    } else if ((board[2] == winning_piece) && (board[4] == winning_piece) && (board[6] == winning_piece)) {
        return 1;
    }

    // this move did not win the game
    return 0;
}


int check_board(struct Game *game, int most_recent_move) {
    int *board = game->board;

    // check if someone has won
    if (check_for_win(game, most_recent_move) > 0) {
        return 1;
    }

    // check if board is full (stalemate)
    int board_size = 9;
    for (int i = 0; i < board_size; i++) {
        if (board[i] == 0) {
            // game is not in stalemate
            return 0;
        }
    }

    // game is in stalemate
    return 2;
}


int main() {
    struct Game game = {{0}};
    if (setup_game(&game) < 0) {
        printf("error in game setup \n");
        return -1;
    }

    int new_move;
    while (1) {
        if (is_player_turn) {
            // do player move
            print_board(&game);

            new_move = get_player_move(&game);
            if (new_move == -1) {
                printf("standard input closed\n");
                return -1;
            }
            make_move(new_move, game.player_piece, &game);
        } else {
            // do computer move
            print_board(&game);
            sleep(1);

            new_move = get_computer_move(&game);
            make_move(new_move, game.computer_piece, &game);
        }

        // check if the game is over
        if (check_board(&game, new_move) == 1) {
            if (is_player_turn) {
                print_board(&game);
                printf("You win!  Congrats!\n");
                return 0;
            } else {
                print_board(&game);
                printf("You lose, ggwp\n");
                return 0;
            }
        } else if (check_board(&game, new_move) == 2) {
            printf("Stalemate.  Well fought!\n");
            return 0;
        } else {
            // continue playing
            is_player_turn = !is_player_turn;
        }
    }
}