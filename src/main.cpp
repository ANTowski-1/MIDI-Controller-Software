/*This file contains main functions of the ESP32 MIDI controller (setup and loop).

File is integral part of MIDI-Controller's software, full source code, and license is avalible on github:
https://github.com/ANTowski-1/MIDI-Controller-Software/tree/main

Copyright 2026, Antoni Kołaczek
Licensed under GPL-3.0.*/

#include <Arduino.h>
#include <Control_Surface.h>
#include <main.cpp_includes.h>


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

// Config variables
int lastConfBtnCheck{};
int lastConfBtnState{1};
int currentConfBtnState{};

// Control Surface bank allowing to stop sending messeges while in Config Mode
OutputBank configBank(1);

// Get specified encoder value 
int csGetEncVal(int num){
    return enc[num].getValue();
}

// Call display function to update display for changes Control Surfaces (or when it's forced)
void potBtnUpdate(bool force) {
    ScreenUpdated = false;
    while (ScreenUpdated == false) {
        // Update mute buttons and faders
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

        // Update encoders
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

    // Print build and copyright info.
    Serial0.println("MIDI controller by ANTON");
    Serial0.print("Copyright ");
    Serial0.print(__DATE__ + 7); // Shift date so it only displays YEAR
    Serial0.println(", Antoni Kołaczek");
    Serial0.println("Licensed under GPL-3.0.");
    Serial0.print("Code version: ");
    Serial0.println(VERSION);
    Serial0.print("Code build date: ");
    Serial0.println(__DATE__);
    loadCSToTransIntStruct();
    loadCfg();
    applyCfg();
    Control_Surface.begin();
    if (debugMode == 1) Serial0.println("CS Begun");
    pinMode(BTN_ENC1, INPUT);
    configBank.select(0);
    tft_init();
    potBtnUpdate(true);
    lastDispUpdate = millis();
    lastConfBtnCheck = millis();
    if (debugMode == 1) Serial0.println("Setup complete");
}

void loop() {
    if (debugMode == 1) Serial0.println("Debug: Loop Started");

    Control_Surface.loop();

    if (millis() - lastDispUpdate >= 100) {
        potBtnUpdate(false);
    } // Display Update

    // Open settings page, and slow down encoder
    if (lastConfBtnCheck - millis() >= 100) {
        currentConfBtnState = digitalRead(BTN_ENC1);
        if (digitalRead(BTN_ENC1) == 1 && currentConfBtnState != lastConfBtnState) {
            enc[0].setSpeedMultiply(1);
            settings();
            lcdClear();
            potBtnUpdate(true);
            enc[0].setSpeedMultiply(4);
        }
        lastConfBtnState = currentConfBtnState;
    }

    if (debugMode == 1) Serial0.println("Debug: Loop Finished");
}
