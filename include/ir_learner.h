#pragma once
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

void initIrLearner();
bool captureIrSignal();
void replayCapturedIr();

extern uint64_t lastCapturedCode;
extern decode_type_t lastCapturedProtocol;
extern uint16_t lastCapturedBits;