// Control Surface 3.1.2 USB MIDI interface struct.
// Allows using the device even when it's not connected via USB
// (File from 3.1.3 froze the code the second anything was touched,
// if USB MIDI backend was chosen.)

// Made by Pieter Pas (tttapa), and licenced under GNU-GPL 3.0.
// Modifications made by Antoni Kołaczek only contain adding the top comments.

#include <AH/Containers/Array.hpp>
#include <Settings/NamespaceSettings.hpp>

#include <USB.h>
#include <USBMIDI.h>

BEGIN_CS_NAMESPACE

struct ESP32_USBDeviceMIDIBackend {
    /// USB MIDI packages are 4 bytes.
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    /// Called once upon initialization.
    void begin() {
        backend.begin();
        USB.begin();
    }
    /// Read a single packet. Return a packet of all zeros if there is no packet.
    MIDIUSBPacket_t read() {
        midiEventPacket_t packet {0, 0, 0, 0};
        backend.readPacket(&packet);
        return {packet.header, packet.byte1, packet.byte2, packet.byte3};
    }
    /// Write a single packet to the output buffer.
    void write(MIDIUSBPacket_t p) {
        midiEventPacket_t packet {p[0], p[1], p[2], p[3]};
        backend.writePacket(&packet);
    }
    /// Transmit the output buffer immediately (not implemented).
    void sendNow() {}
    /// No explicit call to sendNow is required.
    bool preferImmediateSend() { return false; }
    /// The actual low-level USB MIDI backend provided by the core libraries.
    USBMIDI backend;
};

END_CS_NAMESPACE