/* ____________________________
   This software is licensed under the MIT License:
   https://github.com/cifertech/nrfbox
   ________________________________________ */

#include "setting.h"
#include "icon.h"
#include "config.h"

RF24 RadioA(NRF_CE_PIN_A, NRF_CSN_PIN_A);
RF24 RadioB(NRF_CE_PIN_B, NRF_CSN_PIN_B);
RF24 RadioC(NRF_CE_PIN_C, NRF_CSN_PIN_C);

void setRadiosNeutralState() {
  RadioA.stopListening();
  RadioA.setAutoAck(false);
  RadioA.setRetries(0, 0);
  RadioA.powerDown(); 
  digitalWrite(NRF_CE_PIN_A, LOW); 

  RadioB.stopListening();
  RadioB.setAutoAck(false);
  RadioB.setRetries(0, 0);
  RadioB.powerDown(); 
  digitalWrite(NRF_CE_PIN_B, LOW); 

  RadioC.stopListening();
  RadioC.setAutoAck(false);
  RadioC.setRetries(0, 0);
  RadioC.powerDown(); 
  digitalWrite(NRF_CE_PIN_C, LOW); 
}

void configureNrf(RF24 &radio) {
  radio.begin();
  radio.setAutoAck(false);
  radio.stopListening();
  radio.setRetries(0, 0);
  radio.setPALevel(RF24_PA_MAX, true);
  radio.setDataRate(RF24_2MBPS);
  radio.setCRCLength(RF24_CRC_DISABLED);
}

void setupRadioA() { configureNrf(RadioA); }
void setupRadioB() { configureNrf(RadioB); }
void setupRadioC() { configureNrf(RadioC); }
void initAllRadios() {
  setupRadioA(); setupRadioB(); setupRadioC();
}

// 🔄 FUNGSI Str() - Konversi u8g2 → TFT_eSPI
void Str(uint8_t x, uint8_t y, const uint8_t* asciiArray, size_t len) {
  char buf[64]; 
  for (size_t i = 0; i < len && i < sizeof(buf) - 1; i++) {
    buf[i] = (char)asciiArray[i];
  }
  buf[len] = '\0';
  
  // 🔄 TFT_eSPI: langsung print, nggak perlu drawStr
  tft.setCursor(x, y);
  tft.print(buf);
}

// 🔄 FUNGSI CenteredStr() - Konversi u8g2 → TFT_eSPI
void CenteredStr(uint8_t screenWidth, uint8_t y, const uint8_t* asciiArray, size_t len, const uint8_t* font) {
  char buf[64];
  for (size_t i = 0; i < len && i < sizeof(buf) - 1; i++) {
    buf[i] = (char)asciiArray[i];
  }
  buf[len] = '\0';

  // 🔄 TFT_eSPI: setTextFont() pakai angka (1=GLCD, 2=Font2, 4=Font4, dll)
  // font parameter diabaikan, pakai font default size 2
  tft.setTextFont(2);
  
  // 🔄 Hitung width text untuk centering
  int16_t x1, y1; uint16_t w, h;
  tft.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
  
  tft.setCursor((screenWidth - w) / 2, y);
  tft.print(buf);
}

// 🔄 FUNGSI utils() - Konversi display
void utils() {
  // 🔄 TFT: langsung fillScreen, nggak perlu buffer
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextFont(2);  // Ganti u8g2_font_6x10_tf
  
  // 🔄 Str() sudah di-update, tinggal panggil
  Str(7, 20, Line_A, sizeof(Line_A));   // Y adjusted for 240px height
  Str(12, 50, Line_B, sizeof(Line_B));
  Str(7, 80, Line_C, sizeof(Line_C));
  
  // ❌ HAPUS: u8g2.sendBuffer(); — TFT direct draw
}

