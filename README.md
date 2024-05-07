# tic-tac-toe
tic tac toe in the console

main idea: board represented as an array of integers, 0 for empty, 1 for player, 2 for computer
player selects square using chess square notation, a1, a2, a3, b1, ..., c2, c3

1st commit: 
    - Basic structure has been created.  The board is printed to stdout, player input is read from stdin.  With tile and player information, a move can be made on the board.  The computer currently makes moves randomnly/selects the first available tile in the array.
    - To be implemented: selecting which piece to play upon start of game, minmax search for best computer move, checking if the game is won or not.

2nd commit: 
    - Error in get_player_move(), where if column != a or b and row !- 1 or 2, the function would assign the values c and 3 respectively, rather than evaluate
    (line[0] = 'c' rather than line[0] == 'c').
    - Player can now select move prior to game start, turn logic also implemented (i.e. if player chooses crosses, they go second)

3rd commit:
    - After each move, we now check if the game is won (or drawn).  Slight changes to the function declaration, we do not return the winner of the game, only if the game is won, since only the person who played the last move can win, and it is easier to compute this only when we have a game over.
    - Todo: Implement minmax to improve computer moves.  Implement alpha-beta pruning to optimize the minmax algorithm

4th commit:
    - minmax has been implemented for improved computer moves.  The static evaluation simply returns neutral, so minmax only considers three cases of the game, a win, a loss, or neither (either stalemat or in play).  As such, a depth of at least 5 is required for the algorithm to not miss forced wins.  As for speed, the algorithm still operates fairly quickly on an empty board with depth 9 (the board with the largest number of computations required).  However, implementing alpha beta pruning will still be an interesting experiment.
    - Function prototypes were edited, mainly to only pass in the int array board rather than the whole game struct, for simplicity's sake.

5th commit:
    - Implemented minmax with alpha-beta pruning
    - However, in both algorithms I discovered an edgecase where the game a2-a3 b2-b3 c2 is played, leading to an easy win for the player.  Will need to fix.  Also need a more better evaluate algorithm.

6th commit:
    - The cause of the error described in commit 5 was due to Minimax not checking for if the game is in stalemate, thus sometimes concluding that the game is lost when there is hope to draw.  This has now been fixed.
    - Main function moved to new file, had to also move the boolean var 'is_player_turn' to the header file
    - Makefile created to help compile the program
    - Static evaluation function improved, it now evaluates a position based on how many winning oppotunities a player has, minus how many winning opportunities the opponent has
    - AI now plays both offensively and defensively, rather than randombly until a win or loss is found in Minimax search.