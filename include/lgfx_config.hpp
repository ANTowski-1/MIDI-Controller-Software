// Custom SPI pins for ESP32-S3
#define LGFX_USE_SPI
#define LGFX_SPI_MOSI 11   // Replace with your MOSI pin
#define LGFX_SPI_MISO 13   // Replace with your MISO pin (optional if not used)
#define LGFX_SPI_SCLK 12   // Replace with your SCK pin
#define LGFX_SPI_CS   14   // Replace with your CS pin
#define LGFX_SPI_DC   17   // Replace with your DC pin
#define LGFX_SPI_RST  18   // Replace with your RST pin (optional)

// ST7789 display settings
#define LGFX_AUTODETECT
#define LGFX_ST7789
#define LGFX_WIDTH  320    // Replace with your display width
#define LGFX_HEIGHT 240    // Replace with your display height