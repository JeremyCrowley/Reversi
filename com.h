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
#ifndef COM_H
#define COM_H

#include <project.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define SEND_SIZE 40
    
typedef enum
{
    START1,
    START2,
    ID, 
    DATA,
} DATA_STATES;

struct Receive
{
    uint8_t seq;
    uint8_t pass;
    uint8_t row;
    uint8_t col;
};

typedef struct Receive receive_t;
receive_t receiveData;

struct Send
{
    uint8_t seq;
    uint8_t pass;
    uint8_t rowH;
    uint8_t rowL;
    uint8_t colH;
    uint8_t colL;
};

typedef struct Send send_t;
send_t sendData;

/* populates an array that will be used iteratively to send packet */
void BuildSendPacket(uint8_t packet[SEND_SIZE], uint8_t seg, uint8_t pass, uint8_t rowHigh, uint8_t rowLow, uint8_t colHigh, uint8_t colLow);

/* extracts the hundreds digit */
uint8_t Hundreds(uint8_t num);
/* exctracts the tens digit */
uint8_t Tens(uint8_t num, uint8_t hundreds);
/* extracts the ones digit */
uint8_t Ones(uint8_t num, uint8_t hundreds, uint8_t tens);
/* returns true if data is new, false if same data -- based on seq number */
bool newData(uint8_t currentSeq);

void ParsePacket(uint8_t packet[SEND_SIZE - 3], int idEnd);


#endif
/* [] END OF FILE */
