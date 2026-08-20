#include "display.h"

MenuState currentMenu = MENU_CAROUSEL;
int currentAppIndex = 0;
int selectedIndex = 0;

const int kTotalApps = 7;
const char* appTitles[kTotalApps] = {
    "PC REMOTE",
    "TV-B-GONE",
    "AC-B-GONE",
    "BLE RADAR",
    "IMU LEVEL",
    "FLASHLIGHT",
    "SNAKE GAME"
};

const char* pcItems[] = {
    "1. PC Shutdown (BLE)",
    "2. PC Lock (BLE)",
    "3. Mute Audio (BLE)",
    "< Back"
};

int getBatteryPercent() {
    float vbat = M5.Axp.GetBatVoltage();
    if (vbat < 3.2) return 0;
    if (vbat >= 4.1) return 100;
    return (int)((vbat - 3.2) * 100.0 / 0.9);
}

void setPowerSave(bool enable) {
    if (enable) {
        M5.Axp.SetLDO2(false);
    } else {
        M5.Axp.SetLDO2(true);
        M5.Axp.ScreenBreath(25);
    }
}

void initDisplay() {
    M5.Lcd.setRotation(3);
    M5.Axp.SetLDO2(true);
    M5.Axp.ScreenBreath(25);
}

bool checkPinLock() {
    int pin[4] = {0, 0, 0, 0};
    int targetPin[4] = {1, 3, 3, 7};
    int currentDigit = 0;

    M5.Lcd.fillScreen(BLACK);

    while (currentDigit < 4) {
        M5.Lcd.fillRect(0, 0, 240, 20, NAVY);
        M5.Lcd.setTextColor(WHITE, NAVY);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(5, 6);
        M5.Lcd.print("SECURITY LOCK | Enter PIN");

        M5.Lcd.fillRect(0, 25, 240, 75, BLACK);

        for (int i = 0; i < 4; i++) {
            int x = 45 + (i * 40);
            if (i == currentDigit) {
                M5.Lcd.fillRect(x, 40, 30, 40, DARKCYAN);
                M5.Lcd.drawRect(x, 40, 30, 40, CYAN);
                M5.Lcd.setTextColor(YELLOW, DARKCYAN);
                M5.Lcd.setTextSize(3);
                M5.Lcd.setCursor(x + 7, 48);
                M5.Lcd.print(pin[i]);
            } else if (i < currentDigit) {
                M5.Lcd.fillRect(x, 40, 30, 40, DARKGREY);
                M5.Lcd.drawRect(x, 40, 30, 40, WHITE);
                M5.Lcd.setTextColor(GREEN, DARKGREY);
                M5.Lcd.setTextSize(3);
                M5.Lcd.setCursor(x + 7, 48);
                M5.Lcd.print("*");
            } else {
                M5.Lcd.drawRect(x, 40, 30, 40, DARKGREY);
                M5.Lcd.setTextColor(DARKGREY, BLACK);
                M5.Lcd.setTextSize(3);
                M5.Lcd.setCursor(x + 7, 48);
                M5.Lcd.print("-");
            }
        }

        M5.Lcd.fillRect(0, 115, 240, 20, BLACK);
        M5.Lcd.drawFastHLine(0, 114, 240, DARKCYAN);
        M5.Lcd.setTextColor(WHITE, BLACK);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(15, 120);
        M5.Lcd.println("[B] Change Digit (0-9)  [A] Confirm");

        while (true) {
            M5.update();
            if (M5.BtnB.wasPressed()) {
                pin[currentDigit] = (pin[currentDigit] + 1) % 10;
                break;
            }
            if (M5.BtnA.wasPressed()) {
                currentDigit++;
                break;
            }
            delay(30);
        }
    }

    bool matched = true;
    for (int i = 0; i < 4; i++) {
        if (pin[i] != targetPin[i]) matched = false;
    }

    if (matched) {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setTextColor(GREEN, BLACK);
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(35, 55);
        M5.Lcd.println("ACCESS GRANTED");
        delay(600);
        return true;
    } else {
        M5.Lcd.fillScreen(RED);
        M5.Lcd.setTextColor(WHITE, RED);
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(35, 55);
        M5.Lcd.println("ACCESS DENIED");
        delay(1200);
        return false;
    }
}

