#pragma once
#include <M5StickCPlus.h>

enum MenuState {
    MENU_CAROUSEL,
    MENU_PC
};

extern MenuState currentMenu;
extern int currentAppIndex;
extern int selectedIndex;

void initDisplay();
void renderCarouselUI();
void renderPcMenuUI();
void showStatus(const char* msg, uint16_t bgColor, uint16_t fgColor);
int getBatteryPercent();
void setPowerSave(bool enable);
bool checkPinLock();