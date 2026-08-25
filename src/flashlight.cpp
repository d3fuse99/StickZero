#include "flashlight.h"

void runSolidFlashlight() {
    pinMode(10, OUTPUT);
    digitalWrite(10, LOW);
    M5.Axp.ScreenBreath(100);
    M5.Lcd.fillScreen(WHITE);

    delay(200);
    while (true) {
        M5.update();
        if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) break;
        delay(20);
    }

    digitalWrite(10, HIGH);
    M5.Axp.ScreenBreath(18);
}

void runSosStrobe() {
    pinMode(10, OUTPUT);
    M5.Axp.ScreenBreath(100);

    const int dot = 120;
    const int dash = 360;
    int morse[] = {dot, dot, dot, dash, dash, dash, dot, dot, dot};

    while (true) {
        for (int i = 0; i < 9; i++) {
            M5.Lcd.fillScreen(WHITE);
            digitalWrite(10, LOW);

            for (int t = 0; t < morse[i]; t += 20) {
                M5.update();
                if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) {
                    digitalWrite(10, HIGH);
                    M5.Axp.ScreenBreath(18);
                    return;
                }
                delay(20);
            }

            M5.Lcd.fillScreen(BLACK);
            digitalWrite(10, HIGH);

            for (int t = 0; t < 120; t += 20) {
                M5.update();
                if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) {
                    M5.Axp.ScreenBreath(18);
                    return;
                }
                delay(20);
            }
        }
    }
}

void runPoliceFlasher() {
    M5.Axp.ScreenBreath(100);
    pinMode(10, OUTPUT);

    while (true) {
        for (int i = 0; i < 3; i++) {
            M5.Lcd.fillScreen(RED);
            digitalWrite(10, LOW);
            for (int t = 0; t < 50; t += 10) {
                M5.update();
                if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) goto exit_police;
                delay(10);
            }
            M5.Lcd.fillScreen(BLACK);
            digitalWrite(10, HIGH);
            for (int t = 0; t < 50; t += 10) {
                M5.update();
                if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) goto exit_police;
                delay(10);
            }
        }

        for (int i = 0; i < 3; i++) {
            M5.Lcd.fillScreen(BLUE);
            for (int t = 0; t < 50; t += 10) {
                M5.update();
                if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) goto exit_police;
                delay(10);
            }
            M5.Lcd.fillScreen(BLACK);
            for (int t = 0; t < 50; t += 10) {
                M5.update();
                if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) goto exit_police;
                delay(10);
            }
        }
    }

exit_police:
    digitalWrite(10, HIGH);
    M5.Axp.ScreenBreath(18);
}

void startDirectFlashlight() {
    int currentMode = 0;

    while (true) {
        if (currentMode == 0) {
            runSolidFlashlight();
            currentMode = 1;
        } else if (currentMode == 1) {
            runSosStrobe();
            currentMode = 2;
        } else if (currentMode == 2) {
            runPoliceFlasher();
            break;
        }
    }
}