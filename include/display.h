#pragma once
#include <M5StickCPlus.h>

enum MenuState {
    MENU_MAIN,
    MENU_PC,
    MENU_TV,
    MENU_AC
};

extern MenuState currentMenu;
extern int selectedIndex;

void initDisplay();
void renderUI();
void showLoadingAnimation(const char* title);
void showStatus(const char* msg, uint16_t bgColor, uint16_t fgColor);
int getBatteryPercent();
void setPowerSave(bool enable);
void startFlashlight();