#include "jammer_controller.h"
#include "display.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

bool jammerActive = false;
int jammerPacketCount = 0;
int jammerMode = 0;
int jammerMenuCursor = 0;
BLEScan* pBLEScan = nullptr;
std::vector<String> deviceList;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        String name = String(advertisedDevice.getName().c_str());
        String addr = String(advertisedDevice.getAddress().toString().c_str());
        if (name.length() > 0) {
            bool found = false;
            for (int i = 0; i < (int)deviceList.size(); i++) {
                if (deviceList[i].indexOf(addr) >= 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                deviceList.push_back(name + " (" + addr + ")");
            }
        }
    }
};

void initJammer() {
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    jammerActive = false;
    jammerPacketCount = 0;
    jammerMode = 0;
    deviceList.clear();
}

void startJammer() {
    jammerActive = true;
    deviceList.clear();
    pBLEScan->start(5, false);
}

void stopJammer() {
    jammerActive = false;
    pBLEScan->stop();
}

void renderJammerUI() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.fillRect(0, 0, 240, 18, NAVY);
    M5.Lcd.setTextColor(WHITE, NAVY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(6, 5);
    M5.Lcd.print("BT Jammer");
    drawBatteryIndicator(212, 1);

    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(6, 24);
    M5.Lcd.printf("Status: %s", jammerActive ? "ACTIVE" : "STOPPED");
    M5.Lcd.setCursor(6, 36);
    M5.Lcd.printf("Packets: %d", jammerPacketCount);
    M5.Lcd.setCursor(6, 48);
    const char* modes[] = {"All", "Speakers", "Headphones"};
    M5.Lcd.printf("Mode: %s", modes[jammerMode]);

    M5.Lcd.setCursor(6, 64);
    M5.Lcd.print("Devices:");
    int y = 78;
    for (int i = 0; i < (int)deviceList.size() && i < 4; i++) {
        M5.Lcd.setTextColor(WHITE, BLACK);
        M5.Lcd.setCursor(6, y);
        M5.Lcd.print(deviceList[i].substring(0, 20).c_str());
        y += 12;
    }

    const char* menuItems[] = {"[A] Start/Stop", "[B] Change Mode", "[A Long] Back"};
    for (int i = 0; i < 3; i++) {
        int py = 100 + (i * 8);
        M5.Lcd.setTextColor(jammerMenuCursor == i ? YELLOW : DARKGREY, BLACK);
        M5.Lcd.setCursor(6, py);
        M5.Lcd.print(menuItems[i]);
    }
}

void handleJammerSelect() {
    if (jammerMenuCursor == 0) {
        if (!jammerActive) {
            startJammer();
        } else {
            stopJammer();
        }
        renderJammerUI();
    } else if (jammerMenuCursor == 1) {
        jammerMode = (jammerMode + 1) % 3;
        renderJammerUI();
    }
}

void loopJammer() {
    if (jammerActive) {
        jammerPacketCount++;
        delay(100);
    }
}