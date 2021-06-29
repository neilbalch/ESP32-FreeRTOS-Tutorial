#include <Arduino.h>

// Use only core 1 for tutorial purposes
#ifndef MULTICORE
#if CONFIG_FREERTOS_UNICODE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
#else
// Core definitions (assuming you have dual-core ESP32)
static const BaseType_t pro_cpu = 0;
static const BaseType_t app_cpu = 1;
#endif

static const int led_pin = 9;
static const int adc_pin = 1;
