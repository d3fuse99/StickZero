#include "imu_level.h"

void startBubbleLevel() {
    M5.IMU.Init();
    float ax = 0, ay = 0, az = 0;

    while (true) {
        M5.update();
        if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) break;

        M5.IMU.getAccelData(&ax, &ay, &az);

        int bubbleX = 120 + (int)(ay * 90);
        int bubbleY = 70 - (int)(ax * 70);

        if (bubbleX < 25) bubbleX = 25;
        if (bubbleX > 215) bubbleX = 215;
        if (bubbleY < 25) bubbleY = 25;
        if (bubbleY > 115) bubbleY = 115;

        bool isLevel = (abs(ax) < 0.04 && abs(ay) < 0.04);

        M5.Lcd.fillScreen(BLACK);

        M5.Lcd.drawCircle(120, 70, 48, DARKGREY);
        M5.Lcd.drawCircle(120, 70, 16, isLevel ? GREEN : WHITE);
        M5.Lcd.drawFastHLine(20, 70, 200, DARKGREY);
        M5.Lcd.drawFastVLine(120, 20, 100, DARKGREY);

        M5.Lcd.fillCircle(bubbleX, bubbleY, 9, isLevel ? GREEN : YELLOW);

        M5.Lcd.fillRect(0, 0, 240, 18, NAVY);
        M5.Lcd.setTextColor(WHITE, NAVY);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(5, 5);
        M5.Lcd.printf("IMU LEVEL | X:%.1f Y:%.1f %s", ax * 90.0, ay * 90.0, isLevel ? "[LEVEL OK]" : "");

        delay(40);
    }
}