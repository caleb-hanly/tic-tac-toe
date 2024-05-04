# tic-tac-toe
tic tac toe in the console

main idea: board represented as an array of integers, 0 for empty, 1 for player, 2 for computer
player selects square using chess square notation, a1, a2, a3, b1, ..., c2, c3

1st push: 
    Basic structure has been created.  The board is printed to stdout, player input is read from stdin.  With tile and player information, a move can be made on the board.  The computer currently makes moves randomnly/selects the first available tile in the array.

    To be implemented: selecting which piece to play upon start of game, minmax search for best computer move, checking if the game is won or not.

2nd push: 
    Error in get_player_move(), where if column != a or b and row !- 1 or 2, the function would assign the values c and 3 respectively, rather than evaluate
    (line[0] = 'c' rather than line[0] == 'c').

    Player can now select move prior to game start, turn logic also implemented (i.e. if player chooses crosses, they go second)

3rd push:
    After each move, we now check if the game is won (or drawn).  Slight changes to the function declaration, we do not return the winner of the game, only if the game is won, since only the person who played the last move can win, and it is easier to compute this only when we have a game over.