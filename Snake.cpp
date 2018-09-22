#include <iostream>
#include <stdlib.h> //Srand(), rand()
#include <time.h>   //Time seed for srand()
#include <windows.h> //Sleep()
#include <array> //Using std::arrays rather than c style arrays for easier equality comparisons.
#include "sl.h" //SIGIL header

int main()
{
    /* Class representing each space on board.
    // Distinguishing background vs. snake vs. pill depends on space color */
    class BoardSpace{

        public:
            int bodyTimer = 0;

            bool isSnake = false;

            void setSpaceColor( double red, double blue, double green, double alpha){
                slSetForeColor( red, green , blue, alpha);
            }

            void drawSpace( double x, double y, double width, double height){
                slRectangleFill( x, y, width, height);
            }
    };

    int growBy = 0;
    int directionX = 0, directionY = 0; //Direction that snake's head is moving
    int positionX = 1, positionY = 1; //Position of snake's head
    int pillX = 0, pillY = 0;
    bool isFirstTurn = true; //boolean to prevent collision detection from checking before player has the chance to move

    srand(time(NULL));

    slWindow( 993, 601, "Snake Game", false);
    BoardSpace snakeBoard[43][26];

    //Randomly generate first pill
    pillX = rand() % 43;
    pillY = rand() % 26;

    snakeBoard[pillX][pillY].setSpaceColor( 1.0, 0.4, 0.2, 1.0);
    snakeBoard[pillX][pillY].drawSpace( 13 + (23 * pillX), 588 - (23 * pillY), 20.0, 20.0);

    while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE)){

        //If snake eats a pill, randomly generate another in a different position that isn't on the snake
        if(positionX == pillX && positionY == pillY){
            growBy += 3;

            //By increasing all current bodyTimers by three, the snake will grow forward by three spaces while all other spaces are frozen.
            for(int k = 0; k < 43; ++k){
                for(int L = 0; L < 26; ++L){
                    if(snakeBoard[k][L].bodyTimer > 0){
                        snakeBoard[k][L].bodyTimer += 3;
                    }
                }
            }

            do{
                pillX = rand() % 43;
                pillY = rand() % 26;
                //If those coordinates are already occupied by the snake, get new ones
            }while(snakeBoard[pillX][pillY].isSnake == true);


            snakeBoard[pillX][pillY].setSpaceColor( 1.0, 0.4, 0.2, 1.0);
            snakeBoard[pillX][pillY].drawSpace( 13 + (23 * pillX), 588 - (23 * pillY), 20.0, 20.0);
        }

        //Draw foreground colors
        for(int i = 0; i < 43; ++i){
            for(int j = 0; j < 26; ++j){
                /* Each square has a 3px border, so they effectively take up a 23x23 space. The coordinates of the bottom corner of each
                /  square are both given by 13 + (23 * n) ,but to have the squares drawn from left to right, bottom to top, the y coordinate
                /  must be given by 601 - ( 13 + (23 * n)), which simplifies to 588 - (23 * n) */

                //Draw snake
                if (snakeBoard[i][j].bodyTimer > 0){
                    snakeBoard[i][j].setSpaceColor(  1.0, 0.7, 0.7, 1.0);
                    snakeBoard[i][j].drawSpace( 13 + (23 * i), 588 - (23 * j), 20.0, 20.0);
                    --snakeBoard[i][j].bodyTimer;
                    if(snakeBoard[i][j].bodyTimer == 0){
                        snakeBoard[i][j].isSnake = false;
                    }
                }
                //Draw unoccupied spaces
                else if((i != pillX) || (j != pillY)){
                    snakeBoard[i][j].setSpaceColor( 0.3, 0.6, 0.4, 1.0);
                    snakeBoard[i][j].drawSpace( 13 + (23 * i), 588 - (23 * j), 20.0, 20.0);
                }

                //Draw pill
                else{
                    snakeBoard[pillX][pillY].setSpaceColor( 1.0, 0.4, 0.2, 1.0);
                    snakeBoard[pillX][pillY].drawSpace( 13 + (23 * pillX), 588 - (23 * pillY), 20.0, 20.0);
                }
            }
        }

        if(slGetKey(SL_KEY_UP) && snakeBoard[positionX][positionY - 1].isSnake == false){
            directionY = -1;
            directionX = 0;
            isFirstTurn = false;
        }
        else if(slGetKey(SL_KEY_DOWN) && snakeBoard[positionX][positionY + 1].isSnake == false){
            directionY = 1;
            directionX = 0;
            isFirstTurn = false;
        }
        else if(slGetKey(SL_KEY_LEFT) && snakeBoard[positionX - 1][positionY].isSnake == false){
            directionX = -1;
            directionY = 0;
            isFirstTurn = false;
        }
        else if(slGetKey(SL_KEY_RIGHT) && snakeBoard[positionX + 1][positionY].isSnake == false){
            directionX = 1;
            directionY = 0;
            isFirstTurn = false;
        }

        positionX += directionX;
        positionY += directionY;


        if((isFirstTurn == false && snakeBoard[positionX][positionY].isSnake == true)
                || positionX > 42 ||positionX < 0 || positionY > 25 || positionY < 0){

            slClose();
            std::cout << "\nYou died.\n\n";

        }
        else{
            snakeBoard[positionX][positionY].setSpaceColor( 1.0, 0.7, 0.7, 1.0);
            snakeBoard[positionX][positionY].drawSpace( 13 + (23 * positionX), 588 - (23 * positionY), 20.0, 20.0);
            snakeBoard[positionX][positionY].bodyTimer += growBy;
            if(snakeBoard[positionX][positionY].bodyTimer > 0){
                snakeBoard[positionX][positionY].isSnake = true;
            }

        }

        Sleep(70);

        slRender();
    }
}
