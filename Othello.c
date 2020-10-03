#include <stdio.h>
#include <stdbool.h>
#include <string.h>
//#include "lab8part2lib.h"
//#include "part2.h"
#include <stdlib.h>
#include <time.h>
//*****
//int calculateFlips (char board[][26],int n, int row, int col, char colour, int deltaRow, int deltaCol);
//void chooseMove (char board[][26],int n,char colour,int *row, int *col);
/////
double frontierScoreMultiplier=74.396;
double mobilityScoreMultiplier=78.922;
double pieceDifferenceMultiplier=10;
double valueMatrixMultiplier=10;
double cornersOccupiedMultiplier=801.724;
double cornerScoreMultiplier=382.026;
double movesSearched;
///
void playGame(char board[][8], int n, char cpu1, char cpu2);
void initialBoard(char board[][8], int n);
void printBoard(char board[][8], int n);
bool checkLegalInDirection(char board[][8],int n, int row, int col, char colour, int deltaRow, int deltaCol);
bool availableMoves(char board[][8], int n, char colour);
bool enterMove(char board[][8],int n, char moveColour, int cpuStyle);
bool positionInBounds(int n, int row, int col);
bool checkChangeMove(char board[][8],int n, char colour, int row, int col);
void changeMove(char board[][8],int n, int row, int col, char colour, int deltaRow, int deltaCol);
void winDetermine(char board[][8],int n);
void boardCopy(char board[][8], char newBoard[][8]);
char oppositeColor(char colour);
void compositeScore(char board[][8], char colour, double *frontierScore, double *mobilityScore, double *pieceScore, double *tileScore, double *goodCornerScore, double *badCornerScore);
bool checkForFrontier(char board[][8],int n, int row, int col);
bool legalMove(char board[][8], char colour, int row, int col);
void algo(char board[][8],char colour, int *chosenRow, int *chosenCol);
double miniMax(char board[][8],char myColour, char theirColor, int depth, int *bestRow, int *bestCol, double alpha, double beta);
double evalScore(char board[][8], char colour);
///
/*This program is the first part of an othello game. Othello is a game where you place pieces of an nxn board 
 * and you are trying to get as many pieces of your color as possible. The way you get pieces is by placing a piece that will
 * trap your opponent's pieces and flip them over.
 * The purpose of the main function is to create the board and run the other functions to print and change the board */

int main(void){
    int n;
    printf("Enter the board dimension: "); //should be even
    scanf("%d",&n);
    n=8;
    char board[8][8]={}; //Initializes all terms to 0
    initialBoard(board,n);
    char cpu1, cpu2;
    cpu1='A' ;
    cpu2='E';
    char input;
    printf("Computer plays (B/W): ");
    scanf(" %c",&input);
    if (input=='B'){
        if (n==8){
            cpu1='A';
            cpu2='Z';
        } else {
            cpu1='B';
            cpu2='Z';
        }
    } else if (input=='W'){
        if (n==8){
            cpu1='Z';
            cpu2='C';
        } else {
            cpu1='Z';
            cpu2='W';
        }
    }
    /*TABLE ************************************
     * Black goes first
     * B and W are dumb AI, cpu no.2
     * A and C are smart AI (respectively), cpu no.3
     * D and E are TA AI (respectively), cpu no.1
     * you are cpu no.0
     */
    /*
    srand(time(NULL));
    frontierScoreMultiplier=double(rand()%100);
    mobilityScoreMultiplier=double(rand()%100);
    pieceDifferenceMultiplier=double(rand()%100);
    valueMatrixMultiplier=double(rand()%100);
    cornersOccupiedMultiplier=double(rand()%100);
    cornerScoreMultiplier=double(rand()%100);
    */
    printBoard(board,n);
    playGame(board,n,cpu1,cpu2); 
    winDetermine(board,n);
    /*
    int totalMultiplier=frontierScoreMultiplier+mobilityScoreMultiplier+pieceDifferenceMultiplier+valueMatrixMultiplier;
    printf("Frontier Score Multiplier: %d\n",(100*frontierScoreMultiplier/totalMultiplier));
    printf("Mobility Score Multiplier: %d\n",(100*mobilityScoreMultiplier/totalMultiplier));
    printf("Piece Difference Multiplier: %d\n",(100*pieceDifferenceMultiplier/totalMultiplier));
    printf("Value Matrix Multiplier: %d\n",(100*valueMatrixMultiplier/totalMultiplier));
     */ 
	return 0;
}

