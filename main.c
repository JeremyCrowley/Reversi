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
#include <project.h>
#include "game.h"
#include "com.h"
#include "wifi.h"
#include "FS.h"
 
#include <stdio.h>
#include <stdlib.h>


#define SDLOG_SIZE 36
#define FILENAME ("Logger.txt")

/* SD card */
FS_FILE *fp;

/* interrupt variables */

//rx
DATA_STATES dataState = START1;
int idCount = 0;
uint8_t dataRec[SEND_SIZE];
int dataCount = 0;

//tx
uint8_t dataSend[SEND_SIZE];
int countS = 0;

//update
bool dataFlag = false;
int ISRFlag = false;

//wifi
WIFI_STATES wifiState = PLAYER;
SETUP_STATES setupState = ADVERTISE;
char wifiData[120];
char wifiPrint[40];
int wifiCount = 0;
int wifiSize = 0;

char adPrint[AD_SIZE];
int adCount = 0;
int adSize = 0;

bool wifiFlag = false;

/* isr prototypes */
CY_ISR_PROTO(isr);
CY_ISR_PROTO(isr_tx);
CY_ISR_PROTO(isr_rx);
CY_ISR_PROTO(isr_wifi_rx);
CY_ISR_PROTO(isr_wifi_tx);

int main()
{
    strncpy(player1.ID, "Jeremy", 6);
    
    int i, j;
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    memset(wifiData, '\0', 120);
    memset(wifiPrint, '\0', 40);
    memset(dataSend, '\0', SEND_SIZE);
    memset(dataRec, '\0', SEND_SIZE);
    memset(wifiPrint, '\0', AD_SIZE);
    
    
    /* wifi start up */
    UART_Start();
    USBUART_Start(0u, USBUART_5V_OPERATION);
    while (USBUART_GetConfiguration() == 0){};
    USBUART_CDC_Init();
    isr_wifi_rx_StartEx(isr_wifi_rx);
    Timer_tx_Start();
    
    LCD_Start();
    
    
    //uint8_t ipMessageLen = 0;
    bool ipFound = false;
    char *ipPointer = NULL;
    //char ipMessage[28];
    memset(wifiPrint, '\0', 28);
    
    bool setupFlag = false;
    
    while(!setupFlag)
    {
        switch(setupState)
        {
            case ADVERTISE:
            
            LCD_ClearDisplay();
            LCD_PrintString("ADVERTISE");
            if(USBUART_GetConfiguration() != 0)
            {
                if(USBUART_DataIsReady() != 0)
                {
                    adPrint[adCount] = USBUART_GetChar();
                    adCount++;
                    /* wait for a return carriage */
                    if(adPrint[adCount-1] == 0x0D)
                    {
                        adPrint[adCount-1] = 0x0A;
                        /* check if advertise attempt */
                        if(!strncmp(adPrint, "advertise Jeremy", adCount-1))
                        {
                            while(!USBUART_CDCIsReady());
                            USBUART_PutChar(0xa);
                            while(!USBUART_CDCIsReady());
                            USBUART_PutData((uint8_t*)adPrint, adCount);
                            while(!USBUART_CDCIsReady());
                            USBUART_PutChar(0xd);
                            isr_wifi_tx_StartEx(isr_wifi_tx);

                            setupState = CONNECT;
                        }
                        adCount = 0;
                    } 
                }
            }
            if(wifiFlag)
            {
                wifiFlag = false;
                while(!USBUART_CDCIsReady());
                USBUART_PutData((uint8_t*)wifiPrint, wifiSize);
                while(!USBUART_CDCIsReady());
                USBUART_PutChar(0xa);
                while(!USBUART_CDCIsReady());
                USBUART_PutChar(0xd);
                
                /* extract my IP address from the initialization */
                if(!ipFound)
                {
                    ipPointer = strstr(wifiPrint, "IP Address: ");
                    if(ipPointer)
                    {
                        ipPointer = NULL;
                        ipFound = true;
                        strncpy(player1.IP, wifiPrint+13, IP_STRLEN);
                    }    
                }
                memset(wifiPrint, '\0', wifiSize);
            } 
            break;
                
            case CONNECT:
            
            LCD_ClearDisplay();
            LCD_PrintString("CONNECT");
            
            if(USBUART_GetConfiguration() != 0)
            {
                if(USBUART_DataIsReady() != 0)
                {
                    adPrint[adCount] = USBUART_GetChar();
                    adCount++;
                    if(adPrint[adCount-1] == 0x0D)
                    {
                        adPrint[adCount-1] = 0x0A;
                        
                        /* wait for the connect command */
                        ipPointer = strstr(adPrint, "connect ");
                        if(ipPointer)
                        {
                            strncpy(player2.IP, ipPointer+8, IP_STRLEN);
                            isr_wifi_tx_StartEx(isr_wifi_tx);
                        }                       
                    }                 
                }              
            }
            if(wifiFlag)
            { 
                wifiFlag = false;
                while(!USBUART_CDCIsReady());
                USBUART_PutData((uint8_t*)wifiPrint, wifiSize);
                while(!USBUART_CDCIsReady());
                USBUART_PutChar(0xa);
                while(!USBUART_CDCIsReady());
                USBUART_PutChar(0xd);
                
                /* when connected, break from the setup */
                ipPointer = strstr(wifiPrint, "Connected");
                if(ipPointer)
                {
                    setupFlag = true;
                }
            }    
            break;
        }
   
    }
    /* disable the rxwifi isr */
    isr_wifi_rx_Disable();
    
    /* initialize arrays to 0 */
    
    for(i=0;i<ROW_SIZE;i++)
    {
        for(j=0;j<ROW_SIZE;j++)
        {
            player1.color[i][j] = 0;
            cursor[i][j] = 0;
            player2.color[i][j] = 0;
        }
    }
    
    /* if my ip lower, set up the game accordingly */
    
    if(strncmp(player1.IP, player2.IP, IP_STRLEN) < 0)
    {
        player1.turn = true;
        player1.color = red;
    
        player2.turn = false;
        player2.color = blue;
        
        /* sets up the board */
     
        StartGame(player2.color, player1.color); 
    }
    else
    {
        player1.turn = false;
        player1.color = blue;
    
        player2.turn = true;
        player2.color = red;
        
     
        StartGame(player1.color, player2.color); 
    }

    for(i=0;i<SEND_SIZE;i++)
    {
        dataSend[i] = 0;  
    }
    for(i=0;i<SEND_SIZE;i++)
    {
        dataRec[i] = 0;   
    }
    
    /* initialize for trasfer and receiving data */
    uint8_t seg = 0;
    sendData.seq=0;
    BuildSendPacket(dataSend, sendData.seq, 0, 0, 0, 0, 0);
    
    receiveData.seq = 0;

    
    uint8_t rx;
    
    uint8_t flankDir = 0;
    uint8_t moveCoords[2] = {0, 0};


    /* harware set up */

    isr_tx_StartEx(isr_tx);
    isr_StartEx(isr);
    Timer_Start();
    isr_rx_StartEx(isr_rx);
    
    /* sd card set up */
    char sdBuffer[SDLOG_SIZE];
    memset(sdBuffer, '\0', SDLOG_SIZE);
    int sdSize = 0;
    
    char newLine = '\n';
    
    FS_Init();
    fp = FS_FOpen(FILENAME, "w");
    FS_FWrite("Start of Game", 13, 1, fp);
    FS_FWrite(&newLine, 1, 1, fp);
    FS_FClose(fp);

    
    for(;;) 
    {  

        if(USBUART_GetConfiguration() != 0)
        {
            if(USBUART_DataIsReady() != 0)
            {
                /* obtain data */
                USBUART_GetData(&rx, 1);
                /* only act on it if it is my turn */
                if(player1.turn == true)
                {
                    /* make sure that the command is setting a flankable move */
                    flankDir = KeyboardEvent(diskSelect.row, diskSelect.col, rx, ROW_SIZE, moveCoords, player1.color);
                    
                    /* build a packet with the passing flag high */
                    if(sendData.pass == true)
                    {
                        player1.turn = false;
                        player2.turn = true;

                        sendData.seq++;
                        BuildSendPacket(dataSend, sendData.seq, sendData.pass, 0, 0, 0, 0);
                        sendData.pass = false;
                        
                        /* write to file in sd card */
                        sdSize = sprintf(sdBuffer, "%s %d %d %d", player1.ID, sendData.pass, 0, 0);
                        fp = FS_FOpen(FILENAME, "a");
                        FS_FWrite(sdBuffer, sdSize, 1, fp);
                        FS_FWrite(&newLine, 1, 1, fp);
                        FS_FClose(fp);
                    }
                    /* build a packet with the coordinates of my move */
                    else if(flankDir)
                    {
                        Flank(diskSelect.row, diskSelect.col, flankDir, player1.color, player2.color);
                        player1.turn = false;
                        player2.turn = true;
                        flankDir = 0;
                        
                        /* set information for sending packet */
                        sendData.pass = false;
                        sendData.rowH = Tens(diskSelect.row, 0);
                        sendData.rowL = diskSelect.row + 1 - (10*sendData.rowH);
                        sendData.colH = Tens(diskSelect.col, 0);
                        sendData.colL = diskSelect.col + 1 - (10*sendData.colH);
                        sendData.seq++;
                        
                        BuildSendPacket(dataSend, sendData.seq, sendData.pass, sendData.rowH, sendData.rowL, sendData.colH, sendData.colL);
                        
                        /* write to file in sd card */
                        sdSize = sprintf(sdBuffer, "ID: %s Pass: %d Row: %d Col: %d", player1.ID, 0, diskSelect.row+1, diskSelect.col+1);
                        fp = FS_FOpen(FILENAME, "a");
                        FS_FWrite(sdBuffer, sdSize, 1, fp);
                        FS_FWrite(&newLine, 1, 1, fp);
                        FS_FClose(fp);
                        
                        while(!USBUART_CDCIsReady());
                        USBUART_PutData((uint8_t*)sdBuffer, sdSize);
                    } 
                }
            }
        }
        /* echo back to the terminal */
        if(USBUART_CDCIsReady() && ISRFlag)
        {
            USBUART_PutData(dataRec, (SEND_SIZE)); 
            while(!USBUART_CDCIsReady());
            USBUART_PutChar(0xa);
            while(!USBUART_CDCIsReady());
            USBUART_PutChar(0xd); 
        }
        
        
        if(ISRFlag == true)
        {
            GameUpdate();
            ISRFlag = false;
        }
        
        
        if(player2.turn)
        {
            /* stall until a new packet has arrive */
            while(!newData(seg))
            {
                /* update the scores */
                GameUpdate();
                
                if(dataFlag)
                {
                    seg = (100*(dataRec[idCount+3]-48) + 10*(dataRec[idCount+4]-48) + (dataRec[idCount+5]-48));   
                }
            }

            /* echo the received packet to the terminal */
            while(!USBUART_CDCIsReady());
            USBUART_PutData(dataRec, (idCount+11)); 
            
            
            /* disable the rx isr and process the data */
            isr_rx_Disable();
            
            ParsePacket(dataRec, idCount);
            
            /* make sure the packet is valid */
            flankDir = LegalMove(receiveData.row, receiveData.col, player2.color, player1.color);
            /* packet had a passing move */
            if(receiveData.pass)
            {
                receiveData.seq = seg;
                player2.turn = false;
                player1.turn = true;
                
                /* write to file in sd card */
                sdSize = sprintf(sdBuffer, "%s %d %d %d", player2.ID, receiveData.pass, 0, 0);
                fp = FS_FOpen(FILENAME, "a");
                FS_FWrite(sdBuffer, sdSize, 1, fp);
                FS_FWrite(&newLine, 1, 1, fp);
                FS_FClose(fp);
            }
            /* packet does not flank */
            else if(flankDir)
            {
                receiveData.seq = seg;
                
                player2.color[receiveData.row][receiveData.col] = 1;   
                Flank(receiveData.row, receiveData.col, flankDir, player2.color, player1.color);
            
                player2.turn = false;
                player1.turn = true;
                
                /* write to file in sd card */
                sdSize = sprintf(sdBuffer, "%s %d %d %d", player2.ID, 0, receiveData.row, receiveData.col);
                fp = FS_FOpen(FILENAME, "a");
                FS_FWrite(sdBuffer, sdSize, 1, fp);
                FS_FWrite(&newLine, 1, 1, fp);
                FS_FClose(fp);
                
                while(!USBUART_CDCIsReady());
                USBUART_PutData((uint8_t*)sdBuffer, sdSize);
                
            };
            
            dataFlag = false;
            isr_rx_Enable();


        }
    }
}

