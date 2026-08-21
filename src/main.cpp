#include <M5StickCPlus.h>
#include "display.h"
#include "pc_controller.h"
#include "ir_controller.h"
#include "beaver.h"
#include "radar.h"
#include "imu_level.h"
#include "flashlight.h"
#include "game.h"

uint32_t lastActivity = 0;
bool isPowerSave = false;

void handleCarouselSelect() {
    switch (currentAppIndex) {
        case 0:
            initPcController();
            currentMenu = MENU_PC;
            selectedIndex = 0;
            renderPcMenuUI();
            break;
        case 1:
            showStatus("IR: TV-B-Gone...", CYAN, BLACK);
            sendFullTvBGone();
            showStatus("IR: 5x TV Burst Sent!", GREEN, BLACK);
            renderCarouselUI();
            break;
        case 2:
            showStatus("IR: AC-B-Gone...", CYAN, BLACK);
            sendSenseiAcPower();
            sendAcUniversal();
            showStatus("IR: 5x AC Burst Sent!", GREEN, BLACK);
            renderCarouselUI();
            break;
        case 3:
            startBleRadar();
            renderCarouselUI();
            break;
        case 4:
            startBubbleLevel();
            renderCarouselUI();
            break;
        case 5:
            startDirectFlashlight();
            renderCarouselUI();
            break;
        case 6:
            startSnakeGame();
            renderCarouselUI();
            break;
    }
}

void handlePcMenuSelect() {
    if (selectedIndex == 0) {
        if (bleKeyboard.isConnected()) {
            pcShutdownBle();
            showStatus("BLE: Shutdown Sent!", RED, WHITE);
        } else {
            showStatus("BLE: Disconnected!", RED, WHITE);
        }
        renderPcMenuUI();
    } else if (selectedIndex == 1) {
        if (bleKeyboard.isConnected()) {
            pcLockScreen();
            showStatus("BLE: Screen Locked!", YELLOW, BLACK);
        } else {
            showStatus("BLE: Disconnected!", RED, WHITE);
        }
        renderPcMenuUI();
    } else if (selectedIndex == 2) {
        if (bleKeyboard.isConnected()) {
            pcMuteAudio();
            showStatus("BLE: Mute Toggled!", GREEN, BLACK);
        } else {
            showStatus("BLE: Disconnected!", RED, WHITE);
        }
        renderPcMenuUI();
    } else if (selectedIndex == 3) {
        deinitPcController();
        currentMenu = MENU_CAROUSEL;
        renderCarouselUI();
    }
}

void resetSleepTimer() {
    lastActivity = millis();
    if (isPowerSave) {
        setPowerSave(false);
        isPowerSave = false;
        if (currentMenu == MENU_CAROUSEL) renderCarouselUI();
        else renderPcMenuUI();
    }
}

void setup() {
    setCpuFrequencyMhz(80);
    M5.begin();

    initDisplay();
    showCyberBeaverBoot();

    while (!checkPinLock()) {
        delay(100);
    }

    initIrController();

    lastActivity = millis();
    renderCarouselUI();
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
            if (currentMenu == MENU_CAROUSEL) {
                currentAppIndex = (currentAppIndex + 1) % 7;
                renderCarouselUI();
            } else {
                selectedIndex = (selectedIndex + 1) % 4;
                renderPcMenuUI();
            }
        }
    }

    if (M5.BtnA.wasPressed()) {
        if (isPowerSave) {
            resetSleepTimer();
        } else {
            resetSleepTimer();
            if (currentMenu == MENU_CAROUSEL) {
                handleCarouselSelect();
            } else {
                handlePcMenuSelect();
            }
        }
    }

    delay(30);
}