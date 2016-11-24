/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef GAME_H
#define GAME_H

#include <project.h>
#include <stdint.h>
#include <stdbool.h>

#define ROW_SIZE 4
#define TRUE 1
#define FALSE 0

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define HOME 'h'
#define PASS 'p'
#define SET ' '
    

struct Player
{
    uint8_t (*color)[ROW_SIZE];
    uint16_t score;
    bool turn;
    uint32_t ipVal;
    char IP[13];
    char ID[8];
};

typedef struct Player player_t;
player_t player1;
player_t player2;

uint8_t red[ROW_SIZE][ROW_SIZE]; // red
uint8_t cursor[ROW_SIZE][ROW_SIZE]; // green
uint8_t blue[ROW_SIZE][ROW_SIZE]; // blue



struct Cursor
{
    uint8_t color;
    uint8_t row;
    uint8_t col;
};

typedef struct Cursor cursor_t;
cursor_t diskSelect;

/* initializes the game, sets cursor at the top left and set first 4 tokens */
void StartGame(uint8_t blue[ROW_SIZE][ROW_SIZE], uint8_t red[ROW_SIZE][ROW_SIZE]);


/* updates the display based on the three arrays and board size */
void UpdateBoard(int i, int j, uint8_t **redPosition, uint8_t **greenPosition, uint8_t **bluePosition, uint8_t rowSize);

/* processes any keyboard event and returns a flag if a move has been made */
uint8_t KeyboardEvent(uint8_t currentRow, uint8_t currentCol, uint8_t keyboardCommand, 
                      uint8_t rowSize, uint8_t *move, uint8_t player[ROW_SIZE][ROW_SIZE]);

/* determines if placing a piece at the location specified is a legal move */
uint8_t LegalMove(uint8_t row, uint8_t col, uint8_t player[ROW_SIZE][ROW_SIZE], uint8_t enemy[ROW_SIZE][ROW_SIZE]);

/* flips the piece that need to be flipped when flanked */
void Flank(uint8_t row, uint8_t col, uint8_t flankDir, uint8_t player[ROW_SIZE][ROW_SIZE], uint8_t enemy[ROW_SIZE][ROW_SIZE]);

/* checks the board to see if there are any possible moves */
bool GameActive(uint8_t rowSize, uint8_t player[ROW_SIZE][ROW_SIZE], uint8_t enemy[ROW_SIZE][ROW_SIZE]);

/* obtains the current score */
uint16_t GetScore(uint8_t rowSize, uint8_t player[ROW_SIZE][ROW_SIZE]);

/* displays current score */
void DisplayScore(bool gameOver, bool myTurn);

/* updates the game state on the LCD */
void GameUpdate(void);


#endif

/* [] END OF FILE */
