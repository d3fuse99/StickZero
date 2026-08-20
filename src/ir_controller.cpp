#include "ir_controller.h"

const uint16_t kIrLedPin = 9;
IRsend irsend(kIrLedPin, true);

void initIrController() {
    irsend.begin();
}

void sendSenseiAcPower() {
    irsend.sendCOOLIX(0xB24D7B, 24, 3);
    delay(50);
    irsend.sendGree(0x1100000000000000ULL, 64, 2);
}

void sendFullTvBGone() {
    irsend.sendNEC(0x807618E7, 32, 3);
    delay(30);
    irsend.sendNEC(0x00FF00FF, 32, 3);
    delay(30);
    irsend.sendNEC(0xE0E040BF, 32, 3);
    delay(30);
    irsend.sendNEC(0x20DF10EF, 32, 3);
    delay(30);
    irsend.sendSony(0xA90, 12, 4);
    delay(30);
    irsend.sendSony(0xA90, 15, 4);
    delay(30);
    irsend.sendPanasonic64(0x40040100BCBD);
    delay(30);
    irsend.sendRC5(0x0C, 12, 2);
    delay(30);
    irsend.sendRC6(0x1000C, 20, 2);
    delay(30);
    irsend.sendNEC(0x02FD48B7, 32, 3);
    delay(30);
    irsend.sendNEC(0x46A802FD, 32, 3);
    delay(30);
    irsend.sendNEC(0x10EF20DF, 32, 3);
}

void sendAcUniversal() {
    irsend.sendCOOLIX(0xB24D7B, 24, 3);
    delay(40);
    irsend.sendCOOLIX(0xB27BE0, 24, 3);
    delay(40);
    irsend.sendGree(0x1100000000000000ULL, 64, 2);
    delay(40);
    irsend.sendPanasonic64(0x0220E00400000006ULL, 48, 2);
    delay(40);
    irsend.sendNEC(0x807618E7, 32, 3);
}