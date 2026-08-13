#include <M5StickCPlus.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRac.h>
#include <BleKeyboard.h>

const uint16_t kIrLedPin = 9;
IRsend irsend(kIrLedPin, true);
IRac ac(kIrLedPin, true);

BleKeyboard bleKeyboard("StickZero Remote", "M5Stack", 100);

int currentMenuItem = 0;
const int kTotalMenuItems = 6;
const char* menuLabels[kTotalMenuItems] = {
    "1. BLE Screenshot",
    "2. TV Power (Universal)",
    "3. AC Power (Universal)",
    "4. TV Samsung",
    "5. TV LG / Philips",
    "6. TV Sony"
};

int getBatteryPercent() {
    float vbat = M5.Axp.GetBatVoltage();
    if (vbat < 3.2) return 0;
    if (vbat >= 4.1) return 100;
    return (int)((vbat - 3.2) * 100.0 / 0.9);
}

void drawMenu() {
    M5.Lcd.fillScreen(BLACK);

    // Верхняя панель (Header + Battery)
    M5.Lcd.fillRect(0, 0, 240, 20, NAVY);
    M5.Lcd.setTextColor(WHITE, NAVY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(5, 6);
    M5.Lcd.print("StickZero OS v2.0");

    int bat = getBatteryPercent();
    M5.Lcd.setCursor(170, 6);
    M5.Lcd.printf("BAT:%d%%", bat);

    // Отрисовка списка элементов
    for (int i = 0; i < kTotalMenuItems; i++) {
        int yPos = 23 + (i * 16);
        if (i == currentMenuItem) {
            M5.Lcd.fillRect(0, yPos, 240, 16, YELLOW);
            M5.Lcd.setTextColor(BLACK, YELLOW);
            M5.Lcd.setTextSize(2);
            M5.Lcd.setCursor(2, yPos);
            M5.Lcd.printf(">%s", menuLabels[i]);
        } else {
            M5.Lcd.setTextColor(WHITE, BLACK);
            M5.Lcd.setTextSize(1);
            M5.Lcd.setCursor(15, yPos + 4);
            M5.Lcd.println(menuLabels[i]);
        }
    }

    // Нижняя панель подсказок
    M5.Lcd.fillRect(0, 120, 240, 15, DARKGREY);
    M5.Lcd.setTextColor(WHITE, DARKGREY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(15, 124);
    M5.Lcd.println("[B] Scroll / Next     [A] Execute");
}

void showStatus(const char* msg, uint16_t bgColor, uint16_t fgColor) {
    M5.Lcd.fillRect(0, 100, 240, 20, bgColor);
    M5.Lcd.setTextColor(fgColor, bgColor);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(5, 102);
    M5.Lcd.println(msg);
    delay(900);
    drawMenu();
}

void sendTvUniversal() {
    irsend.sendNEC(0xE0E040BF, 32, 3); // Samsung
    delay(30);
    irsend.sendNEC(0x20DF10EF, 32, 3); // LG
    delay(30);
    irsend.sendSony(0xA90, 12, 4);     // Sony 12-bit
    delay(30);
    irsend.sendSony(0xA90, 15, 4);     // Sony 15-bit
    delay(30);
    irsend.sendPanasonic64(0x40040100BCBD); // Panasonic
    delay(30);
    irsend.sendRC5(0x0C, 12, 2);       // Philips RC5
    delay(30);
    irsend.sendRC6(0x1000C, 20, 2);    // Philips RC6
    delay(30);
    irsend.sendNEC(0x02FD48B7, 32, 3); // Toshiba / Sharp
    delay(30);
    irsend.sendNEC(0x00FF00FF, 32, 3); // Universal NEC
    delay(30);
    irsend.sendNEC(0x807618E7, 32, 3); // TCL / Hisense
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

void executeMenuItem() {
    switch (currentMenuItem) {
        case 0:
            if (bleKeyboard.isConnected()) {
                bleKeyboard.write(KEY_PRTSC);
                showStatus("BLE: Shot Sent!", GREEN, BLACK);
            } else {
                showStatus("BLE: Disconnected!", RED, WHITE);
            }
            break;

        case 1:
            showStatus("IR: Sending TV...", CYAN, BLACK);
            sendTvUniversal();
            showStatus("IR: TV Sent!", GREEN, BLACK);
            break;

        case 2:
            showStatus("IR: Sending AC...", CYAN, BLACK);
            sendAcUniversal();
            showStatus("IR: AC Sent!", GREEN, BLACK);
            break;

        case 3:
            showStatus("IR: Samsung TV...", CYAN, BLACK);
            irsend.sendNEC(0xE0E040BF, 32, 3);
            showStatus("IR: Samsung Sent!", GREEN, BLACK);
            break;

        case 4:
            showStatus("IR: LG/Philips TV..", CYAN, BLACK);
            irsend.sendNEC(0x20DF10EF, 32, 3);
            irsend.sendRC5(0x0C, 12, 2);
            showStatus("IR: Sent!", GREEN, BLACK);
            break;

        case 5:
            showStatus("IR: Sony TV...", CYAN, BLACK);
            irsend.sendSony(0xA90, 12, 4);
            irsend.sendSony(0xA90, 15, 4);
            showStatus("IR: Sony Sent!", GREEN, BLACK);
            break;
    }
}

void setup() {
    setCpuFrequencyMhz(80);
    M5.begin();

    M5.Axp.SetLDO2(true);

    irsend.begin();
    bleKeyboard.begin();

    M5.Lcd.setRotation(3);
    drawMenu();
}

void loop() {
    M5.update();

    if (M5.BtnB.wasPressed()) {
        currentMenuItem = (currentMenuItem + 1) % kTotalMenuItems;
        drawMenu();
    }

    if (M5.BtnA.wasPressed()) {
        executeMenuItem();
    }

    delay(20);
}