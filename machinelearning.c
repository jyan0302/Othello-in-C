#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lab8part2lib.h"
#include "part2.h"
#include <stdlib.h>
#include <time.h>
int frontierScoreMultiplier=29;
int mobilityScoreMultiplier=30;
int pieceDifferenceMultiplier=12;
int valueMatrixMultiplier=28;
bool smartest=false;

/*This program is the first part of an othello game. Othello is a game where you place pieces of an nxn board 
 * and you are trying to get as many pieces of your color as possible. The way you get pieces is by placing a piece that will
 * trap your opponent's pieces and flip them over.
 * The purpose of the main function is to create the board and run the other functions to print and change the board */

int main(void){
    int dim;
    printf("Enter the board dimension: "); //should be even
    scanf("%d",&dim);
    char cpu1, cpu2;
    /* char input;
    printf("Computer plays (B/W): ");
    scanf(" %c",&input);
    if (input=='B'){
        if (dim==8){
            cpu1='A';
            cpu2='Z';
        } else {
            cpu1='B';
            cpu2='Z';
        }
    } else if (input=='W'){
        if (dim==8){
            cpu1='Z';
            cpu2='C';
        } else {
            cpu1='Z';
            cpu2='W';
        }
    }
     */ 
    /*TABLE ************************************
     * Black goes first
     * B and W are dumb AI, cpu no.2
     * A and C are smart AI (respectively), cpu no.3
     * D and E are TA AI (respectively), cpu no.1
     * you are cpu no.0
     */
    /*
    srand(time(NULL));
    frontierScoreMultiplier+=(rand()%5)-2;
    mobilityScoreMultiplier+=(rand()%5)-2;
    pieceDifferenceMultiplier+=(rand()%5)-2;
    valueMatrixMultiplier+=(rand()%5)-2;
    */
    char board[26][26]={}; //Initializes all terms to 0
    initialBoard(board,dim);
    cpu1='A' ;
    cpu2='W';
    playGame(board,dim,cpu1,cpu2); 
    //****
    char secondBoard[26][26]={}; //Initializes all terms to 0
    initialBoard(secondBoard,dim);
    cpu1='B' ;
    cpu2='C';
    playGame(secondBoard,dim,cpu1,cpu2);
    //****
    char thirdBoard[26][26]={}; //Initializes all terms to 0
    initialBoard(thirdBoard,dim);
    cpu1='A' ;
    cpu2='E';
    playGame(thirdBoard,dim,cpu1,cpu2); 
    //****
    char fourthBoard[26][26]={}; //Initializes all terms to 0
    initialBoard(fourthBoard,dim);
    cpu1='D' ;
    cpu2='C';
    playGame(fourthBoard,dim,cpu1,cpu2);
    //****
    char fifthBoard[26][26]={}; //Initializes all terms to 0
    initialBoard(fifthBoard,dim);
    cpu1='A' ;
    cpu2='E';
    smartest=true;
    playGame(fifthBoard,dim,cpu1,cpu2); 
    //****
    char sixthBoard[26][26]={}; //Initializes all terms to 0
    initialBoard(sixthBoard,dim);
    cpu1='D' ;
    cpu2='C';
    playGame(sixthBoard,dim,cpu1,cpu2);
    //****
    printf("Against noob AI\n");
    winDetermine(board,dim);
    winDetermine(secondBoard,dim);
    printf("Against Smarter AI\n");
    winDetermine(thirdBoard,dim);
    winDetermine(fourthBoard,dim);
    printf("Against Smartest AI\n");
    winDetermine(fifthBoard,dim);
    winDetermine(sixthBoard,dim);
    int totalMultiplier=frontierScoreMultiplier+mobilityScoreMultiplier+pieceDifferenceMultiplier+valueMatrixMultiplier;
    printf("*******\n");
    printf("Frontier Score Multiplier: %d\n",(100*frontierScoreMultiplier/totalMultiplier));
    printf("Mobility Score Multiplier: %d\n",(100*mobilityScoreMultiplier/totalMultiplier));
    printf("Piece Difference Multiplier: %d\n",(100*pieceDifferenceMultiplier/totalMultiplier));
    printf("Value Matrix Multiplier: %d\n",(100*valueMatrixMultiplier/totalMultiplier));
    printf("*******\n");
	return 0;
}

