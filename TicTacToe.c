#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Αρχικοποίηση μεγέθους πινάκων
//Initialization table size
#define MAXTABLE 25 
#define USETABLE 9
//Αρχικοποίηση μεταβλητών και πινάκων
//Initialization variable and tables
int board[ MAXTABLE ];
enum {X, O, E, B};
enum {HUMAN_WIN, COMPUTER_WIN, DRAW};

//Χρησιμοποιείται για την μετατροπή του πίνακα με τα όρια σε 
//πίνακα με μεταβλητές από 0-8 που θα δίνει ο χρήστης
//It is used to transform the table with boundaries into
//table which accept variables in a range from 0 to 8.
const int convertTo25[USETABLE] = {
    6,7,8,
    11,12,13,
    16,17,18
};

const int InMiddle = 4;
const int Corners[4] = {0, 2, 6, 8};

//Για τις κατευθύνσεις που θα ελέγχει τη νίκη   
//It is for the directions which are used to check the winner
const int Directions[4] = {1, 5, 4, 6};

int findThreeInARow(const int ourIndex, const int side){
    int dirIndex = 0, dir = 0, threeCount = 1;
    
    for(dirIndex = 0; dirIndex < 4; dirIndex++){
        dir = Directions[dirIndex];
        threeCount += getDirection(ourIndex + dir, dir, side);
        threeCount += getDirection(ourIndex + dir * -1, dir * -1, side);
        if (threeCount == 3) break;
        threeCount = 1;
    }
    return threeCount;
}

int getDirection(int startSq, const int dir, const int side){
    int found = 0;
    while(board[startSq] !=  B){
        if(board[startSq] != side) break;
        found++;
        startSq += dir;
    }
    return found;
}

//Αρχικοποίηση πίνακα 
//Initialization table
void initBoard(){
    for (int i=0; i<MAXTABLE; i++){
        board[i] = B;
    }
    for (int i=0; i<USETABLE; i++){
        board[ convertTo25[i] ] = E;
    }
}

//Εκτύπωση Board
//Print Board
void printBoard(){
    char intToChar[] = "XO-+";
    printf("\nBoard\n");
    for (int i=0; i<USETABLE; i++){  
        if (i%3 == 0  && i!=0)  printf("\n");
        printf("%4c",intToChar[ board[convertTo25[i]] ] );
        }
    printf("\n");
}

//Έλεγχος του πίνακα εάν έχει κάποια άδεια θέση 
//Check the table if there is an empty space
int hasEmpty(){
    for (int i=0; i<USETABLE; i++){
        if (board[convertTo25[i]] == E) 
            return 1;
    }
    return 0;
}

//Κάνει την κίνηση στο Board
//Makes the move on the Board
void makeMove(int sq, int side){
    board[sq] = side;
}

int getNextBest(){
    int ourMove = convertTo25[InMiddle];
    if(board[ourMove] == E) return ourMove;

    ourMove = -1;
    for(int i=0; i<4; i++){
        ourMove = convertTo25[Corners[i]];
        if(board[ourMove] == E) break;
        ourMove = -1;
    }
    return ourMove;
}

int getWinningMove(const int side){
    int ourMove = -1, winFound = 0;

    for (int i=0; i < 9; i++){
        if (board[ convertTo25[i] ] == E){
            ourMove = convertTo25[i];
            board[ourMove] = side;

            if(findThreeInARow(ourMove, side) == 3 ){
                winFound = 1;
            }
            board[ourMove] = E;
            if(winFound) break;
        }
        ourMove = -1;
    }
    return ourMove;
}

//Υπολογίζει την τυχαία κίνηση για τον υπολογιστή
//When it is the computer turn it calculates a random move
int getComputerMove(const int side){
    int freePlaces = 0;
    int availableMoves[9];
    int rndMove = 0;
    
    //get winnimg move
    rndMove = getWinningMove(side);
    if(rndMove != -1) return rndMove;
    
    //block move 
    rndMove = getWinningMove(side ^1);
    if(rndMove != -1) return rndMove;
    
    //get next best move
    rndMove = getNextBest();
    if(rndMove != -1) return rndMove;
    
    //otherwise pick random
    rndMove = 0;
    for (int i=0; i<USETABLE; i++){
        if( board[convertTo25[i]] == E){
            availableMoves[freePlaces++] = convertTo25[i];
        }
    }
    rndMove = (rand()  % freePlaces);
    return availableMoves[rndMove];
}

//Εισαγωγή κίνησης για τον χρήστη (από το πληκτρολόγιο)
//Input move from user (through keyboard)
int getHumanMove(){

    int place = -1;
    do{  
        printf("Επιλεξτε θεση για το  Χ apo to 0-8");
        if ( scanf("%d", &place) == 1 
            && (place >=0 && place <=8) 
            && board[convertTo25[place]] == E){ 
            getchar();
            printf("edoses %d\n",place);    
            //board[ convertTo25[place] ] = X;
            //printBoard();
        }else{
            printf("mono arithmous apo 0-8 kai edoses  %d \n ", place);
            place = getchar();
        }
        fflush(stdin);
    }while( place<0 || place>9);
return convertTo25[place];
}

//Εκκίνηση παιχνιδιού
//Start the game
void runGame(){
    int gameOver = 0,
    side = X,
    lastMoveMade = 0;
    

    initBoard();
    printBoard();
    
    while(!gameOver){
        if (side == X){
            lastMoveMade  = getHumanMove();
            makeMove( lastMoveMade, side);
            side = O;
        }else{
            lastMoveMade = getComputerMove(side);
            makeMove( lastMoveMade, side);
            side = X;
        }
        
        printBoard();
        //Έλεγχος για το εάν υπάρχει νικητής
        //Check if there is a winner
        if (findThreeInARow(lastMoveMade, side^1) == 3){
            printf("Game Over!\n");
            if(side == X){
                printf("Computer ( O )  wins\n");
            }else{
                printf("Human ( X ) wins\n");
            }
            gameOver = 1;
        }

        //Έλεγχος για το εάν υπάρχει άδεια θέση αλλιώς είναι ισοπαλία 
        //Check if there are any empty places otherwise it is a draw
        if(!hasEmpty()){
            printf("DRAW\n");
            gameOver = 1;            
        }
    
    }
}

int main (int argc, char* argv[]){

    runGame();
    return 1;
}
