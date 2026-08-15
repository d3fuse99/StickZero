#pragma once
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRac.h>

extern IRsend irsend;
void initIrController();
void sendTvUniversal();
void sendAcUniversal();
void sendFullTvBGone();