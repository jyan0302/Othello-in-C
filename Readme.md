Othello, aka Reversi, is a two-player strategy board game in which players try to get the most pieces by flipping over opponent's pieces
by trapping them between two of their own pieces. For more information, https://en.wikipedia.org/wiki/Reversi

This is a C program which runs a Reversi game on the terminal via ASCII characters. The user first chooses the board size, standard being 8x8, and 
then chooses to either play with a computer or pit two computers against each other.  

The computer finds the best move via a search tree that utilizes a minimax algorithm. Alpha-beta pruning is then used to reduce the number of nodes evaluated.
General heuristics are then used to evaluate the score of the state of the board n moves later, and the computer will choose the move that gives the best final state.

An interesting aspect is that the shortcomings of the minimax algorithm is that it assumes that the other player is just as smart as the computer. However, if the player
is "dumber" than the computer, then minimax algorithm may actually return a move than is slightly worse that the best move as it assumes the other player always pick the 
right move. Yet, it is still expected that the computer will win because it is after all "smarter", so in the long run it should still win the game. The opposite may occur if
the opponent is smarter than the computer. The computer's heuristics are not perfect, so a computer with better heuristics or a very experienced player should beat it.

