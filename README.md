# Reversi

This is a software package for a 2-player board game Reversi, that can be
played through wifi and controlled through a keyboard. This includes a library
for the UART communication, the game logic, and interfacing with the wifi card.

## Overview

The block diagram below outlines the interface between the microcontroller and
each peripheral. 

![Reversi Block Diagram](http://i.imgur.com/XKzO4lu.png?style=centerme)

The schematic below shows the pinout and a few basic voltage dividers to step
a 5V signal down to a 3.3V signal for the microcontroller and SD card reader to
use.

![Reversi Schematic](http://i.imgur.com/bCZ7OCB.png?style=centerme)
