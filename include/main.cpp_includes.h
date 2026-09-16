/*This file contains all main.cpp include headers.
    
File is integral part of MIDI-Controller's software, full source code, and license is avalible on github:
https://github.com/ANTowski-1/MIDI-Controller-Software/tree/main

Copyright 2026, Antoni Kołaczek
Licensed under GPL-3.0.*/

// Include
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

// Control_Surface include
#include <Control_Surface.h>
#include <Arduino_Helpers.h>
#include <AH/Hardware/ExtendedInputOutput/MCP23017.hpp>
#include "Control_surface_setup.h"

// Display Include
#include <bb_spi_lcd.h>
#include <display.h>