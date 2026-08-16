#pragma once
#include <Arduino.h>
#include <BleKeyboard.h>

extern BleKeyboard bleKeyboard;
void initPcController();
void pcShutdownBle();
void pcLockScreen();
void pcMuteAudio();