void drawAppIcon(int x, int y, int index) {
    switch (index) {
        case 0:
            M5.Lcd.drawRect(x + 8, y + 4, 32, 22, CYAN);
            M5.Lcd.fillRect(x + 12, y + 8, 24, 14, NAVY);
            M5.Lcd.drawFastHLine(x + 4, y + 26, 40, CYAN);
            break;
        case 1:
            M5.Lcd.drawRect(x + 8, y + 6, 32, 22, RED);
            M5.Lcd.drawLine(x + 16, y + 2, x + 24, y + 6, YELLOW);
            M5.Lcd.drawLine(x + 32, y + 2, x + 24, y + 6, YELLOW);
            M5.Lcd.drawCircle(x + 24, y + 17, 4, WHITE);
            break;
        case 2:
            M5.Lcd.drawFastHLine(x + 6, y + 16, 36, CYAN);
            M5.Lcd.drawFastVLine(x + 24, y + 4, 24, CYAN);
            M5.Lcd.drawLine(x + 10, y + 8, x + 38, y + 24, CYAN);
            M5.Lcd.drawLine(x + 10, y + 24, x + 38, y + 8, CYAN);
            break;
        case 3:
            M5.Lcd.drawCircle(x + 24, y + 16, 14, GREEN);
            M5.Lcd.drawCircle(x + 24, y + 16, 7, GREEN);
            M5.Lcd.drawFastHLine(x + 6, y + 16, 36, DARKGREEN);
            M5.Lcd.drawFastVLine(x + 24, y + 2, 28, DARKGREEN);
            break;
        case 4:
            M5.Lcd.drawCircle(x + 24, y + 16, 12, WHITE);
            M5.Lcd.fillCircle(x + 24, y + 16, 4, GREEN);
            M5.Lcd.drawFastHLine(x + 4, y + 16, 40, DARKGREY);
            M5.Lcd.drawFastVLine(x + 24, y + 2, 28, DARKGREY);
            break;
        case 5:
            M5.Lcd.fillCircle(x + 24, y + 12, 8, YELLOW);
            M5.Lcd.fillRect(x + 20, y + 18, 8, 8, DARKGREY);
            M5.Lcd.drawFastHLine(x + 21, y + 27, 6, WHITE);
            break;
        case 6:
            M5.Lcd.fillRect(x + 10, y + 12, 8, 8, GREEN);
            M5.Lcd.fillRect(x + 18, y + 12, 8, 8, GREEN);
            M5.Lcd.fillRect(x + 26, y + 12, 8, 8, GREEN);
            M5.Lcd.fillRect(x + 26, y + 4, 8, 8, GREEN);
            M5.Lcd.fillRect(x + 36, y + 4, 4, 4, RED);
            break;
    }
}

void renderCarouselUI() {
    M5.Lcd.fillScreen(BLACK);

    M5.Lcd.fillRect(0, 0, 240, 18, NAVY);
    M5.Lcd.setTextColor(WHITE, NAVY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(5, 5);
    M5.Lcd.print("StickZero OS v5.0");

    M5.Lcd.setCursor(170, 5);
    M5.Lcd.printf("BAT:%d%%", getBatteryPercent());

    int cardX = 35;
    int cardY = 24;
    int cardW = 170;
    int cardH = 78;

    M5.Lcd.fillRect(cardX, cardY, cardW, cardH, DARKCYAN);
    M5.Lcd.drawRect(cardX, cardY, cardW, cardH, CYAN);

    drawAppIcon(cardX + 61, cardY + 8, currentAppIndex);

    M5.Lcd.setTextColor(YELLOW, DARKCYAN);
    M5.Lcd.setTextSize(2);
    int textLen = strlen(appTitles[currentAppIndex]) * 12;
    M5.Lcd.setCursor(cardX + (cardW - textLen) / 2, cardY + 50);
    M5.Lcd.print(appTitles[currentAppIndex]);

    for (int i = 0; i < kTotalApps; i++) {
        int dotX = 75 + (i * 14);
        int dotY = 108;
        if (i == currentAppIndex) {
            M5.Lcd.fillCircle(dotX, dotY, 4, CYAN);
        } else {
            M5.Lcd.fillCircle(dotX, dotY, 2, DARKGREY);
        }
    }

    M5.Lcd.fillRect(0, 118, 240, 17, BLACK);
    M5.Lcd.drawFastHLine(0, 117, 240, DARKCYAN);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(10, 122);
    M5.Lcd.println("[B] NEXT APP ->       [A] OPEN / RUN");
}

void renderPcMenuUI() {
    M5.Lcd.fillScreen(BLACK);

    M5.Lcd.fillRect(0, 0, 240, 18, NAVY);
    M5.Lcd.setTextColor(WHITE, NAVY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(5, 5);
    M5.Lcd.print("StickZero > PC Remote");

    for (int i = 0; i < 4; i++) {
        int y = 24 + (i * 22);
        if (i == selectedIndex) {
            M5.Lcd.fillRect(0, y, 240, 20, DARKCYAN);
            M5.Lcd.drawRect(0, y, 240, 20, CYAN);
            M5.Lcd.setTextColor(YELLOW, DARKCYAN);
            M5.Lcd.setTextSize(2);
            M5.Lcd.setCursor(5, y + 2);
            M5.Lcd.printf("> %s", pcItems[i]);
        } else {
            M5.Lcd.setTextColor(WHITE, BLACK);
            M5.Lcd.setTextSize(2);
            M5.Lcd.setCursor(15, y + 2);
            M5.Lcd.println(pcItems[i]);
        }
    }

    M5.Lcd.fillRect(0, 118, 240, 17, BLACK);
    M5.Lcd.drawFastHLine(0, 117, 240, DARKCYAN);
    M5.Lcd.setTextColor(DARKGREY, BLACK);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(10, 122);
    M5.Lcd.println("[B] SELECT ITEM       [A] EXECUTE");
}

void showStatus(const char* msg, uint16_t bgColor, uint16_t fgColor) {
    M5.Lcd.fillRect(0, 95, 240, 22, bgColor);
    M5.Lcd.setTextColor(fgColor, bgColor);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 98);
    M5.Lcd.println(msg);
    delay(800);
}