#include <M5StickCPlus.h>
#include "display.h"
#include "pc_controller.h"
#include "ir_controller.h"
#include "beaver.h"
#include "radar.h"
#include "imu_level.h"
#include "flashlight.h"
#include "game.h"
#include "gallery.h"
#include "deauth_controller.h"
#include "jammer_controller.h"

void handleDeauthMenuSelect();
void handleJammerMenuSelect();
bool deauthRunning = false;
bool jammerRunning = false;

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
        case 7:
            startGallery();
            renderCarouselUI();
            break;
        case 8:
            currentMenu = MENU_DEAUTH;
            selectedDeauthIndex = 0;
            renderDeauthUI();
            break;
        case 9:
            currentMenu = MENU_JAMMER;
            selectedJammerIndex = 0;
            renderJammerUI();
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

void handleDeauthMenuSelect() {
    if (selectedDeauthIndex == 0) {
        if (!deauthRunning) {
            initDeauth();
            startDeauthAttack();
            deauthRunning = true;
            showStatus("Deauth: STARTED", GREEN, BLACK);
        } else {
            showStatus("Already running", YELLOW, BLACK);
        }
        renderDeauthUI();
    } else if (selectedDeauthIndex == 1) {
        if (deauthRunning) {
            stopDeauthAttack();
            deauthRunning = false;
            showStatus("Deauth: STOPPED", RED, WHITE);
        } else {
            showStatus("Not running", YELLOW, BLACK);
        }
        renderDeauthUI();
    } else if (selectedDeauthIndex == 2) {
        if (deauthRunning) {
            stopDeauthAttack();
            deauthRunning = false;
        }
        currentMenu = MENU_CAROUSEL;
        renderCarouselUI();
    }
}

void handleJammerMenuSelect() {
    if (selectedJammerIndex == 0) {
        if (!jammerRunning) {
            initJammer();
            startJammer();
            jammerRunning = true;
            showStatus("Jammer: STARTED", GREEN, BLACK);
        } else {
            showStatus("Already running", YELLOW, BLACK);
        }
        renderJammerUI();
    } else if (selectedJammerIndex == 1) {
        if (jammerRunning) {
            stopJammer();
            jammerRunning = false;
            showStatus("Jammer: STOPPED", RED, WHITE);
        } else {
            showStatus("Not running", YELLOW, BLACK);
        }
        renderJammerUI();
    } else if (selectedJammerIndex == 2) {
        if (jammerRunning) {
            stopJammer();
            jammerRunning = false;
        }
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
        else if (currentMenu == MENU_PC) renderPcMenuUI();
        else if (currentMenu == MENU_DEAUTH) renderDeauthUI();
        else if (currentMenu == MENU_JAMMER) renderJammerUI();
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

    if (deauthRunning) {
        loopDeauth();
    }
    if (jammerRunning) {
        loopJammer();
    }

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
                currentAppIndex = (currentAppIndex + 1) % 10;
                renderCarouselUI();
            } else if (currentMenu == MENU_PC) {
                selectedIndex = (selectedIndex + 1) % 4;
                renderPcMenuUI();
            } else if (currentMenu == MENU_DEAUTH) {
                selectedDeauthIndex = (selectedDeauthIndex + 1) % 3;
                renderDeauthUI();
            } else if (currentMenu == MENU_JAMMER) {
                selectedJammerIndex = (selectedJammerIndex + 1) % 3;
                renderJammerUI();
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
            } else if (currentMenu == MENU_PC) {
                handlePcMenuSelect();
            } else if (currentMenu == MENU_DEAUTH) {
                handleDeauthMenuSelect();
            } else if (currentMenu == MENU_JAMMER) {
                handleJammerMenuSelect();
            }
        }
    }

    delay(20);
}