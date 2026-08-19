#include "display.h"

MenuState currentMenu = MENU_MAIN;
int selectedIndex = 0;

const char* mainItems[] = {"[1] PC Remote", "[2] TV-B-Gone (All)", "[3] AC-B-Gone (All)", "[4] Cyber Bober", "[5] BLE Radar", "[6] IMU Level", "[7] Flashlight", "[8] Snake Game"};
const char* pcItems[]   = {"1. PC Shutdown (BLE)", "2. PC Lock (BLE)", "3. Mute Audio (BLE)", "< Back"};

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

void showLoadingAnimation(const char* title) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(CYAN, BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(15, 35);
    M5.Lcd.println(title);

    M5.Lcd.drawRect(20, 75, 200, 16, WHITE);
    for (int i = 0; i <= 196; i += 28) {
        M5.Lcd.fillRect(22, 77, i, 12, GREEN);
        delay(15);
    }
}

void renderUI() {
    M5.Lcd.fillScreen(BLACK);

    M5.Lcd.fillRect(0, 0, 240, 18, NAVY);
    M5.Lcd.setTextColor(WHITE, NAVY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(5, 5);

    if (currentMenu == MENU_MAIN) M5.Lcd.print("StickZero OS v4.2");
    else if (currentMenu == MENU_PC) M5.Lcd.print("StickZero > PC");

    M5.Lcd.setCursor(170, 5);
    M5.Lcd.printf("BAT:%d%%", getBatteryPercent());

    int count = 0;
    const char** items = NULL;

    if (currentMenu == MENU_MAIN) { count = 8; items = mainItems; }
    else if (currentMenu == MENU_PC) { count = 4; items = pcItems; }

    int startIdx = 0;
    if (count > 5 && selectedIndex >= 5) startIdx = selectedIndex - 4;
    int displayCount = (count - startIdx > 5) ? 5 : (count - startIdx);

    for (int i = 0; i < displayCount; i++) {
        int actualIdx = startIdx + i;
        int y = 21 + (i * 19);
        if (actualIdx == selectedIndex) {
            M5.Lcd.fillRect(0, y, 240, 18, DARKCYAN);
            M5.Lcd.drawRect(0, y, 240, 18, CYAN);
            M5.Lcd.setTextColor(YELLOW, DARKCYAN);
            M5.Lcd.setTextSize(2);
            M5.Lcd.setCursor(2, y);
            M5.Lcd.printf(">%s", items[actualIdx]);
        } else {
            M5.Lcd.setTextColor(WHITE, BLACK);
            M5.Lcd.setTextSize(1);
            M5.Lcd.setCursor(15, y + 4);
            M5.Lcd.println(items[actualIdx]);
        }
    }

    M5.Lcd.fillRect(0, 120, 240, 15, BLACK);
    M5.Lcd.drawFastHLine(0, 119, 240, NAVY);
    M5.Lcd.setTextColor(DARKGREY, BLACK);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(10, 123);
    M5.Lcd.println("[B] SELECT ITEM   |   [A] ENTER / EXEC");
}

void showStatus(const char* msg, uint16_t bgColor, uint16_t fgColor) {
    M5.Lcd.fillRect(0, 100, 240, 20, bgColor);
    M5.Lcd.setTextColor(fgColor, bgColor);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(5, 102);
    M5.Lcd.println(msg);
    delay(900);
    renderUI();
}