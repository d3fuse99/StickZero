#include "flashlight.h"

void runSolidFlashlight() {
    pinMode(10, OUTPUT);
    digitalWrite(10, LOW);
    M5.Axp.ScreenBreath(100);
    M5.Lcd.fillScreen(WHITE);

    delay(300);
    while (true) {
        M5.update();
        if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) break;
        delay(30);
    }

    digitalWrite(10, HIGH);
    M5.Axp.ScreenBreath(25);
}

void runSosStrobe() {
    pinMode(10, OUTPUT);
    M5.Axp.ScreenBreath(100);

    const int dot = 150;
    const int dash = 450;
    int morse[] = {dot, dot, dot, dash, dash, dash, dot, dot, dot};

    while (true) {
        for (int i = 0; i < 9; i++) {
            M5.Lcd.fillScreen(WHITE);
            digitalWrite(10, LOW);
            delay(morse[i]);

            M5.Lcd.fillScreen(BLACK);
            digitalWrite(10, HIGH);
            delay(150);

            M5.update();
            if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) {
                M5.Axp.ScreenBreath(25);
                return;
            }
        }
        delay(1000);
    }
}

void runPoliceFlasher() {
    M5.Axp.ScreenBreath(100);
    pinMode(10, OUTPUT);

    while (true) {
        for (int i = 0; i < 3; i++) {
            M5.Lcd.fillScreen(RED);
            digitalWrite(10, LOW);
            delay(60);
            M5.Lcd.fillScreen(BLACK);
            digitalWrite(10, HIGH);
            delay(60);
        }

        for (int i = 0; i < 3; i++) {
            M5.Lcd.fillScreen(BLUE);
            delay(60);
            M5.Lcd.fillScreen(BLACK);
            delay(60);
        }

        M5.update();
        if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) break;
    }

    M5.Axp.ScreenBreath(25);
}

void startFlashlightMenu() {
    int subIndex = 0;
    const char* modes[] = {"1. Solid White", "2. SOS Morse Beacon", "3. Police Flasher", "< Back"};

    while (true) {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.fillRect(0, 0, 240, 20, NAVY);
        M5.Lcd.setTextColor(WHITE, NAVY);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(5, 6);
        M5.Lcd.print("StickZero OS > Light Tools");

        for (int i = 0; i < 4; i++) {
            int y = 25 + (i * 22);
            if (i == subIndex) {
                M5.Lcd.fillRect(0, y, 240, 20, DARKCYAN);
                M5.Lcd.drawRect(0, y, 240, 20, CYAN);
                M5.Lcd.setTextColor(YELLOW, DARKCYAN);
                M5.Lcd.setTextSize(2);
                M5.Lcd.setCursor(5, y + 2);
                M5.Lcd.printf("> %s", modes[i]);
            } else {
                M5.Lcd.setTextColor(WHITE, BLACK);
                M5.Lcd.setTextSize(2);
                M5.Lcd.setCursor(15, y + 2);
                M5.Lcd.println(modes[i]);
            }
        }

        while (true) {
            M5.update();
            if (M5.BtnB.wasPressed()) {
                subIndex = (subIndex + 1) % 4;
                break;
            }
            if (M5.BtnA.wasPressed()) {
                if (subIndex == 0) runSolidFlashlight();
                else if (subIndex == 1) runSosStrobe();
                else if (subIndex == 2) runPoliceFlasher();
                else if (subIndex == 3) return;
                break;
            }
            delay(30);
        }
    }
}