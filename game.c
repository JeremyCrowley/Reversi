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


#include "game.h"
#include "com.h"

void StartGame(uint8_t player[ROW_SIZE][ROW_SIZE], uint8_t enemy[ROW_SIZE][ROW_SIZE])
{   
    cursor[0][0] = 1;
    
    if(ROW_SIZE == 4)
    {
        player[2][2] = 1;
        player[1][1] = 1;
        
        enemy[2][1] = 1;
        enemy[1][2] = 1;
        
    }
    else if(ROW_SIZE == 8)
    {      
        
        player[4][4] = 1;
        player[3][3] = 1;
        
        enemy[4][3] = 1;
        enemy[3][4] = 1;
        
    }
    else if(ROW_SIZE == 16)
    {
        player[8][8] = 1;
        player[7][7] = 1;
        
        enemy[8][7] = 1;
        enemy[7][8] = 1;

    }
    // set the position of the cursor to 0,0
    diskSelect.row = 0;
    diskSelect.col = 0;
}

uint8_t KeyboardEvent(uint8_t currentRow, uint8_t currentCol, uint8_t keyboardCommand, uint8_t rowSize, uint8_t *move, uint8_t player[ROW_SIZE][ROW_SIZE])
{
    
    uint8_t flankDir = 0;
    
    if(keyboardCommand == UP)
    {
        if(diskSelect.row!=0)
        {
            cursor[currentRow-1][currentCol] = 1;
            cursor[currentRow][currentCol] = 0;
            
            diskSelect.row -= 1;
            
            return false;
        }
    }
    else if(keyboardCommand == DOWN)
    {
        if(diskSelect.row!=(rowSize-1))
        {
            cursor[currentRow+1][currentCol] = 1;
            cursor[currentRow][currentCol] = 0;
            
            diskSelect.row += 1;
            
            return false;
        }
    }
    else if(keyboardCommand == LEFT)
    {
        if(diskSelect.col!=0)
        {
            cursor[currentRow][currentCol-1] = 1;
            cursor[currentRow][currentCol] = 0;
 
            diskSelect.col -= 1;
            
            return false;
        }
    }
    else if(keyboardCommand == RIGHT)
    {
        if(diskSelect.col!=(rowSize-1))
        {
            cursor[currentRow][currentCol+1] = 1;
            cursor[currentRow][currentCol] = 0;
            
            diskSelect.col += 1;
            
            return false;
        }
    }
    else if(keyboardCommand == HOME)
    {
        cursor[currentRow][currentCol] = 0;
        diskSelect.row = 0;
        diskSelect.col = 0;
        cursor[diskSelect.row][diskSelect.col] = 1;
        
        return false;
    }
    else if(keyboardCommand == PASS)
    {
        sendData.pass = true;
        
        return false;
    }
    else if(keyboardCommand == SET)
    {
        
        if(player1.turn == true && player2.turn == false)
        {
            flankDir = LegalMove(diskSelect.row, diskSelect.col, player1.color, player2.color);
        }

        if(flankDir)
        {

            player[diskSelect.row][diskSelect.col] = 1;

            
            *move = diskSelect.row;
            *(move+1) = diskSelect.col;
            
            return flankDir;
        }
    }
    return false;
    
}

uint8_t LegalMove(uint8_t row, uint8_t col, uint8_t player[ROW_SIZE][ROW_SIZE], uint8_t enemy[ROW_SIZE][ROW_SIZE])
{
    
    uint8_t offset = 1;
    uint8_t flankDir = 0;
    
    /* a piece is already in that location */
    if((player[row][col] == 1) || (enemy[row][col] == 1))
    {
        return false;   
    }
    
    /* down flank */
    while((enemy[row+offset][col] == 1)  && (row+offset < (ROW_SIZE-1)))
    {
        if(player[row+offset+1][col] == 1)
        {
            flankDir = (flankDir | 0x01); 
        }
        offset++;
    }
    offset = 1;
    /* down-right flank */
    while((enemy[row+offset][col+offset] == 1) && (row+offset < (ROW_SIZE-1)) && (col+offset < (ROW_SIZE-1)))
    {
        if(player[row+offset+1][col+offset+1] == 1)
        {
            flankDir = (flankDir | 0x02);   
        }
        offset++;
    }
    offset = 1;
    /* right flank */
    while((enemy[row][col+offset] == 1) && (col+offset < (ROW_SIZE-1)))
    {
        if(player[row][col+offset+1] == 1)
        {
            flankDir = (flankDir | 0x04);  
        }
        offset++;
    }
    offset = 1;
    /* up-right flank */
    while((enemy[row-offset][col+offset] == 1) && (row-offset > 0) && (col+offset < (ROW_SIZE-1)))
    {
        if(player[row-offset-1][col+offset+1] == 1)
        {
            flankDir = (flankDir | 0x08);  
        }
        offset++;
    }
    offset = 1;
    /* up flank */
    while((enemy[row-offset][col] == 1) && (row-offset > 0))
    {
        if(player[row-offset-1][col] == 1)
        {
            flankDir = (flankDir | 0x10);   
        }
        offset++;
    }
    offset = 1;
    /* up-left flank */
    while((enemy[row-offset][col-offset] == 1) && (row-offset > 0) && (col-offset > 0))
    {
        if(player[row-offset-1][col-offset-1] == 1)
        {
            flankDir = (flankDir | 0x20);  
        }
        offset++;
    }
    offset = 1;
    /* left flank */
    while((enemy[row][col-offset] == 1) && (col-offset > 0))
    {
        if(player[row][col-offset-1] == 1)
        {
            flankDir = (flankDir | 0x40);   
        }
        offset++;
    }
    offset = 1;
    /* down-left flank */
    while((enemy[row+offset][col-offset] == 1) && (row+offset < (ROW_SIZE-1)) && (col-offset > 0))
    {
        if(player[row+offset+1][col-offset-1] == 1)
        {
            flankDir = (flankDir | 0x80);    
        }
        offset++;
    }
    
    /*  Not Legal:   0x00
        Down:        0x01
        DownRight:   0x02
        Right:       0x04
        UpRight:     0x08
        Up:          0x10
        UpLeft:      0x20
        Left:        0x40
        DownLeft:    0x80    */
    
    return flankDir;
}

