#include <M5StickCPlus.h>
#include "display.h"
#include "pc_controller.h"
#include "ir_controller.h"
#include "radar.h"
#include "imu_level.h"
#include "flashlight.h"
#include "game.h"

uint32_t lastActivity = 0;
bool isPowerSave = false;

void handleSelect() {
    if (currentMenu == MENU_MAIN) {
        if (selectedIndex == 0) currentMenu = MENU_PC;
        else if (selectedIndex == 1) {
            showStatus("IR: TV-B-Gone...", CYAN, BLACK);
            sendFullTvBGone();
            showStatus("IR: All TVs Sent!", GREEN, BLACK);
            return;
        } else if (selectedIndex == 2) currentMenu = MENU_AC;
        else if (selectedIndex == 3) {
            startBleRadar();
            renderUI();
            return;
        } else if (selectedIndex == 4) {
            startBubbleLevel();
            renderUI();
            return;
        } else if (selectedIndex == 5) {
            startDirectFlashlight();
            renderUI();
            return;
        } else if (selectedIndex == 6) {
            startSnakeGame();
            renderUI();
            return;
        }
        selectedIndex = 0;
        renderUI();
        return;
    }

    if (currentMenu == MENU_PC) {
        if (selectedIndex == 0) {
            if (bleKeyboard.isConnected()) {
                pcShutdownBle();
                showStatus("BLE: Shutdown Sent!", RED, WHITE);
            } else {
                showStatus("BLE: Disconnected!", RED, WHITE);
            }
        } else if (selectedIndex == 1) {
            if (bleKeyboard.isConnected()) {
                pcLockScreen();
                showStatus("BLE: Screen Locked!", YELLOW, BLACK);
            } else {
                showStatus("BLE: Disconnected!", RED, WHITE);
            }
        } else if (selectedIndex == 2) {
            if (bleKeyboard.isConnected()) {
                pcMuteAudio();
                showStatus("BLE: Mute Toggled!", GREEN, BLACK);
            } else {
                showStatus("BLE: Disconnected!", RED, WHITE);
            }
        } else if (selectedIndex == 3) {
            currentMenu = MENU_MAIN;
            selectedIndex = 0;
            renderUI();
        }
    } else if (currentMenu == MENU_AC) {
        if (selectedIndex == 0) {
            showStatus("IR: Sensei AC...", CYAN, BLACK);
            sendSenseiAcPower();
            showStatus("IR: Sensei Sent!", GREEN, BLACK);
        } else if (selectedIndex == 1) {
            showStatus("IR: Sending AC...", CYAN, BLACK);
            sendAcUniversal();
            showStatus("IR: AC Sent!", GREEN, BLACK);
        } else if (selectedIndex == 2) {
            currentMenu = MENU_MAIN;
            selectedIndex = 0;
            renderUI();
        }
    }
}

void handleNext() {
    int maxCount = 7;
    if (currentMenu == MENU_PC) maxCount = 4;
    else if (currentMenu == MENU_AC) maxCount = 3;

    selectedIndex = (selectedIndex + 1) % maxCount;
    renderUI();
}

void resetSleepTimer() {
    lastActivity = millis();
    if (isPowerSave) {
        setPowerSave(false);
        isPowerSave = false;
        renderUI();
    }
}

void setup() {
    setCpuFrequencyMhz(80);
    M5.begin();

    initDisplay();
    showLoadingAnimation("StickZero OS...");

    initIrController();
    initPcController();

    lastActivity = millis();
    renderUI();
}

void loop() {
    M5.update();

    if (millis() - lastActivity > 10000 && !isPowerSave) {
        setPowerSave(true);
        isPowerSave = true;
    }

    if (M5.BtnB.wasPressed()) {
        if (isPowerSave) {
            resetSleepTimer();
        } else {
            resetSleepTimer();
            handleNext();
        }
    }

    if (M5.BtnA.wasPressed()) {
        if (isPowerSave) {
            resetSleepTimer();
        } else {
            resetSleepTimer();
            handleSelect();
        }
    }

    delay(30);
}