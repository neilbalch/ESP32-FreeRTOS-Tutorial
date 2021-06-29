#include "Part8.h"
// Likely required for vanilla FreeRTOS
// #include "timers.h"

// Enable software timers
#ifndef configUSE_TIMERS
#define configUSE_TIMERS 1
#endif

namespace Part8 {
static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;

void timerCallback(TimerHandle_t xTimer) {
  if ((uint32_t)pvTimerGetTimerID(xTimer) == 0)
    Serial.println("One-shot timer expired");
  if ((uint32_t)pvTimerGetTimerID(xTimer) == 1)
    Serial.println("Auto-reload timer expired");
}

void setup() {
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);  // Wait a moment to start
  Serial.println();
  Serial.println("--- FreeRTOS Timer Demo ---");

  // Create one-shot timer
  one_shot_timer =
      xTimerCreate("One-shot timer",           // Timer name
                   2000 / portTICK_PERIOD_MS,  // Timer period (ticks)
                   pdFALSE,                    // Auto-reload flag
                   (void*)0,                   // Timer ID
                   timerCallback);             // Callback func

  // Create auto-reload timer
  auto_reload_timer =
      xTimerCreate("Auto-reload timer",        // Timer name
                   1000 / portTICK_PERIOD_MS,  // Timer period (ticks)
                   pdTRUE,                     // Auto-reload flag
                   (void*)1,                   // Timer ID
                   timerCallback);             // Callback func

  // Check to make sure that the timer was created successfully
  if (one_shot_timer == NULL || auto_reload_timer == NULL)
    Serial.println("Couldn't create timers!");
  else {
    // Wait and then start timer
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Starting timers...");
    // Will start the timer immediately or wait until the command queue has a
    // free slot Recall that commands to the timer task go into an execution
    // queue
    xTimerStart(one_shot_timer, portMAX_DELAY);
    xTimerStart(auto_reload_timer, portMAX_DELAY);
  }

  // Delete the setup() and loop() task
  vTaskDelete(NULL);
}

void loop() {}
}  // namespace Part8
