/*This file contains Control Surface header configuration:
    - MCP info
    - MCP pins
    - Transport Modes
    - Avalible controls
    
File is integral part of MIDI-Controller's software, full source code, and license is avalible on github:
https://github.com/ANTowski-1/MIDI-Controller-Software/tree/main

Copyright 2026, Antoni Kołaczek
Licensed under GPL-3.0.*/



// MCP connection data

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

pin_t ledm1 = mcp2.pinA(0);
pin_t ledm2 = mcp2.pinA(1);
pin_t ledm3 = mcp2.pinA(2);
pin_t ledm4 = mcp2.pinA(3);
pin_t ledm5 = mcp2.pinA(4);
pin_t ledm6 = mcp2.pinA(5);



// Lib setup
USBMIDI_Interface midi;
//USBDebugMIDI_Interface midi;


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
    {{40, 10}, {108, Channel_1}, 4},
    {{9, 8}, {109, Channel_1}, 4},
};
