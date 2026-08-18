#include "flashlight.h"

void startDirectFlashlight() {
    pinMode(10, OUTPUT);
    int currentMode = 0;

    M5.Axp.ScreenBreath(100);

    while (true) {
        if (currentMode == 0) {
            digitalWrite(10, LOW);
            M5.Lcd.fillScreen(WHITE);
            M5.Lcd.setTextColor(BLACK, WHITE);
            M5.Lcd.setTextSize(2);
            M5.Lcd.setCursor(35, 45);
            M5.Lcd.println("WHITE LIGHT");
            M5.Lcd.setTextSize(1);
            M5.Lcd.setCursor(20, 85);
            M5.Lcd.println("[B] Next Mode | [A] Exit");

            while (true) {
                M5.update();
                if (M5.BtnB.wasPressed()) { currentMode = 1; break; }
                if (M5.BtnA.wasPressed()) { currentMode = -1; break; }
                delay(30);
            }
        } else if (currentMode == 1) {
            const int dot = 150;
            const int dash = 450;
            int morse[] = {dot, dot, dot, dash, dash, dash, dot, dot, dot};

            for (int i = 0; i < 9; i++) {
                M5.Lcd.fillScreen(WHITE);
                digitalWrite(10, LOW);
                delay(morse[i]);
                M5.Lcd.fillScreen(BLACK);
                digitalWrite(10, HIGH);
                delay(150);

                M5.update();
                if (M5.BtnB.wasPressed()) { currentMode = 2; break; }
                if (M5.BtnA.wasPressed()) { currentMode = -1; break; }
            }
            delay(500);
        } else if (currentMode == 2) {
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
            if (M5.BtnB.wasPressed()) { currentMode = 0; }
            if (M5.BtnA.wasPressed()) { currentMode = -1; }
        }

        if (currentMode == -1) break;
    }

    digitalWrite(10, HIGH);
    M5.Axp.ScreenBreath(25);
}