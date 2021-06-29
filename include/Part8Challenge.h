#include "config.h"

namespace Part8Challenge {
void backlightTimerCallback(TimerHandle_t xTimer);
void backlightTask(void* param);
void terminalTask(void* param);
void setup();
void loop();
}