// 🔄 FUNGSI conf() - Konversi display + bitmap
void conf() {
  // 🔄 TFT init sequence
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextFont(4);  // Ganti u8g2_font_ncenB14_tr
  
  CenteredStr(240, 40, txt_n, sizeof(txt_n), nullptr);  // screenWidth = 240
  tft.setTextFont(2);
  CenteredStr(240, 70, txt_c, sizeof(txt_c), nullptr);
  CenteredStr(240, 100, txt_v, sizeof(txt_v), nullptr);
  
  // ❌ HAPUS: u8g2.sendBuffer();
  delay(3000);
  
  tft.fillScreen(TFT_BLACK);
  
  // 🔄 Bitmap: drawXBMP format sama, tapi warna perlu specify
  // ⚠️ Pastikan bitmap di icon.h format MSB-first 1-bit
  if (cred != nullptr) {
    tft.drawXBitmap(0, 0, cred, 128, 64, TFT_WHITE, TFT_BLACK);
  }
  
  // ❌ HAPUS: u8g2.sendBuffer();
  delay(250);
}

namespace Setting {

#define EEPROM_ADDRESS_NEOPIXEL 0
#define EEPROM_ADDRESS_BRIGHTNESS 1

int currentOption = 0;
int totalOptions = 3; 

bool buttonUpPressed = false;
bool buttonDownPressed = false;
bool buttonSelectPressed = false;

// 🔄 FUNGSI updateFirmware() - Konversi display messages
void updateFirmware() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextFont(2);
  tft.setCursor(0, 20);
  tft.print("Updating Firmware.");
  // ❌ HAPUS: u8g2.sendBuffer();

  tft.setTextFont(1);
  if (!SD.begin(SD_CS_PIN)) {
    tft.setCursor(0, 45);
    tft.print("[ SD Init Failed ]");
    // ❌ HAPUS: sendBuffer
    delay(2000);
    return;
  }

  if (!SD.exists(FIRMWARE_FILE)) {
    tft.setCursor(0, 45);
    tft.print("[ File Not Found ]");
    delay(2000);
    return;
  }

  File firmware = SD.open(FIRMWARE_FILE);
  if (!firmware) {
    tft.setCursor(0, 45);
    tft.print("[ Open Failed ]");
    delay(2000);
    return;
  }

  if (firmware) {
    tft.setCursor(0, 45);
    tft.print("[ Wait a Moment ]");
  }

  if (!Update.begin(firmware.size())) {
    tft.setCursor(0, 70);
    tft.print("[ Update Init Failed ]");
    firmware.close();
    delay(2000);
    return;
  }

  Update.writeStream(firmware);
  if (Update.end(true)) {
    tft.setCursor(0, 70);
    tft.print("[ Update Success! ]");
    delay(1000);
    ESP.restart();
  } else {
    tft.setCursor(0, 70);
    tft.print("[ Update Failed ]");
    delay(2000);
  }
  firmware.close();
}

// 🔄 FUNGSI toggleOption() - Brightness handling
void toggleOption(int option) {
  if (option == 0) { 
    neoPixelActive = !neoPixelActive;
    EEPROM.write(EEPROM_ADDRESS_NEOPIXEL, neoPixelActive);
    EEPROM.commit();
    Serial.print("NeoPixel is now ");
    Serial.println(neoPixelActive ? "Enabled" : "Disabled");

  } else if (option == 1) { 
    // 🔄 Brightness: OLED contrast → TFT backlight PWM
    uint8_t brightnessPercent = map(brightness, 0, 255, 0, 100); 
    brightnessPercent += 10; 
    if (brightnessPercent > 100) brightnessPercent = 0; 
    brightness = map(brightnessPercent, 0, 100, 0, 255); 

    // 🔄 TFT backlight PWM (jika TFT_BL didefinisikan & pakai PWM)
    #ifdef TFT_BL
    analogWrite(TFT_BL, brightness);  // Perlu pinMode(TFT_BL, OUTPUT) di setup
    #endif
    
    EEPROM.write(EEPROM_ADDRESS_BRIGHTNESS, brightness);
    EEPROM.commit();

    Serial.print("Brightness set to: ");
    Serial.print(brightnessPercent);
    Serial.println("%");

  } else if (option == 2) {
    updateFirmware();
  }
}