CY_ISR(isr_tx)
{
    if(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
    {
        
        UART_PutChar(dataSend[countS]);
        countS++;

        if(dataSend[countS-1] == 0x0A)
        {
            countS = 0;   
        }
        
    }
}

CY_ISR(isr_rx)
{
    uint8_t rxStatus;
    uint8_t data; 
    if((rxStatus = UART_ReadRxStatus()) & UART_RX_STS_FIFO_NOTEMPTY)
    {
        data = UART_GetChar();
        switch(dataState)
        {
            case START1:
                if(data == 0x55)
                {
                    dataFlag = false;
                    dataRec[dataCount] = data;
                    dataCount++;
                    dataState = START2;   
                }
                else
                {
                    dataCount = 0;
                    dataState = START1;   
                }
                break;
            case START2:
                if(data == 0xAA)
                {
                    dataRec[dataCount] = data;
                    dataCount++;
                    idCount = 0;
                    dataState = ID; 
                }
                else
                {
                    dataState = START1;   
                }
                break;
            case ID:
                if(data == 0x20)
                {
                    strncpy(player2.ID, (char*)(dataRec+2), dataCount-2);
                    
                    dataRec[dataCount] = data;
                    dataCount++;
                    dataFlag = false;
                    dataState = DATA;  
                }
                else
                {
                    dataRec[dataCount] = data;
                    dataCount++;
                    idCount++;
                    dataState = ID;
                }
                break;
            case DATA:
                if(data == 0x55)
                {
                    dataState = START1;
                    dataFlag = true;
                }          
                else
                {
                    dataRec[dataCount] = data;
                    dataCount++;
                    dataState = DATA;
                }
                break;
        }  
    }  
}

CY_ISR(isr_wifi_tx)
{
    while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
    {
        UART_PutChar(adPrint[countS]);
        countS++;

        if(adPrint[countS-1] == 0x0A)
        {
            countS = 0;
            memset(adPrint, '\0', countS);
            isr_wifi_tx_Disable();
            
        }

    }
}

CY_ISR(isr_wifi_rx)
{
    uint8_t rxStatus;
    uint8_t data;
    
    if((rxStatus = UART_ReadRxStatus()) & UART_RX_STS_FIFO_NOTEMPTY)
    {   
        data = UART_GetChar();
        switch(wifiState)
        {
            case PLAYER:
                if(data == 0xA || data == 0x0D)
                {
                    
                    wifiData[wifiCount] = data;
                    wifiCount++;
                    wifiState = NEWLINE;
                }
                else
                {
                    wifiData[wifiCount] = data;
                    wifiCount++;
                    wifiState = PLAYER;
                }
                break;


            case NEWLINE:    
                strncpy(wifiPrint, wifiData, wifiCount);
                memset(wifiData, 0, wifiCount);
                wifiSize = wifiCount;
                wifiCount = 0;
                wifiFlag = true;
                wifiState = PLAYER;
                
                wifiData[wifiCount] = data;
                wifiCount++;
                break;
        } 
    }
}

/* reads from the three arrays - red green blue - and updates board */
CY_ISR(isr)
{   
    static int i=0;
    int j=0;

    Control_Reg_OE_Write(1);
    Control_Reg_LAT_Write(0);
    Control_Reg_ABC_Write(i);

    for(j=0;j<ROW_SIZE;j++)
    {
        if(ROW_SIZE>8)
        {
            Control_Reg_RGB_1_Write(red[i][j]|((cursor[i][j])<<1)|((blue[i][j])<<2));
            Control_Reg_RGB_2_Write(red[i+8][j]|((cursor[i+8][j])<<1)|((blue[i+8][j])<<2));             
        }
        else
        {
            Control_Reg_RGB_1_Write(red[i][j]|((cursor[i][j])<<1)|((blue[i][j])<<2));
        }
        Control_Reg_CLK_Write(1);
        Control_Reg_CLK_Write(0); 
    }

    Control_Reg_RGB_1_Write(0);
    Control_Reg_RGB_2_Write(0);
    for(j=0;j<(32-ROW_SIZE);j++)
    {
        Control_Reg_CLK_Write(1);
        Control_Reg_CLK_Write(0);
    }

    Control_Reg_LAT_Write(1);
    Control_Reg_LAT_Write(0);
    Control_Reg_OE_Write(0);
    
    i++;
    
    if(ROW_SIZE == 16)
    {
        if(i==(.5*ROW_SIZE))
        {
            i=0;   
        }
    }
    else if(i == ROW_SIZE)
    {
        i=0;   
    }

    ISRFlag = TRUE;
    
}
