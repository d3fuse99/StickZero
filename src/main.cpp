#include <M5StickCPlus.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <BleKeyboard.h>

const uint16_t kIrLedPin = 9;
// Флаг "true" активирует Active-LOW инверсию сигнала для аппаратной схемы M5StickC PLUS
IRsend irsend(kIrLedPin, true);
BleKeyboard bleKeyboard("M5Stick Remote", "M5Stack", 100);

void sendUniversalIrPower() {
    // Samsung TV Power (NEC 32-bit, 2 повтора)
    irsend.sendNEC(0xE0E040BF, 32, 2);
    delay(40);

    // LG TV Power (NEC 32-bit, 2 повтора)
    irsend.sendNEC(0x20DF10EF, 32, 2);
    delay(40);

    // Sony TV Power (Sony 12-bit, 2 повтора)
    irsend.sendSony(0xA90, 12, 2);
    delay(40);

    // Universal NEC Power 1 (ТВ / Приставки)
    irsend.sendNEC(0x00FF00FF, 32, 2);
    delay(40);

    // Universal NEC Power 2 (TCL / Haier / Hisense)
    irsend.sendNEC(0x807618E7, 32, 2);
}

void setup() {
    setCpuFrequencyMhz(80);
    M5.begin();

    M5.Axp.SetLDO2(true);

    irsend.begin();
    bleKeyboard.begin();

    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(GREEN, BLACK);
    M5.Lcd.setTextSize(2);

    M5.Lcd.setCursor(10, 10);
    M5.Lcd.println("M5Stick Remote");
    M5.Lcd.setCursor(10, 35);
    M5.Lcd.println("A: Screenshot");
    M5.Lcd.setCursor(10, 60);
    M5.Lcd.println("B: TV Power ON/OFF");
}

void loop() {
    M5.update();

    if (M5.BtnA.wasPressed()) {
        M5.Lcd.fillRect(0, 85, 240, 50, BLACK);
        M5.Lcd.setCursor(10, 85);

        if (bleKeyboard.isConnected()) {
            bleKeyboard.write(KEY_PRTSC);
            M5.Lcd.setTextColor(YELLOW, BLACK);
            M5.Lcd.println("BLE: Screenshot! ");
        } else {
            M5.Lcd.setTextColor(RED, BLACK);
            M5.Lcd.println("BLE: Disconnected");
        }
    }

    if (M5.BtnB.wasPressed()) {
        M5.Lcd.fillRect(0, 85, 240, 50, BLACK);
        M5.Lcd.setCursor(10, 85);
        M5.Lcd.setTextColor(CYAN, BLACK);
        M5.Lcd.println("IR: Sending...   ");

        sendUniversalIrPower();

        M5.Lcd.setCursor(10, 85);
        M5.Lcd.setTextColor(GREEN, BLACK);
        M5.Lcd.println("IR: Power Sent!  ");
    }

    delay(20);
}