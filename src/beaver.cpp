#include "beaver.h"

void drawDetailedBeaver(int x, int y, int frame) {
    uint16_t brown = 0x8A22;
    uint16_t darkBrown = 0x4200;
    uint16_t earColor = 0xD4C8;

    int tailWag = (frame % 2 == 0) ? 0 : 3;
    M5.Lcd.fillRect(x + 55, y + 25 + tailWag, 28, 16, darkBrown);
    M5.Lcd.drawRect(x + 55, y + 25 + tailWag, 28, 16, BLACK);
    M5.Lcd.drawLine(x + 60, y + 27 + tailWag, x + 78, y + 38 + tailWag, BLACK);
    M5.Lcd.drawLine(x + 60, y + 38 + tailWag, x + 78, y + 27 + tailWag, BLACK);

    M5.Lcd.fillRect(x + 12, y + 10, 48, 48, brown);
    M5.Lcd.fillRect(x + 18, y + 2, 36, 16, brown);

    M5.Lcd.fillCircle(x + 18, y + 2, 6, darkBrown);
    M5.Lcd.fillCircle(x + 54, y + 2, 6, darkBrown);
    M5.Lcd.fillCircle(x + 18, y + 2, 3, earColor);
    M5.Lcd.fillCircle(x + 54, y + 2, 3, earColor);

    M5.Lcd.fillRect(x + 20, y + 12, 32, 10, CYAN);
    M5.Lcd.drawRect(x + 20, y + 12, 32, 10, WHITE);
    M5.Lcd.fillRect(x + 26, y + 14, 5, 6, BLACK);
    M5.Lcd.fillRect(x + 41, y + 14, 5, 6, BLACK);

    M5.Lcd.fillCircle(x + 36, y + 28, 4, BLACK);

    M5.Lcd.fillRect(x + 31, y + 34, 10, 10, WHITE);
    M5.Lcd.drawRect(x + 31, y + 34, 10, 10, BLACK);
    M5.Lcd.drawFastVLine(x + 36, y + 34, 10, BLACK);

    M5.Lcd.fillCircle(x + 20, y + 58, 6, darkBrown);
    M5.Lcd.fillCircle(x + 52, y + 58, 6, darkBrown);
}

void showCyberBeaverBoot() {
    M5.Lcd.fillScreen(BLACK);

    for (int i = 0; i <= 100; i += 5) {
        drawDetailedBeaver(75, 10, i / 10);

        M5.Lcd.fillRect(0, 85, 240, 50, BLACK);

        M5.Lcd.setTextColor(CYAN, BLACK);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(65, 88);
        M5.Lcd.println("STICKZERO OS v5.0");

        M5.Lcd.drawRect(30, 102, 180, 10, WHITE);
        M5.Lcd.fillRect(32, 104, (int)(i * 1.76), 6, GREEN);

        M5.Lcd.setTextColor(DARKGREY, BLACK);
        M5.Lcd.setCursor(85, 118);
        M5.Lcd.printf("Loading %d%%", i);

        delay(15);
    }
    delay(150);
}