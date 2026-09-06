#include <main.cpp_includes.h>

// Variables
uint8_t lastCheckedMuteButton = 1;
bool currentMuteBtnState = false;
lv_obj_t* led_mbtn1;
lv_obj_t* led_mbtn2;
// extern lv_obj_t* led_mbtn3;
// extern lv_obj_t* led_mbtn4;
// extern lv_obj_t* led_mbtn5;
// extern lv_obj_t* led_mbtn6;
lv_obj_t* muteButtonLeds[] = {led_mbtn1, led_mbtn2};
uint16_t last_update{0};

void setup() {
    // Connection protocols initialisation
    Wire.begin(47, 48);
    Serial.begin(9600);
    SPI.begin(12, 13, 11);

    delay(100);

    // Libs initialisation
    Control_Surface.begin();
    lvgl_init();
    ui_init();

    // Additional setup() code
    pinMode(ledm1, OUTPUT);
    digitalWrite(ledm1, 1);

}

void loop() {
    Control_Surface.loop();
    
    // Displaying mute buttons state on screen
    lastCheckedMuteButton = (lastCheckedMuteButton + 1) % 2;
    currentMuteBtnState = muteButtons[lastCheckedMuteButton].getState();

    if (currentMuteBtnState == true) {
        action_led_color_change(muteButtonLeds[lastCheckedMuteButton], 0xFF2A00);
    } else {
        action_led_color_change(muteButtonLeds[lastCheckedMuteButton], 0x909090);
    };


    // Updating screen through LVGL every 10 ms
    if (millis() - last_update >= 10) {
        lv_timer_handler();
        last_update = millis();
    }
}