void playGame(char board[][8], int n, char cpu1, char cpu2){
    bool gameEnd=false; //true for game ended
    int turn=0, realTurns=0;  //even for turn B, odd for turn W
    while (!gameEnd){
        bool movesForW=availableMoves(board,n,'W'); 
        bool movesForB=availableMoves(board,n,'B');
        bool legalMove=false; 
        if ((!movesForB)&&(!movesForW)) {
            gameEnd=true;
        } else if (turn%2==0) {//turn for B
            if (movesForB) {
                //printf("Black's turn:%d\n",realTurns+1); //delete both term entries
                if (cpu1=='B'){
                    legalMove=enterMove(board,n,'B',2);
                } else if (cpu1=='A') {
                    legalMove=enterMove(board,n,'B',3);
                } else if (cpu1=='D') {
                    legalMove=enterMove(board,n,'B',1);
                } else {
                    legalMove=enterMove(board,n,'B',0);
                }
                if (!legalMove){
                    printf("Invalid Move.\n");
                    printf("W player wins.\n");
                    exit(0);
                }
                printBoard(board,n);
                realTurns++;
            } else {
               printf("B player has no valid move.\n");  
            }
            turn++;
        } else if (turn%2!=0) {//turn for W
            if (movesForW) {
                //printf("White's turn:%d\n",realTurns+1); //delete both term entries
                if (cpu2=='W'){
                    legalMove=enterMove(board,n,'W',2);
                } else if (cpu2=='C') {
                    legalMove=enterMove(board,n,'W',3);
                } else if (cpu2=='E') {
                    legalMove=enterMove(board,n,'W',1);
                } else {
                    legalMove=enterMove(board,n,'W',0);
                }
                if (!legalMove){
                    printf("Invalid Move.\n");
                    printf("B player wins.\n");
                    exit(0);
                }
                printBoard(board,n);
                realTurns++;
            } else {
               printf("W player has no valid move.\n");  
            }
            turn++;
        }
        //char dummy;
        //scanf("%c",&dummy);
    }
    return;
}

//This function sets all the default piece to U, and places B&W pieces on the middle
void initialBoard(char board[][8], int n){
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            board[i][j]='U';
        }
    }
    board[n/2-1][n/2-1]='W';
    board[n/2][n/2]='W';
    board[n/2-1][n/2]='B';
    board[n/2][n/2-1]='B';
}

//This function prints the board on nxn size and the line of letters arounds it
void printBoard(char board[][8], int n){
    for (int i=0; i<n; i++){
        if (i==0){//first row we need to put the row of letters first
            printf("  ");
            for (int letter=0;letter<n;letter++){
                printf("%c",'a'+letter);
            }
            printf("\n");
        }
        for (int j=0;j<n;j++) {
            if (j==0){//every first term of the column we need to print a letter first
                printf("%c ", 'a'+i);
            }
            printf("%c",board[i][j]);
        }
        printf("\n");
    }
}

bool checkLegalInDirection(char board[][8],int n, int row, int col, char colour, int deltaRow, int deltaCol){
    char checkColour=oppositeColor(colour); //opposite color
    int times=1;
    while (positionInBounds(n,row+deltaRow*times,col+deltaCol*times)){ //cannot test out of bound values, auto skip to return false
        if (board[row+deltaRow*times][col+deltaCol*times]==checkColour){//start checking next terms
            times++;
        } else if ((board[row+deltaRow*times][col+deltaCol*times]==colour)){//ends checking
            if (times==1){
                return false;
            } else {
                return true;
            }
        } else if ((board[row+deltaRow*times][col+deltaCol*times]=='U')) {
            return false;
        } else {
            return false;
        }
    }
    return false;
}