void Flank(uint8_t row, uint8_t col, uint8_t flankDir, uint8_t player[ROW_SIZE][ROW_SIZE], uint8_t enemy[ROW_SIZE][ROW_SIZE])
{
	uint8_t offset = 1;

	/* down */
	if(flankDir & 0x01)
	{
		while(enemy[row+offset][col] == 1)
		{
			enemy[row+offset][col] = 0;
			player[row+offset][col] = 1;

			offset++;
		}
	}
	offset = 1;

	/* down-right */ 
	if(flankDir & 0x02)
	{
		while(enemy[row+offset][col+offset] == 1)
		{
			enemy[row+offset][col+offset] = 0;
			player[row+offset][col+offset] = 1;

			offset++;
		}
	}

	/* right */
	offset = 1;
	if(flankDir & 0x04)
	{
		while(enemy[row][col+offset])
		{
			enemy[row][col+offset] = 0;
			player[row][col+offset] = 1;

			offset++;
		}
	}
	offset = 1;

	/* up-right */
	if(flankDir & 0x08)
	{
		while(enemy[row-offset][col+offset] == 1)
		{
			enemy[row-offset][col+offset] = 0;
			player[row-offset][col+offset] = 1;

			offset++;
		}
	}
	offset = 1;

	/* up */
	if(flankDir & 0x10)
	{
		while(enemy[row-offset][col] == 1)
		{
			enemy[row-offset][col] = 0;
			player[row-offset][col] = 1;

			offset++;
		}
	}
	offset = 1;

	/* up-left */
	if(flankDir & 0x20)
	{
		while(enemy[row-offset][col-offset] == 1)
		{
			enemy[row-offset][col-offset] = 0;
			player[row-offset][col-offset] = 1;

			offset++;
		}
	}
	offset = 1;

	/* left */
	if(flankDir & 0x40)
	{
		while(enemy[row][col-offset] == 1)
		{
			enemy[row][col-offset] = 0;
			player[row][col-offset] = 1;

			offset++;
		}
	}
	offset = 1;

	/* down-left */
	if(flankDir & 0x80)
	{
		while(enemy[row+offset][col-offset] == 1)
		{
			enemy[row+offset][col-offset] = 0;
			player[row+offset][col-offset] = 1;

			offset++;
		}
	}
}


bool GameActive(uint8_t rowSize, uint8_t player[ROW_SIZE][ROW_SIZE], uint8_t enemy[ROW_SIZE][ROW_SIZE])
{
	int i=0;
	int j=0;

	uint8_t move = 0;

	for(i=0;i<rowSize;i++)
	{
		for(j=0;j<rowSize;j++)
		{
            

            move = LegalMove(i, j, player, enemy);

			if(move)
			{
				return true;
			}
		}
	}
	return false;
}

uint16_t GetScore(uint8_t rowSize, uint8_t player[ROW_SIZE][ROW_SIZE])
{
    int i, j;
    
    uint16_t score =0;
    
    for(i=0;i<rowSize;i++)
    {
        for(j=0;j<rowSize;j++)
        {
            if(player[i][j] == 1)
            {
                score++;   
            }
        }
    }
    return score;
}

void DisplayScore(bool gameOver, bool myTurn)
{
    
    player1.score = GetScore(ROW_SIZE, player1.color);
    player2.score = GetScore(ROW_SIZE, player2.color);
    
    LCD_ClearDisplay();
    if(gameOver == true)
    {
        if(player1.score > player2.score)
        {
            LCD_PrintString("Player 1 Wins!");
        }
        else if (player2.score > player1.score)
        {
            LCD_PrintString("Player 2 Wins!");   
        }
        else
        {
            LCD_PrintString("Tie Game");
        }
    }
    else
    {
        if(myTurn)
        {
            LCD_Position(0,0);
            LCD_PrintString("My Turn");
        }
        else
        {
            LCD_Position(0,0);
            LCD_PrintString("Opponent's Turn");
        }
    }
    LCD_Position(1,0);
    LCD_PrintString("P1: ");
    LCD_PrintNumber(player1.score);
    
    LCD_Position(1,7);
    LCD_PrintString("P2: ");
    LCD_PrintNumber(player2.score);   
}

void GameUpdate(void)
{   
    // check for the end of the game
    if(!GameActive(ROW_SIZE, player1.color, player2.color) && !GameActive(ROW_SIZE, player2.color, player1.color))
    {
        DisplayScore(true, player1.turn);
    }
    else
    {
        DisplayScore(false, player1.turn);
    }
}

