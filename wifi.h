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
#include "com.h"

#define AD_SIZE 17
#define IP_STRLEN 13

typedef enum
{
    PLAYER,
    NEWLINE
} WIFI_STATES;

typedef enum
{
    ADVERTISE,
    CONNECT,
} SETUP_STATES;

uint32_t IpVal(char string[13]);


/* [] END OF FILE */
