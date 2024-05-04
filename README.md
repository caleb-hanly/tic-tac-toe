# tic-tac-toe
tic tac toe in the console

main idea: board represented as an array of integers, 0 for empty, 1 for player, 2 for computer
player selects square using chess square notation, a1, a2, a3, b1, ..., c2, c3

1st commit: basic structure has been created.  The board is printed to stdout, player input is read from stdin.  With tile and player information, a move can be made on the board.  The computer currently makes moves randomnly/selects the first available tile in the array. To be implemented: selecting which piece to play upon start of game, minmax search for best computer move, checking if the game is won or not.