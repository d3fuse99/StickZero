#include "radar.h"

void startBleRadar() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.fillRect(0, 0, 240, 18, NAVY);
    M5.Lcd.setTextColor(WHITE, NAVY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(5, 5);
    M5.Lcd.print("BLE Radar Scanning...");

    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);

    BLEScanResults foundDevices = pBLEScan->start(2, false);
    int count = foundDevices.getCount();

    M5.Lcd.fillScreen(BLACK);

    M5.Lcd.drawCircle(65, 75, 50, DARKGREEN);
    M5.Lcd.drawCircle(65, 75, 30, DARKGREEN);
    M5.Lcd.drawCircle(65, 75, 12, DARKGREEN);
    M5.Lcd.drawFastHLine(15, 75, 100, DARKGREEN);
    M5.Lcd.drawFastVLine(65, 25, 100, DARKGREEN);

    M5.Lcd.fillRect(0, 0, 240, 18, NAVY);
    M5.Lcd.setTextColor(WHITE, NAVY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(5, 5);
    M5.Lcd.printf("BLE Radar | Found: %d", count);

    int maxDisplay = (count > 5) ? 5 : count;
    for (int i = 0; i < maxDisplay; i++) {
        BLEAdvertisedDevice device = foundDevices.getDevice(i);
        int rssi = device.getRSSI();
        float dist = pow(10.0, (-59.0 - rssi) / (10.0 * 2.0));

        int dotAngle = (i * 72) * 3.14 / 180;
        int dotDist = map(constrain(rssi, -95, -40), -95, -40, 45, 10);
        int dotX = 65 + cos(dotAngle) * dotDist;
        int dotY = 75 + sin(dotAngle) * dotDist;

        M5.Lcd.fillCircle(dotX, dotY, 3, GREEN);

        int yText = 24 + (i * 19);
        M5.Lcd.setTextColor(YELLOW, BLACK);
        M5.Lcd.setCursor(125, yText);
        M5.Lcd.printf("%s", device.getAddress().toString().substr(9).c_str());
        M5.Lcd.setTextColor(CYAN, BLACK);
        M5.Lcd.setCursor(125, yText + 9);
        M5.Lcd.printf("%.1fm (%ddBm)", dist, rssi);
    }

    pBLEScan->clearResults();

    M5.Lcd.fillRect(0, 120, 240, 15, BLACK);
    M5.Lcd.drawFastHLine(0, 119, 240, DARKCYAN);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(10, 123);
    M5.Lcd.println("Press [A] or [B] to Exit");

    delay(300);
    while (true) {
        M5.update();
        if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) break;
        delay(30);
    }
}