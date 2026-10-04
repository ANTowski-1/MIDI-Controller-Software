/*This file contains mechanics allowing for configuring the needed output of the MIDI mixer.
    
File is integral part of MIDI-Controller's software, full source code, and license is avalible on github:
https://github.com/ANTowski-1/MIDI-Controller-Software/tree/main

Copyright 2026, Antoni Kołaczek
Licensed under GPL-3.0.*/

#include <Control_Surface.h>
#include <Preferences.h>


// Define Preferences instance.
Preferences config;

// External MIDI Interaces and pipes.
extern USBMIDI_Interface USB_MIDI;
extern HardwareSerialMIDI_Interface SERIAL_MIDI;
extern HardwareSerialDebugMIDI_Interface DEBUG_MIDI;
extern BluetoothMIDI_Interface BLE_MIDI;

extern BidirectionalMIDI_PipeFactory<2> pipes;

// Struct holding all options connections, and define it.
struct transportCfg {
    uint8_t source1;
    uint8_t source2;
    uint8_t sink1;
    uint8_t sink2;
};

transportCfg cfg;

// Array holding pointers to each of the MIDI interfaces
TrueMIDI_SinkSource *transportInterfaces[] = {
    nullptr, // slot 0 (Control_Surface) added at runtime
    &USB_MIDI,
    &BLE_MIDI,
    &SERIAL_MIDI,
    &DEBUG_MIDI,
};

// Function for loading the config from NVS partition into the transportCfg stuct.
void loadCfg() {
    config.begin("midiTransport", true);
    cfg.source1 = config.getUInt("source1", 0); // 1st source, default to Control Surface
    cfg.source2 = config.getUInt("source2", 0); // 2nd source, default to Control Surface
    cfg.sink1 = config.getUInt("sink1", 1); // 1st sink, default to USB
    cfg.sink2 = config.getUInt("sink2", 3); // 2nd sink, default to SERIAL
    Serial0.printf("cfg: %u %u %u %u\n", cfg.source1, cfg.source2, cfg.sink1, cfg.sink2);
    config.end();
}

// Function saving the confing into NVS
void saveCfg() {
    config.begin("midiTransport", false);
    config.putUInt("source1", cfg.source1);
    config.putUInt("source2", cfg.source2);
    config.putUInt("sink1", cfg.sink1);
    config.putUInt("sink2", cfg.sink2);
    config.end(); 
}

// Function appling the config based on the transportCfg struct
void applyCfg() {
    if (debugMode == 1) {
        Serial0.print("cfg: ");
        Serial0.print(cfg.source1);
        Serial0.print(cfg.source2);
        Serial0.print(cfg.sink1);
        Serial0.println(cfg.sink2);
        for (int i = 0; i < 5; i++)
            Serial0.printf("iface[%d] = %p\n", i, (void*)transportInterfaces[i]);
    }

    *transportInterfaces[cfg.source1] | pipes | *transportInterfaces[cfg.sink1];
    if (debugMode == 1) Serial0.println("Conn 1 OK");
    *transportInterfaces[cfg.source2] | pipes  | *transportInterfaces[cfg.sink2];
    if (debugMode == 1) Serial0.println("Conn 2 OK");
}

// Function saving the config based on which tab of settings it's called from
void saveConfByTab(int tabNum, int chosenOption) {
    if (tabNum == 0) {
	cfg.source1 = chosenOption;
    } else if (tabNum == 1) {
	cfg.sink1 = chosenOption;
    } else if (tabNum == 2) {
	cfg.source2 = chosenOption;
    } else if (tabNum == 3) {
	cfg.sink2 = chosenOption;
    }
    saveCfg();
}

// Change nullptr in transportInterfaces array to Control_Surface ptr
void loadCSToTransIntStruct() {
    transportInterfaces[0] = &Control_Surface;
}
