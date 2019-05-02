//
//  main.cpp
//  TicTacTo
//
//  Created by Tarek Abdelrahman on 2018-05-15.
//  Copyright © 2018 Tarek Abdelrahman. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include <iostream>

#include <tic-tac-toe/playMove.h>

using namespace std;
using namespace sf;

// Function prototypes
void drawToWindow(sf::RenderWindow& window, sf::RectangleShape shape, int xPos, int yPos, int rotation);
void drawToWindow(sf::RenderWindow& window, sf::Sprite shape, int xPos, int yPos, int rotation);

int main() {

    /************************************************************************************************************/
    /* The following variables define the state of the game                                                     */
    /************************************************************************************************************/

    // You should complete the following declarations
    // Make sure to initalize the variables

    // The game board array
    int gameBoard[3][3] = {Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty};

    // An boolean that represents the turn in the game
    bool turn = true; // X - true, O - false
    
    // A boolean to represent if the move is valid
    bool validMove = false;

    // A boolean to represent if the game is over
    bool is_game_over = false;

    // An integer that represents the win code
    int winCode=0;

    /************************************************************************************************************/
    /* Insert code that loads the various images used by the game and creates the sprites                       */
    /* The loading of the X image is shown. Repreat for the rest of the images                                  */
    /************************************************************************************************************/

    // Get the X image
    sf::Texture X_texture;
    if (!X_texture.loadFromFile("X.jpg")) {
        return EXIT_FAILURE;
    }
    sf::Sprite X_sprite(X_texture);

    // Get the O image
    sf::Texture O_texture;
    if (!O_texture.loadFromFile("O.jpg")) {
        return EXIT_FAILURE;
    }
    sf::Sprite O_sprite(O_texture);

    // Get the blank image
    sf::Texture blank_texture;
    if (!blank_texture.loadFromFile("blank.jpg")) {
        return EXIT_FAILURE;
    }
    sf::Sprite blank_sprite(blank_texture);
    
    /************************************************************************************************************/
    /* Insert code that creates the window and displays it                                                      */
    /************************************************************************************************************/

    // First, we find the sizes of the images so we can determine the size of the window we must create
    // The images (X, O and Black) are assumed to be square are are all of the same size
    // Thus, we just find the size of one of them

    // The tile size is the size of the images
    const int tileSize = X_texture.getSize().x;

    // The cell borders (thick lines) are rectangles
    // The size of these rectangles is 1/20th of the size of the tile by 3 cell sizes plus 2 barWidth's
    const int barWidth = tileSize/20;
    const int barHeight = boardSize*tileSize + (boardSize-1)*barWidth;

    // The size of the window in pixels can now be calculated
    const int windowSize = boardSize*tileSize + (boardSize-1)*barWidth;

    // Create the main window: it has a title bar and a close button, but is not resizable
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "ECE244 Tic-Tac-Toe", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true); // Fix flickering
        
    // Set the Icon that appears in the task bar when the window is minimized
    // Insert you code here, see the lab handout
    sf::Image icon;
    if(!icon.loadFromFile("icon.jpg")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());
    
    // Create the horizonal (vertical) borders as rectangles of sizes barWidth and barHeight (barHeight and barWidth)
    sf::RectangleShape border(sf::Vector2f(barWidth,barHeight));
    border.setFillColor(sf::Color(0,0,0)); // Black
    // Vertical borders
    drawToWindow(window,border,tileSize,0,0);
    drawToWindow(window,border,2*tileSize+barWidth,0,0);
    // Horizontal borders
    drawToWindow(window,border,0,tileSize+barWidth,-90);
    drawToWindow(window,border,0,2*tileSize+2*barWidth,-90);

    /************************************************************************************************************/
    /* This is the main event loop of the code                                                                  */
    /************************************************************************************************************/

    // Start the game loop that runs as long as the window is open
    while (window.isOpen()) {
        // The event
        sf::Event event;

        // Process the events
        while (window.pollEvent(event)) {

            // This is the handling of the close window event
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }

            // Left mouse button pressed: get the click position and play the move
            // is_game_over is a variable you should define above
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // left mouse button is pressed: get the coordinates in pixels
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);

                // Convert the pixel coordinates into game board rows and columns
                // Just divide by tileSize
                // Observe that the x axis is the rows and the y axis is the columns
                // Also make sure that row and column values are valid
                int row = localPosition.y/tileSize; // Honestly not the best because of the borders
                int col = localPosition.x/tileSize;
                
                // Play the move by calling the playMove function
                playMove(gameBoard,row,col,turn,validMove,is_game_over,winCode);
            }
        }

        // Insert code to draw the tiles using the sprites created earlier
        // You must determine the position of each cell on the grid, taking into account the width of
        // the border and then position the sprite there and draw it.
        // Draw the entire board, cell borders included,
        // reflecting any move that has just happened (check the gameBoard array)
        // Further, if the game is over and one player wins, you must create and draw a rectangle of
        // windowSize width and 10 pixels height to indicate the winning marks
        // The fill of this rectangle should be white
        
        // Drawing the tiles
        for(int i=0; i<3; i++) {
            for(int j=0; j<3; j++) {
                int xPos = j*(tileSize+barWidth);
                int yPos = i*(tileSize+barWidth);
                // Figure out which sprite to draw
                switch(gameBoard[i][j]) {
                    case X:
                        drawToWindow(window,X_sprite,xPos,yPos,0);
                        break;
                    case O:
                        drawToWindow(window,O_sprite,xPos,yPos,0);
                        break;
                    default: // Blank
                        drawToWindow(window,blank_sprite,xPos,yPos,0);
                        break;
                }
            }
        }
        
        // Drawing the line if there is a win
        if(is_game_over) {
            sf::RectangleShape gameOverLine(sf::Vector2f(2*barWidth,2*barHeight));
            gameOverLine.setFillColor(sf::Color(255,255,255)); // White
            int horizontalOffset = tileSize/2+barWidth; // Horizontal offset so after rotation the line is in the middle
            int verticalOffset = tileSize/2-barWidth;
            switch(winCode) {
                case 1: // Row 1
                    drawToWindow(window,gameOverLine,0,horizontalOffset,-90);
                    break;
                case 2: // Row 2
                    drawToWindow(window,gameOverLine,0,tileSize+barWidth+horizontalOffset,-90);
                    break;
                case 3: // Row 3
                    drawToWindow(window,gameOverLine,0,2*(tileSize+barWidth)+horizontalOffset,-90);
                    break;
                case 4: // Col 1
                    drawToWindow(window,gameOverLine,verticalOffset,0,0);
                    break;
                case 5: // Col 2
                    drawToWindow(window,gameOverLine,tileSize+barWidth+verticalOffset,0,0);
                    break;
                case 6: // Col 3
                    drawToWindow(window,gameOverLine,2*(tileSize+barWidth)+verticalOffset,0,0);
                    break;
                case 7: // \-diagonal
                    drawToWindow(window,gameOverLine,-barWidth,0,-45); // Centered using an xPos offset
                    break;
                case 8: // /-diagonal
                    drawToWindow(window,gameOverLine,3*tileSize+2*barWidth,-barWidth,45); // Centered using a yPos offset
                    break;
                default: break; // Draw
            }
        }

        // Display to the window
        window.display();
    }

    return EXIT_SUCCESS;
}

void drawToWindow(sf::RenderWindow& window, sf::RectangleShape shape, int xPos, int yPos, int rotation)
{
    shape.setPosition(xPos,yPos);
    shape.rotate(rotation);
    window.draw(shape);
}

void drawToWindow(sf::RenderWindow& window, sf::Sprite shape, int xPos, int yPos, int rotation)
{
    shape.setPosition(xPos,yPos);
    shape.rotate(rotation);
    window.draw(shape);
}