/* ========================================
 * Author: Jeremy Crowley
 * File: com.c
 *
 * Purpose: This file provides the functions for UART communication
 * ========================================
 */

#include "com.h"

#define ASCI_DIFF 48
#define TENS 10

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
    
    packet[14] = Hundreds(seg) + ASCI_DIFF;
    packet[15] = Tens(seg, packet[9]) + ASCI_DIFF;
    packet[16] = Ones(seg, packet[9], packet[10]) + ASCI_DIFF;
    
    packet[17] = pass + ASCI_DIFF;
    packet[18] = rowHigh + ASCI_DIFF;
    packet[19] = rowLow + ASCI_DIFF;
    packet[20] = colHigh + ASCI_DIFF;
    packet[21] = colLow + ASCI_DIFF;
    
    packet[22] = '\n';

}

void ParsePacket(uint8_t packet[SEND_SIZE - 3], int idEnd)
{
    receiveData.pass = packet[idEnd+6]-ASCI_DIFF;
    receiveData.row = (TENS*(packet[idEnd+7]-ASCI_DIFF)+(packet[idEnd+8]-ASCI_DIFF));
    receiveData.col = (TENS*(packet[idEnd+9]-ASCI_DIFF)+(packet[idEnd+10]-ASCI_DIFF));
    
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
    // make sure that the sequence num recieve is one more that the last
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
