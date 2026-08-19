#include "beaver.h"

enum BeaverMood {
    BOBER_IDLE,
    BOBER_HAPPY,
    BOBER_CHOP,
    BOBER_SLEEP
};

static int boberLevel = 1;
static int logsChopped = 0;

void drawPixelBeaver(int x, int y, int frame, BeaverMood mood) {
    uint16_t brown = 0x8A22;
    uint16_t darkBrown = 0x4A00;
    uint16_t earColor = 0xD4C8;

    M5.Lcd.fillRect(x + 10, y + 10, 40, 45, brown);
    M5.Lcd.fillRect(x + 15, y + 4, 30, 15, brown);

    M5.Lcd.fillCircle(x + 16, y + 4, 5, darkBrown);
    M5.Lcd.fillCircle(x + 44, y + 4, 5, darkBrown);
    M5.Lcd.fillCircle(x + 16, y + 4, 3, earColor);
    M5.Lcd.fillCircle(x + 44, y + 4, 3, earColor);

    int tailWag = (frame % 2 == 0) ? 0 : 4;
    M5.Lcd.fillRect(x + 48, y + 35 + tailWag, 25, 14, darkBrown);
    M5.Lcd.drawRect(x + 48, y + 35 + tailWag, 25, 14, BLACK);

    if (mood == BOBER_SLEEP) {
        M5.Lcd.drawFastHLine(x + 22, y + 16, 6, BLACK);
        M5.Lcd.drawFastHLine(x + 32, y + 16, 6, BLACK);
        M5.Lcd.setTextColor(CYAN, BLACK);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(x + 55, y - 5);
        M5.Lcd.print("Zzz...");
    } else {
        M5.Lcd.fillRect(x + 20, y + 12, 8, 8, CYAN);
        M5.Lcd.fillRect(x + 32, y + 12, 8, 8, CYAN);
        M5.Lcd.fillRect(x + 23, y + 14, 3, 4, BLACK);
        M5.Lcd.fillRect(x + 35, y + 14, 3, 4, BLACK);
    }

    M5.Lcd.fillCircle(x + 30, y + 25, 4, BLACK);
    M5.Lcd.fillRect(x + 26, y + 30, 8, 8, WHITE);
    M5.Lcd.drawFastVLine(x + 30, y + 30, 8, BLACK);

    M5.Lcd.fillCircle(x + 16, y + 54, 5, darkBrown);
    M5.Lcd.fillCircle(x + 44, y + 54, 5, darkBrown);

    if (mood == BOBER_CHOP) {
        M5.Lcd.fillRect(x - 15, y + 25, 12, 28, 0xC400);
        M5.Lcd.fillRect(x - 20, y + 20, 22, 10, 0x8410);
    }
}

void startBeaverCompanion() {
    int frame = 0;
    BeaverMood mood = BOBER_IDLE;
    uint32_t lastActionTime = millis();
    float ax = 0, ay = 0, az = 0;

    while (true) {
        M5.update();
        M5.IMU.getAccelData(&ax, &ay, &az);

        if (M5.BtnA.wasPressed()) {
            logsChopped++;
            if (logsChopped % 5 == 0) boberLevel++;
            mood = BOBER_CHOP;
            lastActionTime = millis();
        }

        if (M5.BtnB.wasPressed()) break;

        if (millis() - lastActionTime > 2000) {
            if (abs(ax) > 0.6 || abs(ay) > 0.6) mood = BOBER_HAPPY;
            else mood = BOBER_IDLE;
        }

        M5.Lcd.fillScreen(BLACK);

        M5.Lcd.fillRect(0, 0, 240, 18, NAVY);
        M5.Lcd.setTextColor(WHITE, NAVY);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(5, 5);
        M5.Lcd.printf("CYBER BOBER | LVL: %d | LOGS: %d", boberLevel, logsChopped);

        drawPixelBeaver(85, 26, frame, mood);

        M5.Lcd.fillRect(0, 102, 240, 33, BLACK);
        M5.Lcd.drawFastHLine(0, 101, 240, DARKCYAN);
        M5.Lcd.setTextColor(GREEN, BLACK);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(10, 106);

        if (mood == BOBER_CHOP) M5.Lcd.println("Status: CHOPPING WOOD! +1 Log");
        else if (mood == BOBER_HAPPY) M5.Lcd.println("Status: Wheee! Beaver is tilted!");
        else M5.Lcd.println("Status: Cyber Beaver is guarding OS");

        M5.Lcd.setTextColor(DARKGREY, BLACK);
        M5.Lcd.setCursor(10, 118);
        M5.Lcd.println("[A] Feed / Chop Wood    [B] Exit");

        frame++;
        delay(100);
    }
}