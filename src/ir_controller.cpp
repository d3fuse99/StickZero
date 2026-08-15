#include "ir_controller.h"

const uint16_t kIrLedPin = 9;
IRsend irsend(kIrLedPin, true);
IRac ac(kIrLedPin, true);

void initIrController() {
    irsend.begin();
}

void sendTvUniversal() {
    irsend.sendNEC(0xE0E040BF, 32, 3);
    delay(30);
    irsend.sendNEC(0x20DF10EF, 32, 3);
    delay(30);
    irsend.sendSony(0xA90, 12, 4);
    delay(30);
    irsend.sendPanasonic64(0x40040100BCBD);
    delay(30);
    irsend.sendRC5(0x0C, 12, 2);
    delay(30);
    irsend.sendRC6(0x1000C, 20, 2);
    delay(30);
    irsend.sendNEC(0x02FD48B7, 32, 3);
}

void sendFullTvBGone() {
    irsend.sendNEC(0x807618E7, 32, 3);
    delay(35);
    irsend.sendNEC(0x00FF00FF, 32, 3);
    delay(35);
    irsend.sendNEC(0xE0E040BF, 32, 3);
    delay(35);
    irsend.sendNEC(0x20DF10EF, 32, 3);
    delay(35);
    irsend.sendSony(0xA90, 12, 4);
    delay(35);
    irsend.sendSony(0xA90, 15, 4);
    delay(35);
    irsend.sendPanasonic64(0x40040100BCBD);
    delay(35);
    irsend.sendRC5(0x0C, 12, 2);
    delay(35);
    irsend.sendRC6(0x1000C, 20, 2);
    delay(35);
    irsend.sendNEC(0x02FD48B7, 32, 3);
    delay(35);
    irsend.sendNEC(0x46A802FD, 32, 3);
    delay(35);
    irsend.sendNEC(0x10EF20DF, 32, 3);
}

void sendAcUniversal() {
    ac.next.power = true;
    ac.next.mode = stdAc::opmode_t::kCool;
    ac.next.degrees = 24;

    decode_type_t acProtocols[] = {
        decode_type_t::GREE,
        decode_type_t::COOLIX,
        decode_type_t::MIDEA,
        decode_type_t::DAIKIN,
        decode_type_t::HAIER_AC,
        decode_type_t::LG2,
        decode_type_t::SAMSUNG_AC,
        decode_type_t::PANASONIC_AC,
        decode_type_t::MITSUBISHI_AC
    };

    for (int i = 0; i < 9; i++) {
        ac.next.protocol = acProtocols[i];
        ac.sendAc();
        delay(50);
    }
}