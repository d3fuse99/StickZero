#include "display.h"

MenuState currentMenu = MENU_MAIN;
int selectedIndex = 0;

const char* mainItems[] = {"[1] PC Remote", "[2] TV Remote", "[3] AC Remote", "[4] Flashlight", "[5] Snake Game"};
const char* pcItems[]   = {"1. PC Shutdown (BLE)", "2. PC Lock (BLE)", "3. Mute Audio (BLE)", "< Back"};
const char* tvItems[]   = {"1. TV-B-Gone (KIVI+All)", "2. Samsung / LG", "3. Sony", "< Back"};
const char* acItems[]   = {"1. Sensei AC Power", "2. All ACs Power", "3. Cool Mode 24C", "< Back"};

int getBatteryPercent() {
    float vbat = M5.Axp.GetBatVoltage();
    if (vbat < 3.2) return 0;
    if (vbat >= 4.1) return 100;
    return (int)((vbat - 3.2) * 100.0 / 0.9);
}

void startFlashlight() {
    pinMode(10, OUTPUT);
    digitalWrite(10, LOW);
    M5.Axp.ScreenBreath(100);
    M5.Lcd.fillScreen(WHITE);
    M5.Lcd.setTextColor(BLACK, WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(20, 45);
    M5.Lcd.println("FLASHLIGHT ON");
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(20, 80);
    M5.Lcd.println("Press [A] or [B] to Exit");

    delay(300);
    while (true) {
        M5.update();
        if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) break;
        delay(30);
    }

    digitalWrite(10, HIGH);
    M5.Axp.ScreenBreath(25);
    renderUI();
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
    for (int i = 0; i <= 196; i += 14) {
        M5.Lcd.fillRect(22, 77, i, 12, GREEN);
        delay(20);
    }
}

void renderUI() {
    M5.Lcd.fillScreen(BLACK);

    M5.Lcd.fillRect(0, 0, 240, 20, NAVY);
    M5.Lcd.setTextColor(WHITE, NAVY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(5, 6);

    if (currentMenu == MENU_MAIN) M5.Lcd.print("StickZero OS > Main");
    else if (currentMenu == MENU_PC) M5.Lcd.print("StickZero OS > PC");
    else if (currentMenu == MENU_TV) M5.Lcd.print("StickZero OS > TV");
    else if (currentMenu == MENU_AC) M5.Lcd.print("StickZero OS > AC");

    M5.Lcd.setCursor(170, 6);
    M5.Lcd.printf("BAT:%d%%", getBatteryPercent());

    int count = 0;
    const char** items = NULL;

    if (currentMenu == MENU_MAIN) { count = 5; items = mainItems; }
    else if (currentMenu == MENU_PC) { count = 4; items = pcItems; }
    else if (currentMenu == MENU_TV) { count = 4; items = tvItems; }
    else if (currentMenu == MENU_AC) { count = 4; items = acItems; }

    for (int i = 0; i < count; i++) {
        int y = 22 + (i * 19);
        if (i == selectedIndex) {
            M5.Lcd.fillRect(0, y, 240, 18, DARKCYAN);
            M5.Lcd.drawRect(0, y, 240, 18, CYAN);
            M5.Lcd.setTextColor(YELLOW, DARKCYAN);
            M5.Lcd.setTextSize(2);
            M5.Lcd.setCursor(2, y);
            M5.Lcd.printf(">%s", items[i]);
        } else {
            M5.Lcd.setTextColor(WHITE, BLACK);
            M5.Lcd.setTextSize(1);
            M5.Lcd.setCursor(15, y + 4);
            M5.Lcd.println(items[i]);
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