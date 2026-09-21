#include <Control_Surface.h>

// Lib setup
extern USBMIDI_Interface USB_MIDI;
extern HardwareSerialMIDI_Interface SERIAL_MIDI(Serial1, MIDI_BAUD);
// ?HardwareSerialDebugMIDI_Interface DEBUG_MIDI(Serial, 9600);
extern USBDebugMIDI_Interface DEBUG_MIDI;
extern BluetoothMIDI_Interface BLE_MIDI;

extern BidirectionalMIDI_PipeFactory<2> pipes;

struct transportCfg {
    uint8_t source1;
    uint8_t source2;
    uint8_t sink1;
    uint8_t sink2;
};

TrueMIDI_SinkSource* TransInterfaces[] = {
    &Control_Surface,
    &USB_MIDI,
    &BLE_MIDI,
    &SERIAL_MIDI,
    &DEBUG_MIDI,
};