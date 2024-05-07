#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "ttt.h"

int main() {
    struct Game game = {{0}};
    if (setup_game(&game) < 0) {
        printf("error in game setup \n");
        return -1;
    }

    struct Game *gameptr = &game;

    int new_move;
    while (1) {
        if (is_player_turn) {
            // do player move
            print_board(gameptr->board);

            new_move = get_player_move(gameptr);
            if (new_move == -1) {
                printf("standard input closed\n");
                return -1;
            }
            make_move(new_move, game.player_piece, gameptr->board);
        } else {
            // do computer move
            print_board(gameptr->board);
            //sleep(1);

            new_move = get_computer_move(gameptr);
            make_move(new_move, game.computer_piece, gameptr->board);
        }

        // check if the game is over
        if (check_board(gameptr, new_move) == 1) {
            if (is_player_turn) {
                print_board(gameptr->board);
                printf("You win!  Congrats!\n");
                return 0;
            } else {
                print_board(gameptr->board);
                printf("You lose, ggwp\n");
                return 0;
            }
        } else if (check_board(gameptr, new_move) == 2) {
            print_board(gameptr->board);
            printf("Stalemate.  Well fought!\n");
            return 0;
        } else {
            // continue playing
            is_player_turn = !is_player_turn;
        }
    }
}