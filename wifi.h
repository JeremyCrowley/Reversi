/* ========================================
 * Author: Jeremy Crowley
 * File: wifi.h
 *
 * Purpose: This file provides the enums and headers for the wifi communication
 * through the LaunchPad - CC3200
 * ========================================
 */

#ifndef WIFI_H
#define WIFI_H

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


/* @desc - This function will take in the IP address as a string and return 
 * the address as a 32 bit value
 *
 * @param - the ip address of the other player as a string
 * @return - the ip address of the other player as a uint32
 */ 
uint32_t IpVal(char string[IP_STRLEN]);


#endif 
/* WIFI_H */
