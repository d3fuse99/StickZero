#include "ir_controller.h"

const uint16_t kIrLedPin = 9;
IRsend irsend(kIrLedPin, true);

void initIrController() {
    irsend.begin();
}

void sendSenseiAcPower() {
    for (int i = 0; i < 5; i++) {
        irsend.sendCOOLIX(0xB24D7B, 24, 2);
        delay(20);
    }
    irsend.sendGree(0x1100000000000000ULL, 64, 3);
}

void sendFullTvBGone() {
    irsend.sendNEC(0x807618E7, 32, 5);
    delay(20);
    irsend.sendNEC(0x00FF00FF, 32, 5);
    delay(20);
    irsend.sendNEC(0xE0E040BF, 32, 5);
    delay(20);
    irsend.sendNEC(0x20DF10EF, 32, 5);
    delay(20);
    irsend.sendSony(0xA90, 12, 5);
    delay(20);
    irsend.sendSony(0xA90, 15, 5);
    delay(20);
    irsend.sendPanasonic64(0x40040100BCBD);
    delay(20);
    irsend.sendRC5(0x0C, 12, 3);
    delay(20);
    irsend.sendRC6(0x1000C, 20, 3);
    delay(20);
    irsend.sendNEC(0x02FD48B7, 32, 5);
    delay(20);
    irsend.sendNEC(0x46A802FD, 32, 5);
    delay(20);
    irsend.sendNEC(0x10EF20DF, 32, 5);
}

void sendAcUniversal() {
    for (int i = 0; i < 3; i++) {
        irsend.sendCOOLIX(0xB24D7B, 24, 2);
        delay(20);
        irsend.sendCOOLIX(0xB27BE0, 24, 2);
        delay(20);
        irsend.sendGree(0x1100000000000000ULL, 64, 2);
        delay(20);
    }
    irsend.sendPanasonic64(0x0220E00400000006ULL, 48, 3);
    delay(20);
    irsend.sendNEC(0x807618E7, 32, 4);
}