#include "Part9.h"

static const uint16_t timer_prescaler = 80; // Clock ticks at 80 MHz / 80 = 1 MHz
static const uint64_t timer_max_count = 1e7; // 1e7 max count means that the timer will reset after 1 second

static hw_timer_t *timer = NULL;

// IRAM_ATTR forces the ISR to live in internal RAM and not the flash for faster access
void IRAM_ATTR onTimer() {
  Serial.println("ISR");
  int pin_state = digitalRead(led_pin);
  Serial.println(pin_state);
  digitalWrite(led_pin, !pin_state);
}

void setup9() {
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);

  vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait a moment to start
  Serial.println();
  Serial.println("--- FreeRTOS Hardware Timer Demo ---");

  // Create and start the hardware timer and attach the ISR
  timer = timerBegin(0 /* timer number */, timer_prescaler, true /* countUp? */);
  timerAttachInterrupt(timer, &onTimer, true /* edge */);
  timerAlarmWrite(timer, timer_max_count, true /* autoreload? */);

  // Allow the ISR to trigger
  timerAlarmEnable(timer);
}

void loop9() {
}
