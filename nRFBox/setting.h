/* ____________________________
   This software is licensed under the MIT License:
   https://github.com/cifertech/nrfbox
   ________________________________________ */
   
#ifndef setting_H
#define setting_H

#include <BLEDevice.h>

// 🔄 GANTI LIBRARY DISPLAY
// ❌ HAPUS INI:
// #include <U8g2lib.h>

// ✅ TAMBAHKAN INI:
#include <TFT_eSPI.h>

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <RF24.h>
#include <vector>
#include <string>
#include <SD.h>
#include <Update.h>
#include <SPI.h>

// Function declarations (TETAP SAMA)
void neopixelSetup();
void neopixelLoop();
void setNeoPixelColour(const std::string& colour);
void flash(int numberOfFlashes, const std::vector<std::string>& colors, const std::string& finalColour);

// 🔄 UPDATE EXTERNAL DISPLAY DECLARATION
// ❌ HAPUS INI:
// extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// ✅ GANTI DENGAN INI:
extern TFT_eSPI tft;

extern Adafruit_NeoPixel pixels;  // Tetap sama

//extern bool neoPixelActive;

bool neoPixelActive = false;

// 🔄 OPTIONAL: Rename variable (bisa tetap 'oledBrightness' kalau mau)
// uint8_t oledBrightness = 100;
uint8_t brightness = 100;  // Lebih generic, bisa buat OLED atau TFT

extern RF24 RadioA;
extern RF24 RadioB;
extern RF24 RadioC;

// Function declarations (TETAP SAMA)
void configureNrf(RF24 &radio);
void setRadiosNeutralState();
void setupRadioA();
void setupRadioB();
void setupRadioC();
void initAllRadios();

// 🔄 NOTE: Fungsi Str() & CenteredStr() pakai u8g2 di implementasinya
// Perlu dimodifikasi di file .cpp-nya (setting.cpp / utils.cpp)
void Str(uint8_t x, uint8_t y, const uint8_t* asciiArray, size_t len);
void CenteredStr(uint8_t screenWidth, uint8_t y, const uint8_t* asciiArray, size_t len, const uint8_t* font);

void utils();
void conf();

namespace Setting {
  void settingSetup();
  void settingLoop();
}

#endif
