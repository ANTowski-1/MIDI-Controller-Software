#include <SPI.h>
#include <display.h>
#include <bb_spi_lcd.h>
#include <string>
#include <iostream>
#include <Control_Surface.h>
#include <display_cs.h>

BB_SPI_LCD lcd;

#define TFT_BG TFT_BLACK
String configOptIn[5] = {"Control Surfuce", "USB", "BLE", "Serial", "Debug"};
String configOptOut[5] = {"Control Surfuce", "USB", "BLE", "Serial", "Debug"};
int x = 0;
int lastEncCheck;
int lastEncValue;
int chosenOption{0};
int lastChosenOption{0};
extern pin_t BTN_ENC2;
extern pin_t BTN3;
extern pin_t MBTN1;

void tft_init(){
    SPI.begin(12,13,11);
    lcd.begin(LCD_ST7789, FLAGS_INVERT, 40000000, 14, 17, 18, -1, 13, 11, 12);
    delay(100);
    lcd.setRotation(270);
    lcd.fillScreen(TFT_BLACK);
    lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    lcd.setFont(FONT_12x16);
    Serial.println("Display Have been succesfully initialised!");
}

void tft_draw_circle() {
    lcd.fillCircle(160, 120, 25, TFT_RED);
}

void tft_write_txt(int num) {
    lcd.println(int(num));
}

void muteDisplay(int btnNum, bool muted) {
    int muteBtnPosition = 15 + 50 * btnNum;
    if (muted == true){
        lcd.fillRect(int(muteBtnPosition), 190, 40, 40, TFT_RED);
        //lcd.drawString("Mute", position, 210, FONT_6x8);
    } else if (muted == false) {
        lcd.fillRect(int(muteBtnPosition), 190, 40, 40, TFT_GREY);
        //lcd.drawString("Mute", position, 210, FONT_6x8);
    }
}

void fadersDisplay(int fadNum, int fadPos) {
    int fadPosition = 30 + 50 * fadNum;
    int fadBarPosition = 15 + 50 * fadNum;
    int fadBarVerticalPosition = 70 + (127 - fadPos) * 90/127;
    lcd.fillRect(int(fadPosition), 70, 10, 100, TFT_GREY);
    lcd.fillRect(int(fadBarPosition), fadBarVerticalPosition, 40, 10, TFT_GREY);
    lcd.setFont(FONT_8x8);
    lcd.setCursor(fadPosition, 175);
    lcd.print(int(fadPos));
}

void settings(){

}

void lcdClear(){
    lcd.fillScreen(TFT_BLACK);
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
    lcd.print("Settings");
    while (x<5){
        int yPosition = 50 + 35 * x;
        lcd.drawRect(40, yPosition, 20, 20, TFT_GREY);
        lcd.setFont(FONT_12x16);
        lcd.setCursor(70, yPosition);
        lcd.print(configOptIn[x]);
        x++;
    }
    x = 0;
    lastEncCheck = millis();\
    lastEncValue = csGetEncVal(0);
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
                lcd.fillRect(45, lastYPosition, 10, 10, TFT_BG);
                lcd.fillRect(45, yPosition, 10, 10, TFT_GREEN);
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
                lcd.fillRect(45, lastYPosition, 10, 10, TFT_BG);
                lcd.fillRect(45, yPosition, 10, 10, TFT_GREEN);
                lastChosenOption = chosenOption;
            } else if (csGetEncBtnVal() == false /*|| digitalRead(MBTN1) == 0 || digitalRead(BTN3)*/){
                return;
            }
        }
    }
}

void lcdClear(){
    lcd.fillScreen(TFT_BG);
}


