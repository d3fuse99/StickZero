#include "pc_controller.h"

BleKeyboard bleKeyboard("StickZero OS", "M5Stack", 100);

void initPcController() {
    bleKeyboard.begin();
}

void deinitPcController() {
    bleKeyboard.end();
}

void pcShutdownBle() {
    if (!bleKeyboard.isConnected()) return;
    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('x');
    delay(300);
    bleKeyboard.releaseAll();
    delay(300);
    bleKeyboard.write('u');
    delay(300);
    bleKeyboard.write('u');
}

void pcLockScreen() {
    if (!bleKeyboard.isConnected()) return;
    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('l');
    delay(100);
    bleKeyboard.releaseAll();
}

void pcMuteAudio() {
    if (!bleKeyboard.isConnected()) return;
    bleKeyboard.write(KEY_MEDIA_MUTE);
}