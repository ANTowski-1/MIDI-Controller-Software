#include <SPI.h>
#include <display.h>
#include <bb_spi_lcd.h>
#include <string>
#include <iostream>
#include <Control_Surface.h>
#include <main.h>

BB_SPI_LCD lcd;

// Display Colors Palette — used palette:
// colorspaletts.net #4805
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
extern pin_t BTN_ENC2;

char CCValPadded[4];

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

void fadersDisplay(int fadNum, int fadPos) {
    int fadPosition = 30 + 50 * fadNum;
    int fadBarPosition = 15 + 50 * fadNum;
    int fadBarVerticalPosition = 70 + (127 - fadPos) * 90/127;
    // lcd.fillRect(int(fadBarPosition), 70, 40, 120, TFT_BLACK);
    lcd.fillRect(int(fadPosition), 70, 10, 100, LCD_UI);
    lcd.fillRect(int(fadBarPosition), fadBarVerticalPosition, 40, 10, LCD_UI);
    lcd.setFont(FONT_8x8);
    lcd.setTextColor(LCD_TXT, LCD_BG);
    lcd.setCursor((fadBarPosition + 8), 175);
    snprintf(CCValPadded, sizeof(CCValPadded), "%03d", fadPos);
    lcd.print(CCValPadded);
    // lastFadVal[fadNum] = fadPos;
}

void fadBtnClear(int fadBtnNum){
    int clearXPos = 15 + 50 * fadBtnNum;
    lcd.fillRect(clearXPos, 70, 40, 120, LCD_BG);
}

void encDisplay(int encNum, int encVal){
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

/*
Settings — How to idea:
Make cards, seperate for each setting, beetwen which you switch using button, 
and choose options using encoder. Swiching cards also saves the settings changes.

Order of subtasks:
    Show new tab with options, based on options arrays
    Integrate encoders to make selection
    Add callback to esp32 NVS or pass the config in other way, based on the need
*/

void settings(){
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
    while (digitalRead(BTN_ENC2) == 0) {
        delay(1); // Wait until enc button stops being pressed.
    }
    lastEncCheck = millis();
    lastEncValue = csGetEncVal(0);
    while(true){
        Control_Surface.loop();
        if (lastEncCheck - millis() >= 10){
            if (lastEncValue < csGetEncVal(1)) {
                if (chosenOption < 5) {
                    chosenOption++;
                } else if (chosenOption >= 4) {
                    chosenOption = 0;
                }
                lastEncValue = csGetEncVal(1);
                Serial.println(lastEncValue);
                int yPosition = 55 + 30 * chosenOption;
                int lastYPosition = 55 + 30 * lastChosenOption;
                lcd.fillRect(45, lastYPosition, 10, 10, LCD_BG);
                lcd.fillRect(45, yPosition, 10, 10, LCD_2TXT);
                lastChosenOption = chosenOption;
            } else if (lastEncValue > csGetEncVal(1)) {
                if (chosenOption > 0) {
                    chosenOption--;
                } else if (chosenOption <= 0) {
                    chosenOption = 5;
                }
                lastEncValue = csGetEncVal(1);
                Serial.println(lastEncValue);
                int yPosition = 55 + 30 * chosenOption;
                int lastYPosition = 55 + 30 * lastChosenOption;
                lcd.fillRect(45, lastYPosition, 10, 10, LCD_BG);
                lcd.fillRect(45, yPosition, 10, 10, LCD_2TXT);
                lastChosenOption = chosenOption;
            } else if (digitalRead(BTN_ENC2) == 0){
                if (chosenOption == 5) {
                    return;
                } else {
                    menuTab(chosenOption);
                    while (digitalRead(BTN_ENC2) == 0) {
                        delay(10); // Wait until enc buttons stops being pressed.
                    }
                    return;
                }
            }
        }
    }
}

void menuTab(int tabNum){
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
        while (digitalRead(BTN_ENC2) == 0) {
            delay(10); // wait until Enc Btn stopps being pressed
        } // while BTN_ENC2 == low
        while(true){
            Control_Surface.loop();
            if (lastEncCheck - millis() >= 10){
                if (lastEncValue > csGetEncVal(1)) {
                    if (chosenOption < 4) {
                        chosenOption++;
                    } else if (chosenOption >= 4) {
                        chosenOption = 0;
                    }
                    lastEncValue = csGetEncVal(1);
                    Serial.println(lastEncValue);
                    int yPosition = 55 + 35 * chosenOption;
                    int lastYPosition = 55 + 35 * lastChosenOption;
                    lcd.fillRect(45, lastYPosition, 10, 10, LCD_BG);
                    lcd.fillRect(45, yPosition, 10, 10, LCD_2TXT);
                    lastChosenOption = chosenOption;
                } else if (lastEncValue < csGetEncVal(1)) {
                    if (chosenOption > 0) {
                        chosenOption--;
                    } else if (chosenOption <= 0) {
                        chosenOption = 4;
                    }
                    lastEncValue = csGetEncVal(1);
                    Serial.println(lastEncValue);
                    int yPosition = 55 + 35 * chosenOption;
                    int lastYPosition = 55 + 35 * lastChosenOption;
                    lcd.fillRect(45, lastYPosition, 10, 10, LCD_BG);
                    lcd.fillRect(45, yPosition, 10, 10, LCD_2TXT);
                    lastChosenOption = chosenOption;
                } else if (digitalRead(BTN_ENC2) == 0){
		            saveConfByTab(tabNum, chosenOption);
                    ESP.restart(); // For now ESP is restarted to disconnect all pipes, and connect then the new way at start up.
                }   // If
                lastEncCheck = millis();
            }   // If (lastEncCheck - millis(10) > 10)
        }   //While
    } else if (tabNum == 4){
        lcd.setCursor(40, 50);
        lcd.setFont(FONT_12x16);
        lcd.setTextColor(LCD_TXT, LCD_BG);
        lcd.print("MIDI Mixer by Anton.");
        lcd.setCursor(40, 75);
        lcd.print("Code revision: ");
        lcd.print(String(VERSION));
        lcd.setCursor(40, 100);
        lcd.print("Licenses:");
        lcd.setCursor(60, 120);
        lcd.print("Software:");
        lcd.setCursor(80, 140);
        lcd.print("GNU GPL v3.0");
        lcd.setCursor(60, 160);
        lcd.print("Hardware:");
        lcd.setCursor(80, 180);
        lcd.print("CERN-OHL-S V2+");
        lcd.setCursor(40, 200);
        lcd.print("Project repo: ");
        lcd.setCursor(40, 220);
        lcd.print("bit.ly/MIDI-mixer-by-ANTON");
        
        while (digitalRead(BTN_ENC2) == 0) {
            delay(10); // Wait untill ENC Btn is stopped being pressed.
        }

        while (true){
            if (millis() - lastEncCheck > 100 && digitalRead(BTN_ENC2) == 0){
                return;
            }
        }
    }
}


void lcdClear(){
    lcd.fillScreen(LCD_BG);
}


