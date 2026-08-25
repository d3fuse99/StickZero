#pragma once
#include <M5StickCPlus.h>

enum MenuState {
    MENU_CAROUSEL,
    MENU_PC,
    MENU_DEAUTH,
    MENU_JAMMER
};

extern MenuState currentMenu;
extern int currentAppIndex;
extern int selectedIndex;
extern int selectedDeauthIndex;
extern int selectedJammerIndex;

void initDisplay();
void renderCarouselUI();
void renderPcMenuUI();
void renderDeauthUI();
void renderJammerUI();
void showStatus(const char* msg, uint16_t bgColor, uint16_t fgColor);
int getBatteryPercent();
void setPowerSave(bool enable);
bool checkPinLock();
void drawBatteryIndicator(int x, int y);