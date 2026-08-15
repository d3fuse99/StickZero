#include "wifi_scanner.h"
#include "display.h"

void scanAndDisplayWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.fillRect(0, 0, 240, 20, NAVY);
    M5.Lcd.setTextColor(WHITE, NAVY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(5, 6);
    M5.Lcd.print("Wi-Fi Scanner... Scanning");

    int n = WiFi.scanNetworks();
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.fillRect(0, 0, 240, 20, NAVY);
    M5.Lcd.setTextColor(WHITE, NAVY);
    M5.Lcd.setCursor(5, 6);
    M5.Lcd.printf("Found Networks: %d", n);

    if (n == 0) {
        M5.Lcd.setTextColor(RED, BLACK);
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(10, 50);
        M5.Lcd.println("No Networks!");
    } else {
        M5.Lcd.setTextSize(1);
        int maxDisplay = (n > 5) ? 5 : n;
        for (int i = 0; i < maxDisplay; i++) {
            int y = 25 + (i * 18);
            M5.Lcd.setTextColor(YELLOW, BLACK);
            M5.Lcd.setCursor(5, y);
            M5.Lcd.printf("%d.%s", i + 1, WiFi.SSID(i).substring(0, 14).c_str());
            M5.Lcd.setTextColor(GREEN, BLACK);
            M5.Lcd.setCursor(160, y);
            M5.Lcd.printf("%ddBm C:%d", WiFi.RSSI(i), WiFi.channel(i));
        }
    }

    WiFi.scanDelete();
    WiFi.mode(WIFI_OFF);
    
    delay(3000);
    renderUI();
}