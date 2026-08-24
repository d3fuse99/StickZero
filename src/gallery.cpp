#include "gallery.h"
#include "photo.h"

void startGallery() {
    int currentPhoto = 0;

    while (true) {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.pushImage(0, 0, 240, 135, gallery_photos[currentPhoto]);

        delay(250);
        while (true) {
            M5.update();
            if (M5.BtnB.wasPressed()) {
                currentPhoto = (currentPhoto + 1) % TOTAL_PHOTOS;
                break;
            }
            if (M5.BtnA.wasPressed()) {
                return;
            }
            delay(20);
        }
    }
}