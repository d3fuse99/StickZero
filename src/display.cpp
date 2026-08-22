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
        setCpuFrequencyMhz(20);
        M5.Axp.SetLDO2(false);
    } else {
        setCpuFrequencyMhz(80);
        M5.Axp.SetLDO2(true);
        M5.Axp.ScreenBreath(18);
    }
}

void initDisplay() {
    M5.Lcd.setRotation(3);
    M5.Axp.SetLDO2(true);
    M5.Axp.ScreenBreath(18);
}

void shuffleArray(int* arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = esp_random() % (i + 1);
        int t = arr[i];
        arr[i] = arr[j];
        arr[j] = t;
    }
}

bool checkPinLock() {
    int targetPin[4] = {7, 9, 2, 4};
    int enteredPin[4] = {0, 0, 0, 0};
    int gridNumbers[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    randomSeed(analogRead(36) + esp_random());
    shuffleArray(gridNumbers, 10);

    int cursorIndex = 0;
    int step = 0;

    int gridX[10] = {105, 150, 195, 105, 150, 195, 105, 150, 195, 150};
    int gridY[10] = {  8,   8,   8,  38,  38,  38,  68,  68,  68,  98};

    while (step < 4) {
        M5.Lcd.fillScreen(BLACK);

        M5.Lcd.fillRect(0, 0, 95, 135, NAVY);
        M5.Lcd.setTextColor(WHITE, NAVY);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(6, 12);
        M5.Lcd.println("SECURITY");
        M5.Lcd.setCursor(6, 24);
        M5.Lcd.println("KEYPAD");

        for (int i = 0; i < 4; i++) {
            int dotX = 14 + (i * 18);
            if (i < step) {
                M5.Lcd.fillCircle(dotX, 55, 5, GREEN);
            } else {
                M5.Lcd.drawCircle(dotX, 55, 5, WHITE);
            }
        }

        M5.Lcd.setTextColor(CYAN, NAVY);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(6, 80);
        M5.Lcd.println("[B] Move");
        M5.Lcd.setCursor(6, 95);
        M5.Lcd.println("[A] Enter");
        M5.Lcd.setCursor(6, 115);
        M5.Lcd.printf("BAT:%d%%", getBatteryPercent());

        for (int i = 0; i < 10; i++) {
            int x = gridX[i];
            int y = gridY[i];
            int val = gridNumbers[i];

            if (i == cursorIndex) {
                M5.Lcd.fillRoundRect(x, y, 38, 26, 6, YELLOW);
                M5.Lcd.setTextColor(BLACK, YELLOW);
                M5.Lcd.setTextSize(2);
                M5.Lcd.setCursor(x + 13, y + 5);
                M5.Lcd.print(val);
            } else {
                M5.Lcd.fillRoundRect(x, y, 38, 26, 6, DARKCYAN);
                M5.Lcd.drawRoundRect(x, y, 38, 26, 6, CYAN);
                M5.Lcd.setTextColor(WHITE, DARKCYAN);
                M5.Lcd.setTextSize(2);
                M5.Lcd.setCursor(x + 13, y + 5);
                M5.Lcd.print(val);
            }
        }

        while (true) {
            M5.update();
            if (M5.BtnB.wasPressed()) {
                cursorIndex = (cursorIndex + 1) % 10;
                break;
            }
            if (M5.BtnA.wasPressed()) {
                enteredPin[step] = gridNumbers[cursorIndex];
                step++;
                break;
            }
            delay(20);
        }
    }

    bool matched = true;
    for (int i = 0; i < 4; i++) {
        if (enteredPin[i] != targetPin[i]) matched = false;
    }

    if (matched) {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setTextColor(GREEN, BLACK);
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(35, 55);
        M5.Lcd.println("ACCESS GRANTED");
        delay(400);
        return true;
    } else {
        M5.Lcd.fillScreen(RED);
        M5.Lcd.setTextColor(WHITE, RED);
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(35, 55);
        M5.Lcd.println("ACCESS DENIED");
        delay(1000);
        return false;
    }
}

void drawAppIcon(int x, int y, int index) {
    switch (index) {
        case 0:
            M5.Lcd.drawRect(x + 6, y + 2, 36, 22, CYAN);
            M5.Lcd.fillRect(x + 9, y + 5, 30, 16, BLACK);
            M5.Lcd.drawFastHLine(x + 12, y + 9, 14, GREEN);
            M5.Lcd.drawFastHLine(x + 12, y + 13, 20, CYAN);
            M5.Lcd.drawFastHLine(x + 12, y + 17, 10, YELLOW);
            M5.Lcd.fillRect(x + 2, y + 24, 44, 4, DARKGREY);
            M5.Lcd.drawRect(x + 2, y + 24, 44, 4, WHITE);
            M5.Lcd.drawFastHLine(x + 18, y + 25, 12, CYAN);
            break;
        case 1:
            M5.Lcd.drawRect(x + 4, y + 6, 40, 24, 0x8A22);
            M5.Lcd.fillRect(x + 7, y + 9, 26, 18, BLACK);
            M5.Lcd.drawCircle(x + 20, y + 18, 5, CYAN);
            M5.Lcd.drawCircle(x + 38, y + 12, 2, WHITE);
            M5.Lcd.drawCircle(x + 38, y + 20, 2, WHITE);
            M5.Lcd.drawLine(x + 14, y + 1, x + 24, y + 6, YELLOW);
            M5.Lcd.drawLine(x + 34, y + 1, x + 24, y + 6, YELLOW);
            M5.Lcd.drawCircle(x + 14, y + 1, 1, RED);
            M5.Lcd.drawCircle(x + 34, y + 1, 1, RED);
            break;
        case 2:
            M5.Lcd.drawRect(x + 2, y + 6, 44, 18, WHITE);
            M5.Lcd.fillRect(x + 5, y + 9, 38, 12, DARKGREY);
            M5.Lcd.drawFastHLine(x + 6, y + 19, 36, CYAN);
            M5.Lcd.drawFastHLine(x + 8, y + 22, 32, CYAN);
            M5.Lcd.fillCircle(x + 40, y + 11, 2, GREEN);
            M5.Lcd.drawFastHLine(x + 16, y + 28, 16, 0x5DFF);
            M5.Lcd.drawFastHLine(x + 12, y + 32, 24, 0x5DFF);
            break;
        case 3:
            M5.Lcd.drawCircle(x + 24, y + 17, 16, DARKGREEN);
            M5.Lcd.drawCircle(x + 24, y + 17, 10, GREEN);
            M5.Lcd.drawCircle(x + 24, y + 17, 4, GREEN);
            M5.Lcd.drawFastHLine(x + 4, y + 17, 40, DARKGREEN);
            M5.Lcd.drawFastVLine(x + 24, y + 1, 32, DARKGREEN);
            M5.Lcd.drawLine(x + 24, y + 17, x + 35, y + 8, GREEN);
            M5.Lcd.fillCircle(x + 32, y + 11, 2, RED);
            break;
        case 4:
            M5.Lcd.drawCircle(x + 24, y + 17, 16, DARKGREY);
            M5.Lcd.drawCircle(x + 24, y + 17, 8, WHITE);
            M5.Lcd.drawFastHLine(x + 2, y + 17, 44, DARKGREY);
            M5.Lcd.drawFastVLine(x + 24, y + 1, 32, DARKGREY);
            M5.Lcd.fillCircle(x + 24, y + 17, 4, GREEN);
            M5.Lcd.drawRect(x + 6, y + 2, 36, 30, DARKCYAN);
            break;
        case 5:
            M5.Lcd.fillRect(x + 8, y + 14, 20, 8, DARKGREY);
            M5.Lcd.drawRect(x + 8, y + 14, 20, 8, WHITE);
            M5.Lcd.fillRect(x + 28, y + 10, 8, 16, DARKCYAN);
            M5.Lcd.drawRect(x + 28, y + 10, 8, 16, CYAN);
            M5.Lcd.drawLine(x + 36, y + 10, x + 46, y + 4, YELLOW);
            M5.Lcd.drawLine(x + 36, y + 18, x + 48, y + 18, YELLOW);
            M5.Lcd.drawLine(x + 36, y + 26, x + 46, y + 32, YELLOW);
            break;
        case 6:
            M5.Lcd.fillRect(x + 4, y + 4, 40, 26, 0x4208);
            M5.Lcd.drawRect(x + 4, y + 4, 40, 26, WHITE);
            M5.Lcd.fillRect(x + 8, y + 8, 20, 14, BLACK);
            M5.Lcd.fillRect(x + 10, y + 14, 4, 4, GREEN);
            M5.Lcd.fillRect(x + 14, y + 14, 4, 4, GREEN);
            M5.Lcd.fillRect(x + 18, y + 14, 4, 4, GREEN);
            M5.Lcd.fillRect(x + 22, y + 10, 4, 4, RED);
            M5.Lcd.fillCircle(x + 35, y + 12, 3, RED);
            M5.Lcd.fillCircle(x + 39, y + 18, 3, YELLOW);
            break;
    }
}

void renderCarouselUI() {
    M5.Lcd.fillScreen(BLACK);

    M5.Lcd.fillRect(0, 0, 240, 18, NAVY);
    M5.Lcd.setTextColor(WHITE, NAVY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(5, 5);
    M5.Lcd.print("StickZero OS v5.2");

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