void playGame(char board[][26], int dim, char cpu1, char cpu2){
    bool gameEnd=false; //true for game ended
    int turn=0, realTurns=0;  //even for turn B, odd for turn W
    while (!gameEnd){
        bool movesForW=availableMoves(board,dim,'W'); 
        bool movesForB=availableMoves(board,dim,'B');
        bool legalMove=false; 
        if ((!movesForB)&&(!movesForW)) {
            gameEnd=true;
        } else if (turn%2==0) {//turn for B
            if (movesForB) {
                printBoard(board,dim);
                //printf("Black's turn:%d\n",realTurns+1); //delete both term entries
                if (cpu1=='B'){
                    legalMove=enterMove(board,dim,'B',2);
                } else if (cpu1=='A') {
                    legalMove=enterMove(board,dim,'B',3);
                } else if (cpu1=='D') {
                    legalMove=enterMove(board,dim,'B',1);
                } else {
                    legalMove=enterMove(board,dim,'B',0);
                }
                if (!legalMove){
                    printf("Invalid Move.\n");
                    printf("W player wins.\n");
                    exit(0);
                }
                realTurns++;
            } else {
               printf("B player has no valid move.\n");  
            }
            turn++;
        } else if (turn%2!=0) {//turn for W
            if (movesForW) {
                printBoard(board,dim);
                //printf("White's turn:%d\n",realTurns+1); //delete both term entries
                if (cpu2=='W'){
                    legalMove=enterMove(board,dim,'W',2);
                } else if (cpu2=='C') {
                    legalMove=enterMove(board,dim,'W',3);
                } else if (cpu2=='E') {
                    legalMove=enterMove(board,dim,'W',1);
                } else {
                    legalMove=enterMove(board,dim,'W',0);
                }
                if (!legalMove){
                    printf("Invalid Move.\n");
                    printf("B player wins.\n");
                    exit(0);
                }
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
void initialBoard(char board[][26], int n){
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
void printBoard(char board[][26], int n){
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

//This fucntion changes the initial board according to user settings, and assuming user input is valid
void boardConfig(char board[][26], int n){
    char config[4];
    printf("Enter board configuration:\n");
    while ((config[0]!='!')||(config[1]!='!')||(config[2]!='!')){
        scanf("%s",config);
        board[config[1]-'a'][config[2]-'a']=config[0];
    }
}

/*This function checks for a legal move it a particular direction from the assigned piece, whether North, South, East, or West
 * it checks for a legal move by seeing it the first piece after is the opposite color, and whether after the opposite color pieces there is
 * a same colored pieced, indicating entrapment */
bool checkLegalInDirection(char board[][26],int n, int row, int col, char colour, int deltaRow, int deltaCol){
    char checkColour=oppositeColor(colour); //opposite color
    int times=1;
    while (positionInBounds(n,row+deltaRow*times,col+deltaCol*times)){
        //cannot test out of bound values, auto skip to return false
        if (board[row+deltaRow*times][col+deltaCol*times]==checkColour){//start checking next terms
            times++;
        } else if ((board[row+deltaRow*times][col+deltaCol*times]==colour)){//ends checking
            if (times==1){
                //printf("Returning false due to same value %d%d\n", row,col);
                return false;
            } else {//indicates entrapment
                //printf("Returning true! %d%d\n",row,col);
                return true;
            }
        } else if ((board[row+deltaRow*times][col+deltaCol*times]=='U')) {
            //printf("Returning false due to value U %d%d\n",row,col);
            return false;
        } else {
            //printf("Something is wrong %d%d!\n",row,col);
            return false;
        }
    }
    //printf("Out of bound value %d%d\n",row,col);
    return false;
}

/* This function checks for available moves in any given game state by checking for the available moves for W for
 * every term on the board that is not occupied for all directions, then does the same for B, and it prints it according to
 * row order and then column order */
bool availableMoves(char board[][26], int n, char colour){
    //printf("\nAvailable moves for %c:\n", colour);
    int movesAvailable=0;
    int flips=0,antiflips=0;
    for (int i=0; i<n; i++){
        for (int j=0;j<n;j++) {
            if (board[i][j]=='U'){
                flips=0;
                for (int y=-1;y<=1;y++){ //deltaRow, do this more!!!!
                    for (int z=-1;z<=1;z++){ //deltaCol
                        if ((y!=0)||(z!=0)){//they can't both be zero
                            flips+=calculateFlips(board,n,i,j,colour,y,z);
                        }
                    }
                }
                antiflips=0;
                for (int y=-1;y<=1;y++){ //deltaRow, do this more!!!!
                    for (int z=-1;z<=1;z++){ //deltaCol
                        if ((y!=0)||(z!=0)){//they can't both be zero
                            if (calculateStab(board,n,i,j,colour,y,z)<antiflips){//Top
                                antiflips=calculateStab(board,n,i,j,colour,y,z); //inefficient
                            }
                        }
                    }
                }
                if (flips>0) {
                    //printf("%c%c(%d)(%d)\t",'a'+i,'a'+j,flips,antiflips);
                    movesAvailable++;
                }
                
            }
        }
    }
    //printf("\nAvailable moves:%d\n\n",movesAvailable);
    //printf("\nAvailable moves for %c:%d\n", colour,movesAvailable); //only for pro gameplay
    if (movesAvailable>0) {
        return true;
    } else {
        return false;
    }
}

/*This function prompts the user to enter a move, and then takes the first 3 terms and checks for validity of the move,
 * including: 1. whether it is in bounds, whether it is already filled, whether it is a choosable color, and finally whether it is a valid move
 * If it passes everything, it makes the move. */
bool enterMove(char board[][26],int n, char moveColour, int cpuStyle){
    int row=0, col=0;
    if (cpuStyle==0){
        char moveRow, moveCol;
        printf("Enter move for colour %c (RowCol): ",moveColour);
        scanf(" %c%c",&moveRow,&moveCol);
        row=moveRow-'a';
        col=moveCol-'a';
    } else if (cpuStyle==1){
        int output=0;
        if (smartest){
            output=findSmartestMove(board,n,moveColour,&row,&col);            //*********uncomment
        } else {
            output=findSmarterMove(board,n,moveColour,&row,&col);            //*********uncomment
        }
        //printf("This is the output: %d\n", output);
        printf("Testing AI move (row,col): %c%c\n", row+'a', col+'a');
    } else if (cpuStyle==2){
        chooseMove(board,n,moveColour);
        return true;
    } else if (cpuStyle==3){
        algo(board,moveColour,&row,&col);
        if (!(failSafe(board,moveColour,row,col))){
            //chooseMove(board,n,moveColour);                   //*********uncomment
            //("FAILLLLLL\n");
            return true;
        }
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
bool checkChangeMove(char board[][26],int n, char colour, int row, int col){
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
    if (counter>0){
        //printf("Valid move.\n");
        return true; //returns true on a valid move
    } else {
        return false;
    }
}

//This function flips the pieces over in the direction that is given from the parameters, it flips over the opposite color until the same color is reached
void changeMove(char board[][26],int n, int row, int col, char colour, int deltaRow, int deltaCol){
    char checkColour=oppositeColor(colour); //opposite color, default color is the one entered
    int times=1;
    while (positionInBounds(n,row+deltaRow*times,col+deltaCol*times)){//should not be out of bounds, but just to be safe
        if (board[row+deltaRow*times][col+deltaCol*times]==checkColour){
            board[row+deltaRow*times][col+deltaCol*times]=colour;
            times++;
        } else {
            return; //if its not that same color, leave the function
        }
    }
}

void winDetermine(char board[][26],int n){
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
    printf("Black pieces:%d White pieces:%d Blank pieces:%d\n",blackCounter,whiteCounter,blankCounter); //*********comment
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

int calculateFlips (char board[][26],int n, int row, int col, char colour, int deltaRow, int deltaCol) {//does this check for legality as well? yes
    char checkColour=oppositeColor(colour); //opposite color
    int times=1;
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
            //printf("Something is wrong %d%d!\n",row,col);
            return 0;
        }
    }
    //printf("Out of bound value %d%d\n",row,col);
    return 0;
}

void chooseMove (char board[][26],int n,char colour) {
    int flips, stabs, mostFlips=0;
    int storeRow, storeCol;
    for (int i=n-1; i>=0; i--){ 
        for (int j=n-1;j>=0;j--) {
            if (board[i][j]=='U'){
                flips=0;
                for (int y=-1;y<=1;y++){ //deltaRow, do this more!!!!
                    for (int z=-1;z<=1;z++){ //deltaCol
                        if ((y!=0)||(z!=0)){//they can't both be zero
                            flips+=calculateFlips(board,n,i,j,colour,y,z);
                        }
                    }
                }
                stabs=0;
                for (int y=-1;y<=1;y++){ //deltaRow, do this more!!!!
                    for (int z=-1;z<=1;z++){ //deltaCol
                        if ((y!=0)||(z!=0)){//they can't both be zero
                            if (calculateStab(board,n,i,j,colour,y,z)<stabs){//Top
                                stabs=calculateStab(board,n,i,j,colour,y,z); //inefficient
                            }
                        }
                    }
                }
                if (flips>0){
                    flips+=stabs; //offset
                    if ((i==0)||(i==n-1)){
                        flips*=2;
                    }
                    if ((j==0)||(j==n-1)){
                        flips*=2;
                    }
                    if ((i==1)||(i==n-2)){
                        flips-=100;
                    }
                    if ((j==1)||(j==n-2)){
                        flips-=100;
                    }
                    if ((mostFlips==0)||(flips>mostFlips)){//just in case flip is always negative
                        mostFlips=flips;
                        storeRow=i;
                        storeCol=j;
                    }
                }
            }
        }
    }
    printf("Computer places %c at %c%c.\n",colour,'a'+storeRow,'a'+storeCol);
    cpuChangeMove(board,n,colour,storeRow,storeCol);
    board[storeRow][storeCol]=colour;
}

void cpuChangeMove(char board[][26],int n, char colour, int row, int col){
    for (int y=-1;y<=1;y++){ //deltaRow, do this more!!!!
        for (int z=-1;z<=1;z++){ //deltaCol
            if ((y!=0)||(z!=0)){//they can't both be zero
                if (checkLegalInDirection(board,n,row,col,colour,y,z)) {//top
                    changeMove(board,n,row,col,colour,y,z);
                }
            }
        }
    }
}

//checks for blanks, which and calculates how many pieces the opponent can get from placing there
int calculateStab(char board[][26],int n, int row, int col, char colour, int deltaRow, int deltaCol){
    char checkColour=oppositeColor(colour); //opposite color, opponents color
    int stabs=0;
    if (positionInBounds(n,row+deltaRow,col+deltaCol)){ 
        //cannot test out of bound values, auto skip to return false
        if (board[row+deltaRow][col+deltaCol]=='U'){//start checking reverse terms
            char newBoard[26][26]={};
            bigBoardCopy(board,newBoard,n);
            newBoard[row][col]=colour; //so that calculateflips will work properly
            if (checkLegalInDirection(newBoard,n,row,col,colour,deltaRow*-1,deltaCol*-1)) {//inefficient, call board many times
                changeMove(newBoard,n,row,col,colour,deltaRow*-1,deltaCol*-1); //changes new board to get ready to check
                stabs=calculateFlips(newBoard,n,row+deltaRow,col+deltaCol,checkColour,deltaRow*-1,deltaCol*-1); //check how many opponent can flip in the opposite directionflip
                return stabs*-1;
            }
        } else if ((board[row+deltaRow][col+deltaCol]==colour)){//this is good we want adjacent pieces
            //nothing happens here 
        } else { //its hits opponent piece, this is not as good
            //nothing happens here
        }
    }
    //printf("Out of bound value %d%d\n",row,col);
    return 0;
}

void bigBoardCopy(char board[][26], char newBoard[][26],int n){
    memcpy(newBoard,board,26*26*sizeof(char));
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

double frontierScore(char board[][8],char colour){
    //pass new board, colour of current turn, n should be 8, returns positive frontier disk value
    int bFront=0, wFront=0;
    int n=8;
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            switch(board[i][j]){
                case 'B':
                    if (checkForFrontier(board,n,i,j)){
                        bFront++;
                    }
                    break;
                case 'W':
                    if (checkForFrontier(board,n,i,j)){
                        wFront++;
                    }
                    break;
                case 'U':
                    break;
            }
        }
    }
    double score;
    if ((bFront+wFront)==0){
        score=0;
    } else {
        if (colour=='B'){
            if (bFront>wFront){
                score=-100*(bFront)/(bFront+wFront);
            } else if (wFront>bFront){
                score=100*(wFront)/(bFront+wFront);
            } else {
                score=0;
            }
        } else {
            if (wFront>bFront){
                score=-100*(wFront)/(bFront+wFront);
            } else if (wFront>bFront){
                score=100*(bFront)/(bFront+wFront);
            } else {
                score=0;
            }
        }
    }
    return score;
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

double mobilityScore(char board[][8], char colour){
    int bMoves=0, wMoves=0;
    int n=8;
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            if (board[i][j]=='U'){
                if (legalMove(board,'B',i,j)){
                    bMoves++;
                }
                if (legalMove(board,'W',i,j)){
                    wMoves++;
                }
            }
        }
    }
    double score;
    if ((bMoves+wMoves)==0){
        score=0;
    } else {
        if (colour=='B'){
            if (bMoves>wMoves){
                score=100*(bMoves)/(bMoves+wMoves);
            } else if (wMoves>bMoves){
                score=-100*(wMoves)/(bMoves+wMoves);
            } else {
                score=0;
            }
        } else {
            if (wMoves>bMoves){
                score=100*(wMoves)/(bMoves+wMoves);
            } else if (bMoves>wMoves){
                score=-100*(bMoves)/(bMoves+wMoves);
            } else {
                score=0;
            }
        }
    }
    return score;
}

bool legalMove(char board[][8], char colour, int row, int col){
    //tests for one value of rowcol, whether it has a least one legal direction move for that colour
    for (int y=-1;y<=1;y++){
        for (int z=-1;z<=1;z++){
            if ((y!=0)||(z!=0)){
                if (checkLegal(board,8,row,col,colour,y,z)){
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkLegal(char board[][8],int n, int row, int col, char colour, int deltaRow, int deltaCol){
    char checkColour=oppositeColor(colour); //opposite color
    int times=1;
    while (positionInBounds(n,row+deltaRow*times,col+deltaCol*times)){
        if (board[row+deltaRow*times][col+deltaCol*times]==checkColour){
            times++;
        } else if ((board[row+deltaRow*times][col+deltaCol*times]==colour)){
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

double pieceDifference(char board[][8], char colour) { //does everything in function
    int n=8;
    int bCounter=0, wCounter=0;
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            if (board[i][j]=='W') {
                wCounter++;
            } else if (board[i][j]=='B'){
                bCounter++;
            } 
        }
    }
    double score;
    if ((bCounter+wCounter)==0){
        score=0;
    } else {
        if (colour=='B'){
            if (bCounter>wCounter){
                score=100*(bCounter)/(bCounter+wCounter);
            } else if (wCounter>bCounter){
                score=-100*(wCounter)/(bCounter+wCounter);
            } else {
                score=0;
            }
        } else {
            if (wCounter>bCounter){
                score=100*(wCounter)/(bCounter+wCounter);
            } else if (wCounter>bCounter){
                score=-100*(bCounter)/(bCounter+wCounter);
            } else {
                score=0;
            }
        }
    }
    return score;
}

double valueMatrix(char board[][8], char colour) {
    int n=8;
    double score=0;
    double scoreMatrix[8][8]={
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
     */
     {120,-20,20,5,5,20,-20,120},
     {-20,-40,-5,-5,-5,-5,-40,-20},
     {20,-5,15,3,3,15,-5,20},
     {5,-5,3,3,3,3,-5,5},
     {5,-5,3,3,3,3,-5,5},
     {20,-5,15,3,3,15,-5,20},
     {-20,-40,-5,-5,-5,-5,-40,-20},
     {120,-20,20,5,5,20,-20,120}
    };
    
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            if (board[i][j]=='B'){
                score+=scoreMatrix[i][j];
            } else if (board[i][j]=='W'){
                score-=scoreMatrix[i][j];
            }
        }
    }
    if (colour=='B'){
        return score;
    } else {
        return score*-1;
    }
}

void algo(char board[][26],char colour, int *chosenRow, int *chosenCol){
    //board transform
    char opponentColor=oppositeColor(colour);
    char shortBoard[8][8]={};
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            shortBoard[i][j]=board[i][j];
        }
    }
    int depth=1;
    double finalScore;
    finalScore=miniMax(shortBoard,colour,opponentColor,depth,chosenRow,chosenCol);
    //printf("The highest score is %.4lf and (%c)(%c)\n",finalScore,'a'+*chosenRow,'a'+*chosenCol);
}

bool failSafe(char board[][26], char colour, int row, int col){
    int n=8;
    if (!(positionInBounds(n,row,col))) {
        return false; 
    } else if (!(board[row][col]=='U')){
        return false;
    } else if (!(checkMove(board,n,colour,row,col))){
        return false;
    } else {
        return true;
    }
}

bool checkMove(char board[][26],int n, char colour, int row, int col){
    for (int y=-1;y<=1;y++){ //deltaRow, do this more!!!!
        for (int z=-1;z<=1;z++){ //deltaCol
            if ((y!=0)||(z!=0)){//they can't both be zero
                if(checkLegalInDirection(board,n,row,col,colour,y,z)) {
                    return true;
                }
            }
        }
    }
    return false;
}

double miniMax(char board[][8],char myColour, char theirColor, int depth, int *bestRow, int *bestCol){ //board here should be shortBoard
    //depth 1 is my turn right now
    char colour;
    double maxScore;
    if (depth%2==1){ //odd numbers are my turn
        colour=myColour;
        maxScore=-10000;
    } else {
        colour=theirColor;
        maxScore=10000;
    }
    int maxdepth=5;
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
                double score=miniMax(board,myColour,theirColor,depth+1,bestRow,bestCol);
                if (depth%2==1){ //odd numbers are my turn
                    if (score>maxScore) {
                        bestMoveRow=moveList[i][0];
                        bestMoveCol=moveList[i][1];
                        maxScore=score;
                    }
                } else {
                    if (score<maxScore) {
                        bestMoveRow=moveList[i][0];
                        bestMoveCol=moveList[i][1];
                        maxScore=score;
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
    score+=frontierScore(board,colour)*frontierScoreMultiplier/100;
    score+=mobilityScore(board,colour)*mobilityScoreMultiplier/100;
    score+=pieceDifference(board,colour)*pieceDifferenceMultiplier/100;
    score+=valueMatrix(board,colour)*valueMatrixMultiplier/100;
    return score;
}