void handleButtons() {
  if (!digitalRead(BUTTON_UP_PIN)) {
    if (!buttonUpPressed) {
      buttonUpPressed = true;
      currentOption = (currentOption - 1 + totalOptions) % totalOptions;
    }
  } else { buttonUpPressed = false; }

  if (!digitalRead(BUTTON_DOWN_PIN)) {
    if (!buttonDownPressed) {
      buttonDownPressed = true;
      currentOption = (currentOption + 1) % totalOptions;
    }
  } else { buttonDownPressed = false; }

  if (!digitalRead(BTN_PIN_RIGHT)) {
    if (!buttonSelectPressed) {
      buttonSelectPressed = true;
      toggleOption(currentOption);
    }
  } else { buttonSelectPressed = false; }
}

// 🔄 FUNGSI displayMenu() - Konversi FULL ke TFT 240x240
void displayMenu() {
  // 🔄 TFT: langsung clear, nggak perlu buffer
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Header
  tft.setTextFont(2);  // Ganti u8g2_font_6x10_tf
  tft.setCursor(0, 15);
  tft.print("Settings:");
  tft.drawFastHLine(0, 30, 240, TFT_DARKGREY);  // Separator

  // Menu items (scaled for 240px height)
  tft.setTextFont(1);  // Ganti u8g2_font_5x8_tr
  
  // Option 0: NeoPixel
  if (currentOption == 0) {
    tft.setCursor(0, 50); tft.print("> ");
    tft.setCursor(20, 50); tft.print("NeoPixel: ");
    tft.setCursor(120, 50); tft.print(neoPixelActive ? "Enabled" : "Disabled");
    tft.drawRect(0, 45, 240, 25, TFT_YELLOW);  // Highlight box
  } else {
    tft.setCursor(20, 50); tft.print("NeoPixel: ");
    tft.setCursor(120, 50); tft.print(neoPixelActive ? "Enabled" : "Disabled");
  }

  // Option 1: Brightness
  if (currentOption == 1) {
    tft.setCursor(0, 90); tft.print("> ");
    tft.setCursor(20, 90); tft.print("Brightness: ");
    uint8_t brightnessPercent = map(brightness, 0, 255, 0, 100);
    tft.setCursor(140, 90); tft.print(brightnessPercent); tft.print("%");
    tft.drawRect(0, 85, 240, 25, TFT_YELLOW);
  } else {
    tft.setCursor(20, 90); tft.print("Brightness: ");
    uint8_t brightnessPercent = map(brightness, 0, 255, 0, 100);
    tft.setCursor(140, 90); tft.print(brightnessPercent); tft.print("%");
  }

  // Option 2: Update Firmware
  if (currentOption == 2) {
    tft.setCursor(0, 130); tft.print("> ");
    tft.setCursor(20, 130); tft.print("Update Firmware");
    tft.drawRect(0, 125, 240, 25, TFT_YELLOW);
  } else {
    tft.setCursor(20, 130); tft.print("Update Firmware");
  }

  // Footer hint
  tft.setTextFont(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(10, 220);
  tft.print("UP/DOWN: Navigate | RIGHT: Select");
  
  // ❌ HAPUS: u8g2.sendBuffer(); — TFT direct draw
}

void settingSetup() {
  Serial.begin(115200);
  EEPROM.begin(512);

  neoPixelActive = EEPROM.read(EEPROM_ADDRESS_NEOPIXEL);
  brightness = EEPROM.read(EEPROM_ADDRESS_BRIGHTNESS);  // 🔄 renamed from oledBrightness
  
  if (brightness > 255) brightness = 128; 
  
  // 🔄 TFT backlight init (jika pakai PWM)
  #ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  analogWrite(TFT_BL, brightness);
  #endif

  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BTN_PIN_RIGHT, INPUT_PULLUP);
  
  // 🔄 Initial display
  displayMenu();
}

void settingLoop() {
  handleButtons();
  displayMenu();
}

} // namespace Setting
