#include <Control_Surface.h>

// Lib setup
extern USBMIDI_Interface USB_MIDI;
extern HardwareSerialMIDI_Interface SERIAL_MIDI;
extern HardwareSerialDebugMIDI_Interface DEBUG_MIDI;
// extern USBDebugMIDI_Interface DEBUG_MIDI;
//extern BluetoothMIDI_Interface BLE_MIDI;

extern BidirectionalMIDI_PipeFactory<2> pipes;
extern MIDI_Pipe pipe_tx1;

struct transportCfg {
    uint8_t source1;
    uint8_t source2;
    uint8_t sink1;
    uint8_t sink2;
};

transportCfg cfg;

TrueMIDI_SinkSource *transportInterfaces[] = {
    nullptr, // slot 0 (Control_Surface) added at runtime
    &USB_MIDI,
    nullptr, // BLE Midi is temperarly deleted, as for arduino framework 3.3.7 nimBLE problems.
    &SERIAL_MIDI,
    &DEBUG_MIDI,
};

void loadCfg() {
    config.begin("midiTransport", true);
    cfg.source1 = config.getUInt("source1", 0); // 1st source, default to Control Surface
    cfg.source2 = config.getUInt("source2", 0); // 2nd source, default to Control Surface
    cfg.sink1 = config.getUInt("sink1", 1); // 1st sink, default to USB
    cfg.sink2 = config.getUInt("sink2", 3); // 2nd sink, default to SERIAL
    Serial0.printf("cfg: %u %u %u %u\n", cfg.source1, cfg.source2, cfg.sink1, cfg.sink2);
    config.end();
}

void saveCfg() {
    config.begin("midiTransport", false);
    config.putUInt("source1", cfg.source1);
    config.putUInt("source2", cfg.source2);
    config.putUInt("sink1", cfg.sink1);
    config.putUInt("sink2", cfg.sink2);
    config.end(); 
}

void applyCfg() {
    Serial0.print("cfg: ");
    Serial0.print(cfg.source1);
    Serial0.print(cfg.source2);
    Serial0.print(cfg.sink1);
    Serial0.println(cfg.sink2);
    for (int i = 0; i < 5; i++)
        Serial0.printf("iface[%d] = %p\n", i, (void*)transportInterfaces[i]);
    Serial0.flush();
    *transportInterfaces[cfg.source1] | pipes | *transportInterfaces[cfg.sink1];
    Serial0.println("Conn 1 OK");
    Serial0.print("USB has Sink Source pipe: ");
    Serial0.print(USB_MIDI.hasSinkPipe());
    Serial0.println(USB_MIDI.hasSourcePipe());
    Serial0.flush();
     *transportInterfaces[cfg.source2] | pipes  | *transportInterfaces[cfg.sink2];
    Serial0.println("Conn 2 OK");
    Serial0.print("Serial has Sink Source pipe: ");
    Serial0.print(SERIAL_MIDI.hasSinkPipe());
    Serial0.println(SERIAL_MIDI.hasSourcePipe());
    Serial0.flush();
    Serial0.println("All pipes connected");
}

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

void loadCSToTransIntStruct() {
    transportInterfaces[0] = &Control_Surface;
}
