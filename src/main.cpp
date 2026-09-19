#include <Arduino.h>
#include <Control_Surface.h>
#include <main.cpp_includes.h>

bool inConfigMode = false;
int menuSelection = 0;

// Screen update variables
int lastDispUpdate = 0;

int lastBtnPotUpdate = 0;
int currentBtnPotUpdate = 0;
bool currentMBtnState = false;
int currentFaderPosition = 0;
bool ScreenUpdated = false;

OutputBank configBank(1);

int csGetEncVal(int num){
    return enc[num].getValue();
}
bool csGetEncBtnVal(){
    return (buttons[9].getButtonState() || muteButtons[0].getButtonState());
}

void setup() {
    Control_Surface.begin();
    Wire.begin(47, 48);
    delay(200);
    // pinMode(BTN_ENC2, INPUT_PULLUP);
    // pinMode(BTN3, INPUT_PULLUP);
    // pinMode(MBTN1, INPUT_PULLUP);
    Serial.begin(9600);
    configBank.select(0);

    tft_init();
    // while (i < 6){
    //     muteDisplay(i, false);
    //     fadersDisplay(i, 0);
    //     lastFadVal[i] = 0;
    //     lastMBtnState[i] = false;
    // }
    //configBank.select(1);
    enc[1].setSpeedMultiply(1);
    settings();
    enc[1].setSpeedMultiply(4);
    lastDispUpdate = millis();
    Serial.println("Setup complete");
}

void loop() {
    Control_Surface.loop();
    if (millis() - lastDispUpdate >= 100) {
        ScreenUpdated = false;
        lcdClear();
        while (ScreenUpdated == false) {
            if (lastBtnPotUpdate <= 6) {
                currentBtnPotUpdate++;
                
                //Mute Button Update
                currentMBtnState = muteButtons[currentBtnPotUpdate - 1].getState();
                muteDisplay((currentBtnPotUpdate -1), currentMBtnState);
                
                //Fader Update
                currentFaderPosition = pots[currentBtnPotUpdate - 1].getValue();
                fadersDisplay((currentBtnPotUpdate -1), currentFaderPosition);
                lastBtnPotUpdate = currentBtnPotUpdate;
            } else if (lastBtnPotUpdate > 6) {
                lastBtnPotUpdate = 0;
                currentBtnPotUpdate = 0;
                ScreenUpdated = true;
            }
        }
    }

    // if (digitalRead(configBtn) == LOW) {
    //     inConfigMode = !inConfigMode;

    //     if (inConfigMode = true) {
    //         configBank.select(1);
    //     } else {
    //         configBank.select(0); 
    //     }
    //     while(digitalRead(configBtn) == LOW);
    // }
}