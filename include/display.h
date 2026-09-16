#define TFT_CS 14
#define TFT_DC 17
#define TFT_RST 18
#define TFT_MISO 13
#define TFT_MOSI 11
#define TFT_SCK 12

extern void tft_draw_circle();
extern void tft_write_txt(int num);
extern void tft_init();
extern void muteDisplay(int btnNum, bool muted);
extern void fadersDisplay(int fadNum, int fadPos);
extern void lcdClear();