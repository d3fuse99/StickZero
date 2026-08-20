#pragma once
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

extern IRsend irsend;
void initIrController();
void sendFullTvBGone();
void sendSenseiAcPower();
void sendAcUniversal();