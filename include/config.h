#include <Arduino.h>

// Use only core 1 for tutorial purposes
#if CONFIG_FREERTOS_UNICODE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const int led_pin = 9;
