// Include
#include <Arduino.h>
#include <Control_Surface.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino_Helpers.h>
#include <Wire.h>

#include <AH/Hardware/ExtendedInputOutput/MCP23017.hpp>

using WireType = decltype(Wire);

MCP23017<WireType> mcp {
    Wire,
    0x25,
};
MCP23017<WireType> mcp2 {
    Wire,
    0x24,
};


// MCP Pins definitions

pin_t MBTN1 = mcp2.pinB(0);
pin_t MBTN2 = mcp2.pinB(1);
pin_t MBTN3 = mcp2.pinB(2);
pin_t MBTN4 = mcp2.pinB(3);
pin_t MBTN5 = mcp2.pinB(4);
pin_t MBTN6 = mcp2.pinB(5);
pin_t BTN2 = mcp2.pinA(6);

pin_t BTN_ENC1 = mcp.pinA(2);
pin_t BTN_ENC2 = mcp.pinA(5);
pin_t BTN1 = mcp.pinA(6);

pin_t BTN3 = mcp.pinB(0);
pin_t BTN4 = mcp.pinB(1);
pin_t BTN5 = mcp.pinB(2);
pin_t BTN6 = mcp.pinB(3);
pin_t BTN7 = mcp.pinB(4);
pin_t BTN8 = mcp.pinB(5);
pin_t BTN9 = mcp.pinB(6);

// Variables
uint8_t lastKnobVal[2] = {255, 255};
bool lastBtnState[4] = {};
uint32_t lastUpdate = 0;

// Lib setup
USBMIDI_Interface midi;
//USBDebugMIDI_Interface midi;
// LiquidCrystal_I2C lcd(0x27, 16, 2);

// Controls
CCButton buttons[] {
    {BTN1, {110, Channel_1}},
    {BTN2, {111, Channel_1}},
    {BTN3, {112, Channel_1}},
    {BTN4, {113, Channel_1}},
    {BTN5, {114, Channel_1}},
    {BTN6, {115, Channel_1}},
    {BTN7, {116, Channel_1}},
    {BTN8, {117, Channel_1}},
    {BTN9, {118, Channel_1}},
    {BTN_ENC2, {119, Channel_1}},
};

CCButtonLatched muteButtons[] {
    {MBTN1, {20, Channel_1}},
    {MBTN2, {21, Channel_1}},
    {MBTN3, {22, Channel_1}},
    {MBTN4, {23, Channel_1}},
    {MBTN5, {24, Channel_1}},
    {MBTN6, {25, Channel_1}},
};

CCPotentiometer pots[] {
    {1,	{102, Channel_1}},
    {2,	{103, Channel_1}},
    {3,	{104, Channel_1}},
    {4,	{105, Channel_1}},
    {5,	{106, Channel_1}},
    {6,	{107, Channel_1}},
};


CCAbsoluteEncoder enc2[] {
    {{9, 8}, {108, Channel_1}, 4},
    {{40, 10}, {109, Channel_1}, 4},
};
// void updateDisplay() {
//     uint8_t k0 = knobs[0].getValue();
//     uint8_t k1 = knobs[1].getValue();

//     bool btns[4];
//     bool btnChanged = false;
//     for (uint8_t i = 0; i < 4; i++) {
//         btns[i] = buttons[i].getButtonState() == Button::Pressed;
//         if (btns[i] != lastBtnState[i]) btnChanged = true;
//     }

//     bool knobChanged = (k0 != lastKnobVal[0] || k1 != lastKnobVal[1]);
//     if (!knobChanged && !btnChanged) return; // nothing to redraw

//     // Update tracked state
//     lastKnobVal[0] = k0;
//     lastKnobVal[1] = k1;
//     for (uint8_t i = 0; i < 4; i++) lastBtnState[i] = btns[i];

//     // Row 0: knobs
//     lcd.setCursor(0, 0);
//     lcd.print("K1:");
//     lcd.print(k0);
//     lcd.print("  K2:");
//     lcd.print(k1);
//     lcd.print("   ");

//     // Row 1: buttons
//     lcd.setCursor(0, 1);
//     for (uint8_t i = 0; i < 4; i++) {
//         lcd.print("B");
//         lcd.print(i + 1);
//         lcd.print("");
//         lcd.print(btns[i] ? "\xFF " : "_ ");
//     }
// }

void setup() {
    Wire.begin(47, 48);
    Serial.begin(9600);
    // lcd.init();
    // lcd.backlight();

    // lcd.setCursor(0, 0);
    // lcd.print("Control Surface");
    // lcd.setCursor(0, 1);
    // lcd.print("  Starting...  ");
    delay(100);
    Control_Surface.begin();
    // lcd.clear();
}

void loop() {
    Control_Surface.loop();

    // uint32_t now = millis();
    // if (now - lastUpdate >= 100) {
    //     lastUpdate = now;
    //     updateDisplay();
    // }
}
