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
#include "com.h"


void BuildSendPacket(uint8_t packet[SEND_SIZE], uint8_t seg, uint8_t pass, uint8_t rowHigh, uint8_t rowLow, uint8_t colHigh, uint8_t colLow)
{
    packet[0] = 'd';
    packet[1] = 'a';
    packet[2] = 't';
    packet[3] = 'a';
    packet[4] = ' ';
    
    packet[5] = 0x55;
    packet[6] = 0xaa;

    packet[7] =  'J'; //0x4A;
    packet[8] =  'e'; //0x65; 
    packet[9] =  'r'; //0x72;
    packet[10] =  'e'; //0x65;
    packet[11] =  'm'; //0x6D;
    packet[12] =  'y'; //0x79; 
    
    packet[13] = 0x20;
    
    packet[14] = Hundreds(seg) + 48;
    packet[15] = Tens(seg, packet[9]) + 48;
    packet[16] = Ones(seg, packet[9], packet[10]) + 48;
    
    packet[17] = pass+48;
    packet[18] = rowHigh + 48;
    packet[19] = rowLow + 48;
    packet[20] = colHigh + 48;
    packet[21] = colLow + 48;
    
    packet[22] = '\n';

}

void ParsePacket(uint8_t packet[SEND_SIZE - 3], int idEnd)
{
    receiveData.pass = packet[idEnd+6]-48;
    receiveData.row = (10*(packet[idEnd+7]-48)+(packet[idEnd+8]-48));
    receiveData.col = (10*(packet[idEnd+9]-48)+(packet[idEnd+10]-48));
    
    if(receiveData.row != 0)
    {
        receiveData.row -= 1;   
    }
    if(receiveData.col != 0)
    {
        receiveData.col -= 1;   
    }
}

uint8_t Hundreds(uint8_t num)
{
    return (num/100);
}

uint8_t Tens(uint8_t num, uint8_t hundreds)
{
    return (num%(100*hundreds))/10;
}

uint8_t Ones(uint8_t num, uint8_t hundreds, uint8_t tens)
{
    return (num%((100*hundreds) + (10*tens)));
}

bool newData(uint8_t currentSeq)
{
    if(currentSeq == (receiveData.seq + 1))
    {
        return true;   
    }
    else
    {
        return false;   
    }
}
/* [] END OF FILE */
