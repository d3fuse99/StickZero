import os
import glob
from PIL import Image, ImageOps

supported_exts = ("photo*.jpg", "photo*.png", "photo*.jpeg")
photo_files = []
for ext in supported_exts:
    photo_files.extend(glob.glob(ext))

photo_files = sorted(list(set(photo_files)))

if not photo_files:
    print("Ошибка: Положите файлы с именами photo1.jpg, photo2.jpg в папку проекта!")
    exit(1)

with open("include/photo.h", "w") as f:
    f.write("#pragma once\n#include <Arduino.h>\n\n")
    f.write(f"const int TOTAL_PHOTOS = {len(photo_files)};\n\n")

    for idx, p in enumerate(photo_files):
        img = Image.open(p).convert("RGB")
        img = ImageOps.fit(img, (240, 135), method=Image.Resampling.LANCZOS, centering=(0.5, 0.4))
        pixels = img.load()

        f.write(f"const uint16_t photo_{idx + 1}_data[] PROGMEM = {{\n")
        count = 0
        for y in range(135):
            for x in range(240):
                r, g, b = pixels[x, y]
                c = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
                c_swapped = ((c & 0xFF) << 8) | ((c >> 8) & 0xFF)
                f.write(f"0x{c_swapped:04X}, ")
                count += 1
                if count % 16 == 0:
                    f.write("\n")
        f.write("\n};\n\n")

    f.write("const uint16_t* const gallery_photos[] = {\n")
    for idx in range(len(photo_files)):
        f.write(f"    photo_{idx + 1}_data,\n")
    f.write("};\n")

print(f"Готово! Успешно обработано {len(photo_files)} фото в include/photo.h")