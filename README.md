# StickZero
## ⚠️ LEGAL DISCLAIMER

This software is provided for educational purposes only. The authors are not responsible for any misuse, damage, or legal consequences resulting from the use of this software. Users are solely responsible for complying with all applicable laws and regulations in their jurisdiction.
A hardened, modular multi-tool operating firmware for M5StickC PLUS powered by ESP32.

**Project Status:** Production Release Engineered with an 8-app Bruce-style carousel OS, customized RGB565 photogallery engine, hardware-randomized 3x4 anti-spy security matrix, 5x IR overdrive, and deep battery power management.

---

## Core Features

- **Bruce-Style Carousel GUI:** Horizontal card slider featuring custom pixel art icons, graphical battery gauge (AXP192), and dot navigation indicators.
- **Photogallery Engine:** High-speed 16-bit Big-Endian RGB565 image viewer with instant zero-latency rendering and smart per-photo framing.
- **Anti-Spy Keypad Matrix:** 3x4 smartphone-style PIN keypad with true hardware entropy (esp_random()) shuffling digit slot positions on every boot (7-9-2-4 master code). Prevents click-counting and shoulder-surfing.
- **Ultra-Low Power Management:** CPU dynamically scales down to 20 MHz in idle mode, screen backlight power rail (LD02) completely cuts off after 10 seconds of inactivity, and wake-up clicks strictly restore the display without accidental command triggers.
- **On-Demand BLE Radio:** Bluetooth stack initializes strictly within the PC Remote module and terminates upon exiting, eliminating background battery drain.
- **5x IR Signal Overdrive:** High-density 5-cycle packet bursts with 20ms micro-intervals for KIVI, Samsung, LG, Sony, Panasonic, Philips, Toshiba TVs and Sensei/Universal AC units, delivering reliable 4–6m range.
- **Cyber-Beaver Bootloader:** Flipper-Zero-inspired animated pixel-art cyber-beaver with core loading progress.
- **Language-Independent PC Control:** Hardware-level Windows 10/11 shutdown (win+X -> u -> u), screen lock (Win+L), and audio mute (KEY_MEDIA_MUTE) via Bluetooth HID profile.
- **BLE Proximity Radar:** Live circular Bluetooth locator scanning surrounding devices with real-time RSSI signal levels and distance estimation in meters.
- **Digital IMU Bubble Level:** Precision 6-axis surface leveling tool utilizing on-board MPU6886 accelerometer with visual [LEVEL OK] alignment detection.
- **Multi-Mode Strobe & Light:** Instant white flashlight (100% brightness + red LED), SOS Morse code beacon (… --- … ), and rapid Police Flasher with instant non-blocking button exit.
- **Retro Snake Game:** Built-in interactive mini-game with score tracking.
- **Wi-Fi Deauth Killer:** Selectively target and disconnect devices from Wi-Fi networks using deauthentication attacks. Choose from detected networks or attack all simultaneously. Real-time packet counter and status display.
- **BT Jammer:** Three-mode Bluetooth interference (All/Speakers/Headphones) with live device discovery and packet counter. Disrupts Bluetooth connections in the vicinity.

---

## New Modules

### Wi-Fi Deauth Killer
- Scans for available Wi-Fi networks
- Select target network or attack all
- Real-time packet counter
- Start/Stop attack with visual feedback

### BT Jammer
- Three jamming modes: All, Speakers Only, Headphones Only
- Live device discovery
- Packet counter
- Simple Start/Stop interface

---

## Hardware Requirements

- **Target Board:** M5StickC PLUS (ESP32-PICO-D4, ST7789 display 135x240 px, MPU6886 IMU, AXP192 PMU)

---

## Project Structure

- `include/display.h` / `src/display.cpp` — Bruce carousel GUI, high-res pixel icons, Anti-Spy 3x4 PIN matrix, power management
- `include/gallery.h` / `src/gallery.cpp` — 16-bit RGB565 image viewer & gallery controller
- `include/beaver.h` / `src/beaver.cpp` — Cyber-Beaver bootloader & pixel sprite renderer
- `include/pc_controller.h` / `src/pc_controller.cpp` — On-demand Bluetooth HID shortcuts (Shutdown, Lock, Mute)
- `include/ir_controller.h` / `src/ir_controller.cpp` — 5x overdrive TV-B-Gone and AC-B-Gone IR transmission drivers
- `include/radar.h` / `src/radar.cpp` — BLE Proximity Radar & distance estimator
- `include/imu_level.h` / `src/imu_level.cpp` — 6-axis MPU6886 digital bubble level
- `include/flashlight.h` / `src/flashlight.cpp` — Solid flashlight, SOS Morse, and Police Strobe
- `include/game.h` / `src/game.cpp` — Snake game engine
- `include/deauth_controller.h` / `src/deauth_controller.cpp` — Wi-Fi deauthentication attack module
- `include/jammer_controller.h` / `src/jammer_controller.cpp` — Bluetooth jammer module
- `convert.py` — Python tool for automatic image resizing and RGB565 C-array generation
- `src/main.cpp` — Event loop, sleep orchestrator & app router

---

## Build & Installation

1. Open workspace in VS Code with PlatformIO extension.
2. Target board in `platformio.ini`: `board = m5stick-c`
3. Build & Upload: `pio run --target upload`

---

## Security & Access

- **Default Master PIN:** `7 - 9 - 2 - 4`
- Navigate the randomized 3x4 keypad using button B and confirm each digit with button A.

---

## License

Licensed under the MIT License.

---

## ⚠️ Disclaimer

**This firmware is for educational and research purposes only.** The Wi-Fi deauthentication and Bluetooth jamming features are intended for testing your own devices and networks. Unauthorized use to disrupt networks or devices you do not own may be illegal in your jurisdiction. Use responsibly and in compliance with all applicable laws.
