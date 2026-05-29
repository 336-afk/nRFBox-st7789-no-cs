/* ____________________________
   This software is licensed under the MIT License:
   https://github.com/cifertech/nrfbox
   ________________________________________ */

#ifndef CONFIG_H
#define CONFIG_H

// 🔄 RESOLUSI DIUBAH: OLED 128x64 → TFT 240x240
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240

// Push Buttons-specific Pins (TETAP SAMA)
#define BUTTON_UP_PIN       26 
#define BUTTON_SELECT_PIN   33
#define BUTTON_DOWN_PIN     32 
#define BTN_PIN_RIGHT       27
#define BTN_PIN_LEFT        25

// SD Card Slot-specific Pins (TETAP SAMA)
#define SD_CS_PIN 5
#define FIRMWARE_FILE "/firmware.bin"

// nRF24-specific Pins (TETAP SAMA)
#define NRF_CE_PIN_A    5   
#define NRF_CSN_PIN_A   17 
#define NRF_CE_PIN_B    16  
#define NRF_CSN_PIN_B   4  
#define NRF_CE_PIN_C    15  
#define NRF_CSN_PIN_C   2  

// Common dependencies
#include "setting.h"
#include <Arduino.h>

// 🔄 GANTI LIBRARY DISPLAY: U8g2 → TFT_eSPI
// ❌ HAPUS/KOMEN INI:
// #include <U8g2lib.h>
// #include <Wire.h>  // I2C nggak dipakai lagi kalau OLED dicopot

// ✅ TAMBAHKAN INI:
#include <TFT_eSPI.h>
#include <SPI.h>  // SPI wajib untuk TFT

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <Preferences.h>
#include <vector>
#include <string>
#include <SD.h>
#include <Update.h>

// 🔄 GANTI INSTANCE DISPLAY
// ❌ HAPUS INI:
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// ✅ TAMBAHKAN INI:
TFT_eSPI tft = TFT_eSPI();  // Instance TFT (konfigurasi pin di User_Setup.h)

Adafruit_NeoPixel pixels(1, 14, NEO_GRB + NEO_KHZ800);

// BLE-specific dependencies (TETAP SAMA)
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// nRF24-specific dependencies (TETAP SAMA)
#include <nRF24L01.h>
#include <RF24.h>

// WiFi-specific dependencies (TETAP SAMA)
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>
#include <esp_system.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <string>

// ESP-specific configurations (TETAP SAMA)
#include <esp_bt.h>
#include <esp_wifi.h>

// 🔄 UPDATE EXTERNAL DECLARATIONS
// ❌ HAPUS INI:
// extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// ✅ GANTI DENGAN INI:
extern TFT_eSPI tft;

extern Adafruit_NeoPixel pixels;  // Tetap sama

// BLE-related namespaces (TETAP SAMA)
namespace BleJammer {
  void blejammerSetup();
  void blejammerLoop();
}

namespace BleScan {
  void blescanSetup();
  void blescanLoop();
}

namespace SourApple {
  void sourappleSetup();
  void sourappleLoop();
}

namespace Spoofer {
  void spooferSetup();
  void spooferLoop();
}

// nRF24-related namespaces (TETAP SAMA)
namespace Analyzer {
  void analyzerSetup();
  void analyzerLoop();
}

namespace ProtoKill {
  void blackoutSetup();
  void blackoutLoop();
}

namespace Scanner {
  void scannerSetup();
  void scannerLoop();
}

namespace Jammer {
  void jammerSetup();
  void jammerLoop();
}

// WiFi-related namespaces (TETAP SAMA)
namespace WifiScan {
  void wifiscanSetup();
  void wifiscanLoop();
}

namespace Deauther {
  void deautherSetup();
  void deautherLoop();
}

#endif // CONFIG_H
