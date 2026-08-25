#pragma once
#include <Arduino.h>
#include <BleKeyboard.h>

extern BleKeyboard bleKeyboard;
void initPcController();
void deinitPcController();
void pcShutdownBle();
void pcLockScreen();
void pcMuteAudio();