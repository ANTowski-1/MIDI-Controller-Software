import shutil
import os

patch = "include/USBMIDI_ESP32.hpp"
original = ".pio/libdeps/esp32-s3-devkitc1-n4r8/Control Surface/src/MIDI_Interfaces/USBMIDI/USBMIDI_ESP32.hpp"

if os.path.isfile(patch) and os.path.isfile(original):
    shutil.copy2(patch, original)
    print("Patch applied succesfully!")
else:
    print("Warning: Could not find specified path - are library and patch installed?")
