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

#include "wifi.h"

#define ASCI_DIFF 48
#define TENS 10
#define HUNDREDS 100

uint32_t IpVal(char string[IP_STRLEN])
{
    uint32_t val = 0;
    
    uint32_t first = 0, second = 0, third = 0, fourth = 0;
    
    first = ((string[12]-ASCI_DIFF) + TENS*(string[11]-ASCI_DIFF) + HUNDREDS*(string[10]-ASCI_DIFF));
    second = (string[8]-ASCI_DIFF) << 8;
    third = ((string[6]-ASCI_DIFF) + TENS*(string[5]-ASCI_DIFF) + HUNDREDS*(string[4]-ASCI_DIFF)) << 16;
    fourth = ((string[2]-ASCI_DIFF) + TENS*(string[1]-ASCI_DIFF) + HUNDREDS*(string[0]-ASCI_DIFF));
    
    val = first + second + third + fourth;
    
    return val;
}

/* [] END OF FILE */
