#include <Arduino.h>
#include <Control_Surface.h>
#include <main.cpp_includes.h>


extern Preferences config;

bool inConfigMode = false;
int menuSelection = 0;

// Screen update variables
int lastDispUpdate = 0;

int lastBtnPotUpdate = 0;
int currentBtnPotUpdate = 0;
bool currentMBtnState = false;
bool lastMBtnState[6] = {0,0,0,0,0,0};
int currentFaderPosition = 0;
int lastFadVal[6] = {0,0,0,0,0,0};
bool ScreenUpdated = false;
int i{0};
int lastUpdatedEnc = 0;
int currentEncUpdate = 0;
int currentEncVal{1};
int lastEncVal{127};

int lastConfBtnCheck{};
int lastConfBtnState{1};
int currentConfBtnState{};

OutputBank configBank(1);

int csGetEncVal(int num){
    return enc[num].getValue();
}

void potBtnUpdate(bool force) {
    ScreenUpdated = false;
    while (ScreenUpdated == false) {
            if (lastBtnPotUpdate <= 6) {
                currentBtnPotUpdate++;
                currentMBtnState = muteButtons[currentBtnPotUpdate - 1].getState();
                currentFaderPosition = pots[currentBtnPotUpdate - 1].getValue();
                if (lastFadVal[currentBtnPotUpdate -1] != currentFaderPosition 
                    || lastMBtnState[currentBtnPotUpdate -1] != currentMBtnState || force == true) {
                    fadBtnClear(currentBtnPotUpdate -1);
                    muteDisplay((currentBtnPotUpdate -1), currentMBtnState);
                    fadersDisplay((currentBtnPotUpdate -1), currentFaderPosition);
                    lastFadVal[currentBtnPotUpdate -1] = currentFaderPosition;
                    lastMBtnState[currentBtnPotUpdate -1] = currentMBtnState;
                }
                lastBtnPotUpdate = currentBtnPotUpdate;
            } else if (lastBtnPotUpdate > 6) {
                lastBtnPotUpdate = 0;
                currentBtnPotUpdate = 0;
                lastUpdatedEnc = 0;
                currentEncUpdate = 0;
                ScreenUpdated = true;
                lastDispUpdate = millis();
            } // Pot&MBtn Update
            if (lastUpdatedEnc < 2) {
                currentEncUpdate++;
                currentEncVal = enc[currentEncUpdate - 1].getValue();
                if (lastEncVal != currentEncVal || force == true) {
                    encDisplay((currentEncUpdate - 1), currentEncVal);
                    lastEncVal = currentEncVal;
                }
            lastUpdatedEnc = currentEncUpdate;
        } // Enc Update
    } // While loop
}

void setup() {
    Wire.begin(47, 48);
    Serial0.begin(115200);
    Serial1.begin(MIDI_BAUD, SERIAL_8N1, 42, 41);
    delay(200);
    loadCSToTransIntStruct();
    loadCfg();
    applyCfg();
    Control_Surface.begin();
    if (debugMode == 1){
        Serial0.println("CS Begun");
    }
    Serial0.flush();
    pinMode(BTN_ENC2, INPUT);
    configBank.select(0);
    tft_init();
    potBtnUpdate(true);
    lastDispUpdate = millis();
    lastConfBtnCheck = millis();
    Serial0.println("Setup complete");
}

void loop() {
    if (debugMode == 1){
        Serial0.println("Debug: Loop Started");
    }

    Control_Surface.loop();
    if (millis() - lastDispUpdate >= 100) {
        potBtnUpdate(false);
    } // Display Update

    if (lastConfBtnCheck - millis() >= 100) {
        currentConfBtnState = digitalRead(BTN_ENC2);
        if (digitalRead(BTN_ENC2) == 1 && currentConfBtnState != lastConfBtnState) {
            enc[1].setSpeedMultiply(1);
            settings();
            lcdClear();
            potBtnUpdate(true);
            enc[1].setSpeedMultiply(4);
        }
        lastConfBtnState = currentConfBtnState;
    }

    if (debugMode == 1){
        Serial0.println("Debug: Loop Finished");
    }
}
