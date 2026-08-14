#pragma once
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRac.h>

void initIrController();
void sendTvUniversal();
void sendAcUniversal();