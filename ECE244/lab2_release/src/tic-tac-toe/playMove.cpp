//
//  playMove.cpp
//  TicTacTo
//
//  Created by Tarek Abdelrahman on 2018-05-15.
//  Copyright © 2018 Tarek Abdelrahman. All rights reserved.
//

#include <iostream>
#include <tic-tac-toe/playMove.h>

using namespace std;

int checkWinCode(int board[3][3]);
bool checkGameOver(int board[3][3], int winCode);

void playMove(int board[3][3], int row, int col, bool& turn, bool& validMove, bool& gameOver, int& winCode) {

    // If the game is over, do nothing
    if (gameOver) return;

    // Check if move is valid and play
    validMove = (row>=0 && row<3 && col>=0 && col<3 && board[row][col]==Empty);
    if(validMove) { // Place down the correct marker and change the turn
        board[row][col] = (turn ? X:O);
        turn = !turn;
    }
    
    // Get the current win code and check if game is over
    winCode = checkWinCode(board);
    gameOver = checkGameOver(board,winCode);

    // The function must always print the output specified in the handout before it exits
    // Use the standard output (i.e., cout) to print the output

    // Insert code to print the output below
    // Output board
    for(int i=0; i<3; i++) 
        for(int j=0; j<3; j++)
            cout << board[i][j] << " ";
    // Output: row col turn validMove gameOver winCode
    cout << row << " " << col << " " << turn << " " << validMove << " " << gameOver << " " << winCode << endl;
}

// Check win conditions - 3 in a row
int checkWinCode(int board[3][3])
{
    /* Win code values:
     * 0 - No win / game still going
     * 1 - Row 1
     * 2 - Row 2
     * 3 - Row 3
     * 4 - Col 1
     * 5 - Col 2
     * 6 - Col 3
     * 7 - \ diagonal
     * 8 - / diagonal
     */
    if(board[0][0] != Empty && board[0][0] == board[0][1] && board[0][0] == board[0][2]) return 1;
    else if(board[1][0] != Empty && board[1][0] == board[1][1] && board[1][0] == board[1][2]) return 2;
    else if(board[2][0] != Empty && board[2][0] == board[2][1] && board[2][0] == board[2][2]) return 3;
    else if(board[0][0] != Empty && board[0][0] == board[1][0] && board[0][0] == board[2][0]) return 4;
    else if(board[0][1] != Empty && board[0][1] == board[1][1] && board[0][1] == board[2][1]) return 5;
    else if(board[0][2] != Empty && board[0][2] == board[1][2] && board[0][2] == board[2][2]) return 6;
    else if(board[0][0] != Empty && board[0][0] == board[1][1] && board[0][0] == board[2][2]) return 7;
    else if(board[0][2] != Empty && board[0][2] == board[1][1] && board[0][2] == board[2][0]) return 8;
    else return 0;
}

// Check if the game is over
bool checkGameOver(int board[3][3], int winCode)
{
    // Game is not over as long as winCode is 0 AND there is an empty space
    if(winCode==0) 
        for(int i=0; i<3; i++) // Check board for blank space
            for(int j=0; j<3; j++)
                if(board[i][j]==Empty) return false;
            
    return true;
}