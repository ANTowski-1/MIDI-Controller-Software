/*This file contains extern function definitions for display and configuration to work, and Display pin definitions.

File is integral part of MIDI-Controller's software, full source code, and license is avalible on github:
https://github.com/ANTowski-1/MIDI-Controller-Software/tree/main

Copyright 2026, Antoni Kołaczek
Licensed under GPL-3.0.*/

// Display pin definitions
#define TFT_CS 14
#define TFT_DC 17
#define TFT_RST 18
#define TFT_MISO 13
#define TFT_MOSI 11
#define TFT_SCK 12


// Display Functions
extern void tft_init();
extern void muteDisplay(int btnNum, bool muted);
extern void fadersDisplay(int fadNum, int fadPos);
extern void fadBtnClear(int fadBtnNum);
extern void lcdClear();
extern void settings();
extern void encDisplay(int encNum, int encVal);
extern void menuTab(int tabNum);


// Config Functions
extern void loadCfg();
extern void saveCfg();
extern void applyCfg();
extern void saveConfByTab(int tabNum, int chosenOption);
extern void loadCSToTransIntStruct();
