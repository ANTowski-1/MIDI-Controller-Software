// //#include <LovyanGFX.hpp>

// class LGFX : public lgfx::LGFX_Device 
// {
//     lgfx::Panel_ST7789 _panel_instance;
//     lgfx::Bus_SPI _bus_instance;

//     public:
//         LGFX(void){
//             {auto cfg = _bus_instance.config();
//             cfg.spi_host = SPI3_HOST;
//             cfg.spi_mode = 0;
//             cfg.freq_write = 40000000;
//             cfg.freq_read = 16000000;
//             cfg.spi_3wire = false;
//             cfg.use_lock = true;
//             cfg.pin_miso = 13;
//             cfg.pin_dc = 17;
//             cfg.pin_mosi = 11;
//             cfg.pin_sclk = 12;
//             _bus_instance.config(cfg);
//             _panel_instance.setBus(&_bus_instance);
//             }   
//             {
//             auto cfg = _panel_instance.config();
//             cfg.pin_cs = 14;
//             cfg.pin_rst = 18;
//             cfg.pin_busy = -1;
//             cfg.panel_height = 240;
//             cfg.panel_width = 320;
//             cfg.offset_x = 0;
//             cfg.offset_y = 0;
//             cfg.offset_rotation = 0;
//             cfg.dummy_read_bits = 1;
//             cfg.dummy_read_pixel = 8;
//             cfg.readable = false;
//             cfg.invert = true;
//             cfg.rgb_order = false;
//             cfg.dlen_16bit = false;
//             cfg.bus_shared = true;
            
//             _panel_instance.config(cfg);
//         }
//         setPanel(&_panel_instance);
//     }
// };

// LGFX lcd;
