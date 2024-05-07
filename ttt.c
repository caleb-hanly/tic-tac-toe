#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "ttt.h"

#define LINE_BUFFER 16
#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

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

void print_board(int *board){
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

int make_move(int move, int pieceindex, int *board){
    board[move] = pieceindex;
    return 0;
}

void unmove(int move, int *board) {
    board[move] = 0;
    return;
}

int check_for_win(int *board, int winning_piece) {
    //as only the last move can win, we just check the piece types of the previous move
    //check verticals
    for (int i = 0; i < 3; i++) {
        if ((board[i] == winning_piece) && (board[3+i] == winning_piece) && board[6+i] == winning_piece) {
            return 1;
        }
    }

    //check horizontals (rows starting with tiles 0, 3, 6)
    for (int i = 0; i < 7; i += 3) {
        if ((board[i] == winning_piece) && (board[i+1] == winning_piece) && (board[i+2] == winning_piece)) {
            return 1;
        }
    }

    // check both diagonals (0->4->8) and (2->4->6)
    if ((board[0] == winning_piece) && (board[4] == winning_piece) && (board[8] == winning_piece)) {
        return 1;
    } else if ((board[2] == winning_piece) && (board[4] == winning_piece) && (board[6] == winning_piece)) {
        return 1;
    }

    // there is no winning
    return 0;
}

int check_for_stalemate(int *board) {
    int board_size = 9;
    for (int i = 0; i < board_size; i++) {
        if (board[i] == 0) {
            // game is not in stalemate
            return 0;
        }
    }
    return 1;
}

int check_board(struct Game *game, int most_recent_move) {
    int *board = game->board;

    // check if someone has won
    if (check_for_win(board, board[most_recent_move]) > 0) {
        return 1;
    }

    // check if board is full (stalemate)
    if (check_for_stalemate(board)) {
        return 2;
    }

    //game is not is stalemate
    return 0;
}

int minimax(int *board, int depth, bool maximising_player, int max_player_piece) {
    // get the piece type of the opponent
    int opponent_piece = (max_player_piece == 1) ? 2 : 1;

    // if someone has won, return an eval of +/- 9 to dominate any static evaluation of other boards
    if ((maximising_player) && check_for_win(board, opponent_piece)) {
        // the opponent won last round
        return -9;
    } else if ((!maximising_player) && check_for_win(board, max_player_piece)) {
        // the player won last round
        return 9;
    } else if (check_for_stalemate(board)) {
        // no one is winning or losing
        return 0;
    }

    // if depth == 0 -> return static evaluation
    if (depth == 0) {
        return evaluate(board, (maximising_player) ? max_player_piece : opponent_piece);
    }

    // else pick the best move for the current player
    if (maximising_player) {
        int maxeval = -9;
        
        //for move in moves
        for (int i = 0; i < 9; i++) {
            if (board[i] != 0) {
                continue;
            }
            make_move(i, max_player_piece, board);
            
            // recursively evaluate the position after this move
            int eval = minimax(board, depth-1, false, max_player_piece);
            maxeval = max(maxeval, eval);
            unmove(i, board);
        }
        return maxeval;
    } else {
        //this is the opponent, return mineval
        int mineval = 9;
        for (int i = 0; i < 9; i++) {
            if (board[i] != 0) {
                continue;
            }
            make_move(i, opponent_piece, board);

            // recursively evaluate the new position
            int eval = minimax(board, depth-1, true, max_player_piece);
            mineval = min(mineval, eval);
            unmove(i, board);
        }
        return mineval;
    }
}

int is_free(int *board, int tile, int player) {
    if ((board[tile] == player) || (board[tile] == 0)) {
        return 1;
    }
    return 0;
}

int get_num_possible_wins(int *board, int player) {
    int count = 0;

    // check the horizontals
    for (int i = 0; i < 3; i++) {
        bool player_win_row = is_free(board, 3*i, player) && is_free(board, 3*i + 1, player) && is_free(board, 3*i + 2, player);
        if (player_win_row) {count++;}
    }

    // check the horizontals
    for (int i = 0; i < 3; i++) {
        bool player_win_col = is_free(board, i, player) && is_free(board, i + 3, player) && is_free(board, i + 6, player);
        if (player_win_col) {count++;}
    }

    //check the diagonals
    bool player_win_diag1 = is_free(board, 0, player) && is_free(board, 4, player) && is_free(board, 8, player);
    bool player_win_diag2 = is_free(board, 2, player) && is_free(board, 4, player) && is_free(board, 6, player);
    if (player_win_diag1) {count++;}
    if (player_win_diag2) {count++;}

    return count;
}

int evaluate(int *board, int player) {
    int opponent = (player == 1) ? 2 : 1;

    // calculate the number of lines where the given player can win (0 <= count <= 8)
    int player_count = get_num_possible_wins(board, player);
    int opponent_count = get_num_possible_wins(board, opponent);

    // return the different of the two counts
    return player_count - opponent_count;
}

int alphabeta(int *board, int depth, int alpha, int beta, bool maximising_player, int max_player_piece) {
    // get the piece type of the opponent
    int opponent_piece = (max_player_piece == 1) ? 2 : 1;

    // if someone has won, return best eval for that player
    if ((maximising_player) && check_for_win(board, opponent_piece)) {
        // the opponent won last round, so return worst possible result (-9)
        return -9;
    } else if ((!maximising_player) && check_for_win(board, max_player_piece)) {
        // the player won last turn, so return 9
        return 9;
    } else if (check_for_stalemate(board)) {
        //check for stalemate
        return 0;
    }

    // if depth == 0 -> return static evaluation
    if (depth == 0) {
        return evaluate(board, (maximising_player) ? max_player_piece : opponent_piece);
    }

    // else pick the best move for the current player
    if (maximising_player) {
        int maxeval = -9;
        
        //for move in moves
        for (int i = 0; i < 9; i++) {
            if (board[i] != 0) {
                continue;
            }
            make_move(i, max_player_piece, board);
            
            // recursively evaluate the position after this move
            int eval = alphabeta(board, depth-1, alpha, beta, false, max_player_piece);
            maxeval = max(maxeval, eval);
            unmove(i, board);

            // prune search tree
            alpha = max(alpha, eval);
            if (beta <= alpha) {
                // this position is better for the player than the other moves in the search tree
                // so the opponent should never consider this case (and if they do, good) -> hence prune
                break;
            }
        }
        return maxeval;
    } else {
        //this is the opponent, return mineval
        int mineval = 9;
        for (int i = 0; i < 9; i++) {
            if (board[i] != 0) {
                continue;
            }
            make_move(i, opponent_piece, board);

            // recursively evaluate the new position
            int eval = alphabeta(board, depth - 1, alpha, beta, true, max_player_piece);
            mineval = min(mineval, eval);
            unmove(i, board);

            // prune search tree
            beta = min(eval, beta);
            if (beta <= alpha) {
                // this move is better for the opponent than the worst case in the other subtree
                // so the computer should never consider this case -> hence prune
                break;
            }
        }
        return mineval;
    }
}

int get_computer_move(struct Game *game){
    int *board = game->board;
    int computer_piece = game->computer_piece;
    int max_depth = 2;

    int best_move;
    int best_eval = -10;

    int remaining_squares = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) {
            remaining_squares++;
        }
    }

    if (remaining_squares == 0) {
        return -1;
    }

    for (int i = 0; i < 9; i++) {
        if (board[i] != 0) {
            continue;
        }

        make_move(i, computer_piece, board);
        int eval;

        //eval = minimax(board, min(remaining_squares, max_depth), false, computer_piece);
        eval = alphabeta(board, min(remaining_squares, max_depth), -10, 10, false, computer_piece);
        unmove(i, board);
        if (eval > best_eval) {
            best_move = i;
            best_eval = eval;
        }

        printf("%d %d\n", i, eval);
    }
    return best_move;
}
