/*This file contains functions needed for utilising the display (main UI, settings page).

File is integral part of MIDI-Controller's software, full source code, and license is avalible on github:
https://github.com/ANTowski-1/MIDI-Controller-Software/tree/main

Copyright 2026, Antoni Kołaczek
Licensed under GPL-3.0.*/

#include <SPI.h>
#include <display_and_conf.h>
#include <bb_spi_lcd.h>
#include <string>
#include <iostream>
#include <Control_Surface.h>
#include <main.h>

BB_SPI_LCD lcd;

// Display Colors Palette
// Default pallette: colorspaletts.net 4805
int LCD_BG = 0x1104;
int LCD_UI = 0x2a69;
int LCD_TXT = 0xdf5d;
int LCD_2TXT = 0x7eb6;
int LCD_MBTN = 0x8123;


// Config Screen Variables
String configCSOpt[5] = {"Control Surfuce", "USB", "BLE", "Serial", "Debug"};
String configMain[6] = {"Pipe 1 Source", "Pipe 1 Sink", "Pipe 2 Source", "Pipe 2 Sink", "About", "Exit"};
int x = 0;
int lastEncCheck;
int lastEncValue;
int chosenOption{0};
int lastChosenOption{0};

// Externally defined config pin
extern pin_t BTN_ENC1;

// Main UI variables
char CCValPadded[4];

// Initialise the display
void tft_init(){
    SPI.begin(12,13,11);
    lcd.begin(LCD_ST7789, FLAGS_INVERT, 40000000, 14, 17, 18, -1, 13, 11, 12);
    delay(100);
    lcd.setRotation(270);
    lcd.fillScreen(LCD_BG);
    lcd.setTextColor(LCD_TXT, LCD_BG);
    lcd.setFont(FONT_12x16);
    Serial.println("Display Have been succesfully initialised!");
}

// Display mute buttons state based on the btn number
void muteDisplay(int btnNum, bool muted) {
    int muteBtnPosition = 15 + 50 * btnNum;
    if (muted == true){
        lcd.setTextColor(LCD_2TXT, LCD_MBTN);
        lcd.setFont(FONT_8x8);
	    lcd.fillRect(int(muteBtnPosition), 190, 40, 40, LCD_MBTN);
        lcd.drawString("Mute", (muteBtnPosition +5), 205);
    } else if (muted == false) {
	    lcd.setTextColor(LCD_TXT, LCD_UI);
        lcd.setFont(FONT_8x8);
        lcd.fillRect(int(muteBtnPosition), 190, 40, 40, LCD_UI);
        lcd.drawString("Mute", (muteBtnPosition +5), 205);
    }
}

// Display faders value graphicly and via text based on the fader number
void fadersDisplay(int fadNum, int fadPos) {
    int fadPosition = 30 + 50 * fadNum;
    int fadBarPosition = 15 + 50 * fadNum;
    int fadBarVerticalPosition = 70 + (127 - fadPos) * 90/127;
    lcd.fillRect(int(fadPosition), 70, 10, 100, LCD_UI);
    lcd.fillRect(int(fadBarPosition), fadBarVerticalPosition, 40, 10, LCD_UI);
    lcd.setFont(FONT_8x8);
    lcd.setTextColor(LCD_TXT, LCD_BG);
    lcd.setCursor((fadBarPosition + 8), 175);
    snprintf(CCValPadded, sizeof(CCValPadded), "%03d", fadPos);
    lcd.print(CCValPadded);
}

// Clear part of the display before displaying new fader value.
void fadBtnClear(int fadBtnNum){
    int clearXPos = 15 + 50 * fadBtnNum;
    lcd.fillRect(clearXPos, 70, 40, 120, LCD_BG);
}

// Display encoder value via text based on encoder number
void encDisplay(int encNum, int encVal){
    // Setting position
    int encPosition = 220 + 60 * encNum;
    int encValPosition = 207 + 60 * encNum;
    lcd.fillCircle((encPosition + 5), 35, 20, LCD_BG);
    lcd.drawCircle(encPosition, 35, 25, LCD_UI);

    // Text printing
    lcd.setCursor(encValPosition, 30);
    lcd.setTextColor(LCD_TXT, LCD_BG);
    lcd.setTextSize(FONT_8x8);
    snprintf(CCValPadded, sizeof(CCValPadded), "%03d", encVal);
    lcd.print(CCValPadded);
}

// Display settings main page, and open correct settings page based on selection.
void settings(){
    // Dsiplay options
	lcdClear();
    lcd.setFont(FONT_16x32);
    lcd.setCursor(96, 15);
    lcd.setTextColor(LCD_2TXT, LCD_BG);
    lcd.print("Settings");
    while (x<6){
        int yPosition = 50 + 30 * x;
        lcd.drawRect(40, yPosition, 20, 20, LCD_UI);
        lcd.setFont(FONT_12x16);
        lcd.setTextColor(LCD_TXT, LCD_BG);
        lcd.setCursor(70, yPosition);
        lcd.print(configMain[x]);
        x++;
    }
    x = 0;
    while (digitalRead(BTN_ENC1) == 0) {
        delay(1); // Wait until enc button stops being pressed.
    }
    // Handle the selection mechanism.
    lastEncCheck = millis();
    lastEncValue = csGetEncVal(0);
    while(true){
        Control_Surface.loop();
        if (millis() - lastEncCheck >= 10){
            if (lastEncValue < csGetEncVal(0)) {
                if (chosenOption < 5) {
                    chosenOption++;
                } else if (chosenOption >= 4) {
                    chosenOption = 0;
                }
                lastEncValue = csGetEncVal(0);
                int yPosition = 55 + 30 * chosenOption;
                int lastYPosition = 55 + 30 * lastChosenOption;
                lcd.fillRect(45, lastYPosition, 10, 10, LCD_BG);
                lcd.fillRect(45, yPosition, 10, 10, LCD_2TXT);
                lastChosenOption = chosenOption;
            } else if (lastEncValue > csGetEncVal(0)) {
                if (chosenOption > 0) {
                    chosenOption--;
                } else if (chosenOption <= 0) {
                    chosenOption = 5;
                }
                lastEncValue = csGetEncVal(0);
                int yPosition = 55 + 30 * chosenOption;
                int lastYPosition = 55 + 30 * lastChosenOption;
                lcd.fillRect(45, lastYPosition, 10, 10, LCD_BG);
                lcd.fillRect(45, yPosition, 10, 10, LCD_2TXT);
                lastChosenOption = chosenOption;
            } else if (digitalRead(BTN_ENC1) == 0){
                if (chosenOption == 5) {
                    return;
                } else {
                    menuTab(chosenOption);
                    while (digitalRead(BTN_ENC1) == 0) {
                        delay(10); // Wait until enc buttons stops being pressed.
                    }
                    return;
                }
            } // Encoder checking if's
        }
    } // while(true)
} // void settings

