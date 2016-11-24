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


uint32_t IpVal(char string[13])
{
    uint32_t val = 0;
    
    uint32_t first = 0, second = 0, third = 0, fourth = 0;
    
    first = ((string[12]-48) + 10*(string[11]-48) + 100*(string[10]-48));
    second = (string[8]-48) << 8;
    third = ((string[6]-48) + 10*(string[5]-48) + 100*(string[4]-48)) << 16;
    fourth = ((string[2]-48) + 10*(string[1]-48) + 100*(string[0]-48));
    
    val = first + second + third + fourth;
    
    return val;
}

/* [] END OF FILE */
