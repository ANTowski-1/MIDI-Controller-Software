#define TFT_CS 14
#define TFT_DC 17
#define TFT_RST 18
#define TFT_MISO 13
#define TFT_MOSI 11
#define TFT_SCK 12

// Display Functions
extern void tft_init();
extern void muteDisplay(int btnNum, bool muted);
extern void fadersDisplay(int fadNum, int fadPos);
extern void fadBtnClear(int fadBtnNum);
extern void lcdClear();
extern void settings();
extern void encDisplay(int encNum, int encVal);
extern void menuTab(int tabNum);

// Config Variables

// Config Functions
extern void loadCfg();
extern void saveCfg();
extern void applyCfg();
extern void saveConfByTab(int tabNum, int chosenOption);
extern void loadCSToTransIntStruct();