// Display chosen settings tab, and set chosen options 
void menuTab(int tabNum){
    // Display page name
    lcdClear();
    lcd.setFont(FONT_16x32);
    lcd.setTextColor(LCD_2TXT, LCD_BG);
    lcd.setCursor(40, 15);
    String tabName;
    if (tabNum == 0) {
        tabName = "Pipe 1 Source";
    } else if (tabNum == 1) {
        tabName = "Pipe 1 Sink";
    } else if (tabNum == 2) {
        tabName = "Pipe 2 Source";
    } else if (tabNum == 3) {
        tabName = "Pipe 2 Sink";
    } else if (tabNum == 4) {
        tabName = "About";
    }
    lcd.print(tabName);

    lastEncCheck = millis();
    lastEncValue = csGetEncVal(0);

    // Display MIDI connections configuration page.
    if (tabNum < 4){
        while (x<5){
            int yPosition = 50 + 35 * x;
            lcd.drawRect(40, yPosition, 20, 20, LCD_UI);
            lcd.setFont(FONT_12x16);
            lcd.setTextColor(LCD_TXT, LCD_BG);
            lcd.setCursor(70, yPosition);
            lcd.print(configCSOpt[x]);
            x++;
        }   //While
        x = 0;
        while (digitalRead(BTN_ENC1) == 0) {
            delay(10); // wait until Enc Btn stopps being pressed
        } // while BTN_ENC1 == low

        // Handle selection mechanism, and updating the MIDI connections.
        while(true){
            Control_Surface.loop();
            if (millis() - lastEncCheck >= 10){
                if (lastEncValue > csGetEncVal(0)) {
                    if (chosenOption < 4) {
                        chosenOption++;
                    } else if (chosenOption >= 4) {
                        chosenOption = 0;
                    }
                    lastEncValue = csGetEncVal(0);
                    int yPosition = 55 + 35 * chosenOption;
                    int lastYPosition = 55 + 35 * lastChosenOption;
                    lcd.fillRect(45, lastYPosition, 10, 10, LCD_BG);
                    lcd.fillRect(45, yPosition, 10, 10, LCD_2TXT);
                    lastChosenOption = chosenOption;
                } else if (lastEncValue < csGetEncVal(0)) {
                    if (chosenOption > 0) {
                        chosenOption--;
                    } else if (chosenOption <= 0) {
                        chosenOption = 4;
                    }
                    lastEncValue = csGetEncVal(0);
                    int yPosition = 55 + 35 * chosenOption;
                    int lastYPosition = 55 + 35 * lastChosenOption;
                    lcd.fillRect(45, lastYPosition, 10, 10, LCD_BG);
                    lcd.fillRect(45, yPosition, 10, 10, LCD_2TXT);
                    lastChosenOption = chosenOption;
                } else if (digitalRead(BTN_ENC1) == 0){
		            saveConfByTab(tabNum, chosenOption);
                    ESP.restart(); // For now ESP is restarted to disconnect all pipes, and connect then the new way at start up.
                }   // If
                lastEncCheck = millis();
            }   // If (lastEncCheck - millis(10) > 10)
        }   //While

    // Display about page
    } else if (tabNum == 4){
        lcd.setCursor(40, 50);
        lcd.setFont(FONT_12x16);
        lcd.setTextColor(LCD_TXT, LCD_BG);
        lcd.print("MIDI Mixer by Anton.");
        lcd.setCursor(40, 75);
        lcd.print("Code revision: ");
        lcd.print(String(VERSION));
        lcd.setCursor(40, 95);
        lcd.print("Licenses:");
        lcd.setCursor(50, 115);
        lcd.print("Software:");
        lcd.setCursor(160, 115);
        lcd.print("GNU GPL v3.0");
        lcd.setCursor(50, 135);
        lcd.print("Hardware:");
        lcd.setCursor(160, 135);
        lcd.print("CERN-OHL-S V2+");
        lcd.setCursor(40, 155);
        lcd.print("Project repo: ");
        lcd.setCursor(50, 175);
        lcd.print("bit.ly/ANTON-MIDI");
        lcd.setCursor(40, 195);
        lcd.print("Soft builded on:");
        lcd.setCursor(50, 215);
        lcd.print(__DATE__);

        while (digitalRead(BTN_ENC1) == 0) {
            delay(10); // Wait untill ENC Btn is stopped being pressed.
        }

        // Exit
        while (true){
            if (millis() - lastEncCheck > 100 && digitalRead(BTN_ENC1) == 0){
                return;
            }
        }
    }
}

// Clear the whole display
void lcdClear(){
    lcd.fillScreen(LCD_BG);
}


