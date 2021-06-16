# ESP32-FreeRTOS-Tutorial

This repository contains the code I wrote by watching the videos and doing the programming "challenges" for [Shawn Hymel's FreeRTOS tutorial series](https://www.youtube.com/playlist?list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz). Shawn's demo code is available in [his repo](https://github.com/ShawnHymel/introduction-to-rtos).

## Intended Workspace and Hardware

- VSCode
- PlatformIO
- ESP32-S2 development board (*I'm using an OEM [ESP32-S2-DevKitM-1 board](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/hw-reference/esp32s2/user-guide-devkitm-1-v1.html) with ESP32-S2-Mini-1 SoC*)

## Project Stricture

The repository only contains one program (*single entry point*) which can be switched using compiler macros between code for the different video series parts (*and associated "challenges"*). The macros are defined and switched in `src/main.cpp`. The actual code for each part is separated into distinct source files.