/* This function checks for a single available move for a single colour according to row order and then column order */
bool availableMoves(char board[][8], int n, char colour){
    for (int i=0; i<n; i++){
        for (int j=0;j<n;j++) {
            if (board[i][j]=='U'){
                for (int y=-1;y<=1;y++){ //deltaRow, do this more!!!!
                    for (int z=-1;z<=1;z++){ //deltaCol
                        if ((y!=0)||(z!=0)){//they can't both be zero
                            if (checkLegalInDirection(board,n,i,j,colour,y,z)){
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

/*This function prompts the user to enter a move, and then takes the first 3 terms and checks for validity of the move,
 * including: 1. whether it is in bounds, whether it is already filled, whether it is a choosable color, and finally whether it is a valid move
 * If it passes everything, it makes the move. */
bool enterMove(char board[][8],int n, char moveColour, int cpuStyle){
    int row=0, col=0;
    if (cpuStyle==0){
        char moveRow, moveCol;
        printf("Enter move for colour %c (RowCol): ",moveColour);
        scanf(" %c%c",&moveRow,&moveCol);
        row=moveRow-'a';
        col=moveCol-'a';
    } else if (cpuStyle==1){
        int output=0;
        char cpuBoard[26][26]={};
        for (int i=0;i<8;i++){
            for (int j=0;j<8;j++){
                cpuBoard[i][j]=board[i][j];
            }
        }
        //output=findSmartestMove(board,n,moveColour,&row,&col);
        //chooseMove(cpuBoard,8,moveColour,&row,&col);
        printf("This is the output: %d\n", output);
        printf("Testing AI move (row,col): %c%c\n", row+'a', col+'a');
    } else if (cpuStyle==3){
        algo(board,moveColour,&row,&col);
        printf("Computer places %c at %c%c.\n",moveColour,'a'+row,'a'+col);
    }
   if (!(positionInBounds(n,row,col))) { //true means good, so !true to continue testing
        return false; 
        //printf("Invalid move due to bounds.\n"); //delete
    } else if (!(board[row][col]=='U')){
        return false;
        //printf("Invalid move bc it's filled.\n");//delete
    } else if (!(checkChangeMove(board,n,moveColour,row,col))){
        return false;
        //printf("Invalid move bc its not valid.\n");
    } else {
        board[row][col]=moveColour;
        return true;
    }
}

//This function checks whether the input in bounds by checking it the entered row and col is larger or equal to n or smaller than 0
bool positionInBounds(int n, int row, int col){
    if ((row<0)||(row>=n)){
        return false;
    } else if ((col<0)||(col>=n)){
        return false;
    } else {
        return true;
    }
}

/*This function checks whether a move is valid is a direction, if it is, it makes the counter go up, indicating the number of direction of legal moves 
 * it then calls changeMove function to flip the pieces in that direction */
bool checkChangeMove(char board[][8],int n, char colour, int row, int col){
    int counter=0;
    for (int y=-1;y<=1;y++){ //deltaRow, do this more!!!!
        for (int z=-1;z<=1;z++){ //deltaCol
            if ((y!=0)||(z!=0)){//they can't both be zero
                if(checkLegalInDirection(board,n,row,col,colour,y,z)) {
                    changeMove(board,n,row,col,colour,y,z);
                    counter++;
                }
            }
        }
    }
    if (counter>0){ //printf("Valid move.\n");
        return true; //returns true on a valid move
    } else {
        return false;
    }
}

//This function flips the pieces over in the direction that is given from the parameters, it flips over the opposite color until the same color is reached
void changeMove(char board[][8],int n, int row, int col, char colour, int deltaRow, int deltaCol){
    char checkColour=oppositeColor(colour); //opposite color, default color is the one entered
    int times=1;
    while (positionInBounds(n,row+deltaRow*times,col+deltaCol*times)){//should not be out of bounds, but just to be safe
        if (board[row+deltaRow*times][col+deltaCol*times]==checkColour){
            board[row+deltaRow*times][col+deltaCol*times]=colour;
            times++;
        } else {
            return; //if its not the opposite color, leave the function
        }
    }
}

void winDetermine(char board[][8],int n){
    int blackCounter=0, whiteCounter=0, blankCounter=0;
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            if (board[i][j]=='W') {
                whiteCounter++;
            } else if (board[i][j]=='B'){
                blackCounter++;
            } else {
                blankCounter++;
            }
        }
    }
    //printf("Black pieces:%d White pieces:%d Blank pieces:%d\n",blackCounter,whiteCounter,blankCounter);
    if (blackCounter+whiteCounter+blankCounter!=n*n) {
        //printf("Something is wrong with counting!\n");
    } else {
        if (blackCounter>whiteCounter){
            printf("B player wins.\n");
        } else if (whiteCounter>blackCounter){
            printf("W player wins.\n");
        } else {
            printf("Draw!\n");
        }
    }
}

void boardCopy(char board[][8], char newBoard[][8]){
    memcpy(newBoard,board,8*8*sizeof(char));
}

char oppositeColor(char colour){
    if (colour=='B'){ 
        return 'W';
    }else{
        return 'B';
    }
}

void compositeScore(char board[][8], char colour, double *frontierScore, double *mobilityScore, double *pieceScore, double *tileScore, double *goodCornerScore, double *badCornerScore){
    int bFront=0, wFront=0; //front pieces
    int bPiece=0, wPiece=0; //just any piece
    int bMobile=0, wMobile=0; //mobile piece for colours
    double bScore=0, wScore=0; //tile scores for occupied pieces
    int bCorner=0, wCorner=0;
    int bCornerADJ=0, wCornerADJ=0;
    double scoreMatrix[8][8]={
      {20,-3,11,8,8,11,-3,20},
      {-3,-7,-4,1,1,-4,-7,-3},
      {11,-4,2,2,2,2,-4,11},
      {8,1,2,-3,-3,2,1,8},
      {8,1,2,-3,-3,2,1,8},
      {11,-4,2,2,2,2,-4,11},
      {-3,-7,-4,1,1,-4,-7,-3},
      {20,-3,11,8,8,11,-3,20}
    };
    int n=8;
    for (int i=0;i<n;i++){ //scans entire board
        for (int j=0;j<n;j++){
            if ((i==0)&&(j==0)){//top left corner
                switch(board[i][j]){
                    case 'B':
                        bCorner++;
                        if (checkForFrontier(board,n,i,j)){
                            bFront++;
                        }
                        bScore+=scoreMatrix[i][j];
                        bPiece++;
                        break;
                    case 'W':
                        wCorner++;
                        if (checkForFrontier(board,n,i,j)){
                            wFront++;
                        }
                        wScore+=scoreMatrix[i][j];
                        wPiece++;
                        break;
                    case 'U':
                        if (board[0][1]=='B'){
                            bCornerADJ++;
                        } else if (board[0][1]=='W'){
                            wCornerADJ++;
                        }
                        if (board[1][0]=='B'){
                            bCornerADJ++;
                        } else if (board[1][0]=='W'){
                            wCornerADJ++;
                        }
                        if (board[1][1]=='B'){
                            bCornerADJ++;
                        } else if (board[1][1]=='W'){
                            wCornerADJ++;
                        }
                        if (legalMove(board,'B',i,j)){
                            bMobile++;
                        }
                        if (legalMove(board,'W',i,j)){
                            wMobile++;
                        }
                        break;
                }
            } else if ((i==0)&&(j==7)){//top right corner
                switch(board[i][j]){
                    case 'B':
                        bCorner++;
                        if (checkForFrontier(board,n,i,j)){
                            bFront++;
                        }
                        bScore+=scoreMatrix[i][j];
                        bPiece++;
                        break;
                    case 'W':
                        wCorner++;
                        if (checkForFrontier(board,n,i,j)){
                            wFront++;
                        }
                        wScore+=scoreMatrix[i][j];
                        wPiece++;
                        break;
                    case 'U':
                        if (board[0][6]=='B'){
                            bCornerADJ++;
                        } else if (board[0][6]=='W'){
                            wCornerADJ++;
                        }
                        if (board[1][6]=='B'){
                            bCornerADJ++;
                        } else if (board[1][6]=='W'){
                            wCornerADJ++;
                        }
                        if (board[1][7]=='B'){
                            bCornerADJ++;
                        } else if (board[1][7]=='W'){
                            wCornerADJ++;
                        }
                        if (legalMove(board,'B',i,j)){
                            bMobile++;
                        }
                        if (legalMove(board,'W',i,j)){
                            wMobile++;
                        }
                        break;
                }
            } else if ((i==7)&&(j==0)){//bottom left corner
                switch(board[i][j]){
                    case 'B':
                        bCorner++;
                        if (checkForFrontier(board,n,i,j)){
                            bFront++;
                        }
                        bScore+=scoreMatrix[i][j];
                        bPiece++;
                        break;
                    case 'W':
                        wCorner++;
                        if (checkForFrontier(board,n,i,j)){
                            wFront++;
                        }
                        wScore+=scoreMatrix[i][j];
                        wPiece++;
                        break;
                    case 'U':
                        if (board[6][0]=='B'){
                            bCornerADJ++;
                        } else if (board[6][0]=='W'){
                            wCornerADJ++;
                        }
                        if (board[6][1]=='B'){
                            bCornerADJ++;
                        } else if (board[6][1]=='W'){
                            wCornerADJ++;
                        }
                        if (board[7][1]=='B'){
                            bCornerADJ++;
                        } else if (board[7][1]=='W'){
                            wCornerADJ++;
                        }
                        if (legalMove(board,'B',i,j)){
                            bMobile++;
                        }
                        if (legalMove(board,'W',i,j)){
                            wMobile++;
                        }
                        break;
                }
            } else if ((i==7)&&(j==7)){//bottom right coner
                switch(board[i][j]){
                    case 'B':
                        bCorner++;
                        if (checkForFrontier(board,n,i,j)){
                            bFront++;
                        }
                        bPiece++;
                        bScore+=scoreMatrix[i][j];
                        break;
                    case 'W':
                        wCorner++;
                        if (checkForFrontier(board,n,i,j)){
                            wFront++;
                        }
                        wScore+=scoreMatrix[i][j];
                        wPiece++;
                        break;
                    case 'U':
                        if (board[6][6]=='B'){
                            bCornerADJ++;
                        } else if (board[6][6]=='W'){
                            wCornerADJ++;
                        }
                        if (board[6][7]=='B'){
                            bCornerADJ++;
                        } else if (board[6][7]=='W'){
                            wCornerADJ++;
                        }
                        if (board[7][6]=='B'){
                            bCornerADJ++;
                        } else if (board[7][6]=='W'){
                            wCornerADJ++;
                        }
                        if (legalMove(board,'B',i,j)){
                            bMobile++;
                        }
                        if (legalMove(board,'W',i,j)){
                            wMobile++;
                        }
                        break;
                }
            } else {
                switch(board[i][j]){//anything but corners
                    case 'B': //frontierpiece, valueScore, justpiece
                        if (checkForFrontier(board,n,i,j)){
                            bFront++;
                        }
                        bScore+=scoreMatrix[i][j];
                        bPiece++;
                        break;
                    case 'W': //then add piece score, then calculate valueScore
                        if (checkForFrontier(board,n,i,j)){
                            wFront++;
                        }
                        wScore+=scoreMatrix[i][j];
                        wPiece++;
                        if (checkForFrontier(board,n,i,j)){
                            wFront++;
                        }
                        wScore+=scoreMatrix[i][j];
                        wPiece++;
                        break;
                    case 'U':
                        if (legalMove(board,'B',i,j)){
                            bMobile++;
                        }
                        if (legalMove(board,'W',i,j)){
                            wMobile++;
                        }
                        break;
                }   
            }
        }
    }
    if (colour=='B'){
        if (bFront>wFront){
            *frontierScore=-100*(bFront)/(bFront+wFront);
        } else if (wFront>bFront){
            *frontierScore=100*(wFront)/(bFront+wFront);
        } else {
            *frontierScore=0;
        }
        if (bPiece>wPiece){
            *pieceScore=100*(bPiece)/(bPiece+wPiece);
        } else if (wPiece>bPiece){
            *pieceScore=-100*(wPiece)/(bPiece+wPiece);
        } else {
            *pieceScore=0;
        }
        if (bMobile>wMobile){
            *mobilityScore=100*(bMobile)/(bMobile+wMobile);
        } else if (wMobile>bMobile){
            *mobilityScore=-100*(wMobile)/(bMobile+wMobile);
        } else {
            *mobilityScore=0;
        }
        *tileScore=bScore-wScore;
        *goodCornerScore=25*(bCorner-wCorner);
        *badCornerScore=-12.5*(bCornerADJ-wCornerADJ);
    } else {
        if (wFront>bFront){
            *frontierScore=-100*(wFront)/(bFront+wFront);
        } else if (wFront>bFront){
            *frontierScore=100*(bFront)/(bFront+wFront);
        } else {
            *frontierScore=0;
        }
        if (wPiece>bPiece){
            *pieceScore=100*(wPiece)/(bPiece+wPiece);
        } else if (bPiece>wPiece){
            *pieceScore=-100*(bPiece)/(bPiece+wPiece);
        } else {
            *pieceScore=0;
        }
        if (wMobile>bMobile){
            *mobilityScore=100*(wMobile)/(bMobile+wMobile);
        } else if (bMobile>wMobile){
            *mobilityScore=-100*(bMobile)/(bMobile+wMobile);
        } else {
            *mobilityScore=0;
        }
        *tileScore=wScore-bScore;
        *goodCornerScore=25*(wCorner-bCorner);
        *badCornerScore=-12.5*(wCornerADJ-bCornerADJ);
    }
}

bool checkForFrontier(char board[][8],int n, int row, int col){
    for (int y=-1;y<=1;y++){
        for (int z=-1;z<=1;z++){
            if ((y!=0)||(z!=0)){
                if (positionInBounds(n,row+y,col+z)){
                    if(board[row+y][row+z]=='U'){
                        return true; //there is opening
                    }
                }
            }
        }
    }
    return false; //no openings 
}

bool legalMove(char board[][8], char colour, int row, int col){//tests for one value of rowcol, whether it has a least one legal direction move for that colour
    for (int y=-1;y<=1;y++){
        for (int z=-1;z<=1;z++){
            if ((y!=0)||(z!=0)){
                if (checkLegalInDirection(board,8,row,col,colour,y,z)){
                    return true;
                }
            }
        }
    }
    return false;
}

void algo(char board[][8],char colour, int *chosenRow, int *chosenCol){
    //board transform
    char opponentColor=oppositeColor(colour);
    char newBoard[8][8]={};
    boardCopy(board,newBoard);
    int depth=1;
    movesSearched=0;
    double alpha=-500000;
    double beta=500000;
    double finalScore=miniMax(newBoard,colour,opponentColor,depth,chosenRow,chosenCol,alpha,beta);
    //printf("%lf\n",movesSearched);
    //printf("The highest score is %.4lf and (%c)(%c)\n",finalScore,'a'+*chosenRow,'a'+*chosenCol);
}

double miniMax(char board[][8],char myColour, char theirColor, int depth, int *bestRow, int *bestCol, double alpha, double beta){ //board here should be shortBoard
    //depth 1 is my turn right now
    char colour;
    double maxScore;
    if (depth%2==1){ //odd numbers are my turn
        colour=myColour;
        maxScore=-500000;
    } else {
        colour=theirColor;
        maxScore=500000;
    }
    int maxdepth=10;
    int moveList[60][2]={};
    int listLength=0;
    int bestMoveRow, bestMoveCol;
    if (depth==maxdepth){
        return evalScore(board,myColour); //always gonna eval myColor
    } else {
        for (int i=0;i<8;i++){
            for (int j=0;j<8;j++){
                if (board[i][j]=='U'){
                    if (legalMove(board,colour,i,j)){
                        moveList[listLength][0]=i;
                        moveList[listLength][1]=j;
                        listLength++;
                    }
                }
            }
        }
        if (listLength==0){
            return evalScore(board,myColour);
        } else {
            for (int i=0;i<listLength;i++){
                board[moveList[i][0]][moveList[i][1]]=colour;
                double score=miniMax(board,myColour,theirColor,depth+1,bestRow,bestCol,alpha,beta);
                if (depth%2==1){ //odd numbers are my turn
                    if (score>maxScore) {
                        bestMoveRow=moveList[i][0];
                        bestMoveCol=moveList[i][1];
                        maxScore=score;
                    }
                    if (maxScore>=beta){ //
                        return maxScore;
                    }
                    if (maxScore>alpha){
                        alpha=maxScore;
                    }
                } else {
                    if (score<maxScore) {
                        bestMoveRow=moveList[i][0];
                        bestMoveCol=moveList[i][1];
                        maxScore=score;
                    }
                    if (maxScore<=alpha){
                        return maxScore;
                    }
                    if (maxScore<beta){
                        beta=maxScore;
                    }
                }
                board[moveList[i][0]][moveList[i][1]]='U';
            }
            *bestRow=bestMoveRow;
            *bestCol=bestMoveCol;
            return maxScore;
        }
    }
}

double evalScore(char board[][8], char colour) {
    double score=0;
    double frontierScore=0, mobilityScore=0, pieceDifference=0, valueMatrix=0, cornersOccupied=0, cornerScore=0;
    compositeScore(board,colour,&frontierScore,&mobilityScore,&pieceDifference,&valueMatrix,&cornersOccupied,&cornerScore);
    score+=frontierScore*frontierScoreMultiplier;
    score+=mobilityScore*mobilityScoreMultiplier;
    score+=pieceDifference*pieceDifferenceMultiplier;
    score+=valueMatrix*valueMatrixMultiplier;
    score+=cornersOccupied*cornersOccupiedMultiplier;
    score+=cornerScore*cornerScoreMultiplier;
    movesSearched++; 
    return score;
}

   /*     {3.2125,1.775,1.875,1.975,1.975,1.875,1.775,3.2125},
        {0.15,2.3,0.6625,1.8375,1.8375,0.6625,2.3,0.15},
        {3.525,0.85,2.675,0.175,0.175,2.675,0.85,3.525},
        {1.125,1.95,0.15,0,0,0.15,1.95,1.125},
        {1.125,1.95,0.15,0,0,0.15,1.95,1.125},
        {3.525,0.85,2.675,0.175,0.175,2.675,0.85,3.525},
        {0.15,2.3,0.6625,1.8375,1.8375,0.6625,2.3,0.15},
        {3.2125,1.775,1.875,1.975,1.975,1.875,1.775,3.2125}
    
    {4,-3,2,2,2,2,-3,4},
    {-3,-4,-1,-1,-1,-1,-4,-3},
    {2,-1,1,0,0,1,-1,2},
    {2,-1,0,1,1,0,-1,2},
    {2,-1,0,1,1,0,-1,2},
    {2,-1,1,0,0,1,-1,2},
    {-3,-4,-1,-1,-1,-1,-4,-3},
    {4,-3,2,2,2,2,-3,4}
     
     {120,-20,20,5,5,20,-20,120},
     {-20,-40,-5,-5,-5,-5,-40,-20},
     {20,-5,15,3,3,15,-5,20},
     {5,-5,3,3,3,3,-5,5},
     {5,-5,3,3,3,3,-5,5},
     {20,-5,15,3,3,15,-5,20},
     {-20,-40,-5,-5,-5,-5,-40,-20},
     {120,-20,20,5,5,20,-20,120}
      */

/////
/*
int calculateFlips (char board[][26],int n, int row, int col, char colour, int deltaRow, int deltaCol) {
    char checkColour; //opposite color
    int times=1;
    if (colour=='B'){ 
        checkColour='W';
    }else{
        checkColour='B';
    }
    while (positionInBounds(n,row+deltaRow*times,col+deltaCol*times)){ 
        //cannot test out of bound values, auto skip to return false
        if (board[row+deltaRow*times][col+deltaCol*times]==checkColour){//start checking next terms
            times++;
        } else if ((board[row+deltaRow*times][col+deltaCol*times]==colour)){//ends checking
            if (times==1){
                //printf("Returning false due to same value %d%d\n", row,col);
                return 0;
            } else {//indicates entrapment
                //printf("Returning true! %d%d\n",row,col);
                return times-1; //pieces flip is how many times-1
            }
        } else if ((board[row+deltaRow*times][col+deltaCol*times]=='U')) {
            //printf("Returning false due to value U %d%d\n",row,col);
            return 0;
        } else {
            printf("Something is wrong %d%d!\n",row,col);
            return 0;
        }
    }
    //printf("Out of bound value %d%d\n",row,col);
    return 0;
}

void chooseMove (char board[][26],int n,char colour,int *row, int *col) {
    int flips, mostFlips=0;
    int storeRow, storeCol;
    for (int i=0; i<n; i++){ 
        for (int j=0;j<n;j++) {
            if (board[i][j]=='U'){
                flips=0;
                for (int y=-1;y<=1;y++){ //deltaRow, do this more!!!!
                    for (int z=-1;z<=1;z++){ //deltaCol
                        if ((y!=0)||(z!=0)){//they can't both be zero
                            flips+=calculateFlips(board,n,i,j,colour,y,z);
                            if (flips>mostFlips) {
                                //printf("Current most flips is %c%c(%d)\n",'a'+i,'a'+j,flips);
                                mostFlips=flips;
                                storeRow=i;
                                storeCol=j;
                            }
                        }
                    }
                }
            }
        }
    }
    *row=storeRow;
    *col=storeCol;
}
*/

//add/delete headers, such as AI functions, and 2 #includes
//change default players
//change AIs (uncomment/comment) in enterMove
//activate functions if needed (for my AIs)
//remove turn counter
//remove score & movesSearched
//remove final tiles count
