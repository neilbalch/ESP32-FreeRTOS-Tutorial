# ESP32-FreeRTOS-Tutorial

This repository contains the code I wrote by watching the videos and doing the programming "challenges" for [Shawn Hymel's FreeRTOS tutorial series](https://www.youtube.com/playlist?list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz). Shawn's demo code is available in [his repo](https://github.com/ShawnHymel/introduction-to-rtos).

## Intended Workspace and Hardware

- VSCode
- PlatformIO
- ESP32-S2 development board (*I'm using an OEM [ESP32-S2-DevKitM-1 board](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/hw-reference/esp32s2/user-guide-devkitm-1-v1.html) with ESP32-S2-Mini-1 SoC*)

## Project Structure

The repository only contains one program (*single entry point*) which can be switched using compiler macros between code for the different video series parts (*and associated "challenges"*). The macros are defined and switched in `src/main.cpp`. The actual code for each part is separated into distinct source files.

### [Video Series Parts](https://www.youtube.com/playlist?list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz)

- [Part 2: Getting Started](https://www.youtube.com/watch?v=JIr7Xm_riRs&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=2)
- [Part 3: Task Scheduling](https://www.youtube.com/watch?v=95yUbClyf3E&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=3)
- [Part 4: Memory Management](https://www.youtube.com/watch?v=Qske3yZRW5I&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=4)
- [Part 5: Queues](https://www.youtube.com/watch?v=pHJ3lxOoWeI&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=5)
- [Part 6: Mutexes](https://www.youtube.com/watch?v=I55auRpbiTs&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=6)
- [Part 7: Semaphores](https://www.youtube.com/watch?v=5JcMtbA9QEE&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=7)
- [Part 8: Software Timers](https://www.youtube.com/watch?v=b1f1Iex0Tso&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=8)
- [Part 9: Hardware Interrupts](https://www.youtube.com/watch?v=qsflCf6ahXU&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=9)
- [Part 10: Deadlock and Starvation](https://www.youtube.com/watch?v=hRsWi4HIENc&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=10)
- [Part 11: Priority Inversion](https://www.youtube.com/watch?v=C2xKhxROmhA&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=11)
- [Part 12: Multicore Systems](https://www.youtube.com/watch?v=LPSHUcH5aQc&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=12)
