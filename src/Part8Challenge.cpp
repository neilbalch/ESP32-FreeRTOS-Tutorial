#include "Part8Challenge.h"
// Likely required for vanilla FreeRTOS
// #include "timers.h"

// Enable software timers
#ifndef configUSE_TIMERS
#define configUSE_TIMERS 1
#endif

static TimerHandle_t backlight_timer = NULL;
static bool backlight_on = false;

void backlightTimerCallback(TimerHandle_t xTimer) {
  backlight_on = false;
  // Serial.println("Backlight set off");
}

void backlightTask(void* param) {
  while(1) {
    digitalWrite(led_pin, backlight_on);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void terminalTask(void* param) {
  while(1) {
    if(Serial.available()) {
      char curr = Serial.read();
      Serial.print(curr);
      xTimerStart(backlight_timer, portMAX_DELAY);
      backlight_on = true;
      // Serial.println("Backlight set on");

      // Yield to the backlight task
      vTaskDelay(50 / portTICK_PERIOD_MS);
    }
  }
}

void setup8challenge() {
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);

  vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait a moment to start
  Serial.println();
  Serial.println("--- FreeRTOS Timer Demo ---");

  // Create one-shot timer
  backlight_timer = xTimerCreate("Backlight timer",          // Timer name
                                 5000 / portTICK_PERIOD_MS,  // Timer period (ticks)
                                 pdFALSE,                    // Auto-reload flag
                                 (void*)0,                   // Timer ID
                                 backlightTimerCallback);    // Callback func


  // Check to make sure that the timer was created successfully
  if(backlight_timer == NULL) Serial.println("Couldn't create timer!");

  xTaskCreate(backlightTask, "Backlight Task", 1024, NULL, 1, NULL);
  xTaskCreate(terminalTask, "Terminal Task", 1024, NULL, 1, NULL);

  // Delete the setup() and loop() task
  vTaskDelete(NULL);
}

void loop8challenge() {
}
