#include <Arduino.h>
#include "ir_learner.h"
#include "ir_controller.h"

const uint16_t kIrRxPin = 33;
IRrecv irrecv(kIrRxPin);
decode_results results;

uint64_t lastCapturedCode = 0;
decode_type_t lastCapturedProtocol = UNKNOWN;
uint16_t lastCapturedBits = 0;

void initIrLearner() {
    irrecv.enableIRIn();
}

bool captureIrSignal() {
    if (irrecv.decode(&results)) {
        lastCapturedCode = results.value;
        lastCapturedProtocol = results.decode_type;
        lastCapturedBits = results.bits;
        irrecv.resume();
        return true;
    }
    return false;
}

void replayCapturedIr() {
    if (lastCapturedCode != 0) {
        irsend.send(lastCapturedProtocol, lastCapturedCode, lastCapturedBits);
    }
}