#include "Part2.h"

// Task: blink an LED
void toggleLED(void* param) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500 /* ms */ / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 /* ms */ / portTICK_PERIOD_MS);
  }
}
void toggleLED2(void* param) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(300 /* ms */ / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(300 /* ms */ / portTICK_PERIOD_MS);
  }
}

void setup2() {
  pinMode(led_pin, OUTPUT);

  // Create the toggleLED task
  xTaskCreatePinnedToCore(  // Analogous to xTaskCreate() in vanilla FreeRTOS
    toggleLED,              // Task function pointer
    "Toggle LED",           // Task name
    1024,                   // Stack size (bytes for ESP-IDF, words in FreeRTOS)
                            // Minimum of 768 bytes for ESP-IDF
    NULL,                   // Optionally passsed parameter
    1,                      // Task priority (0 to configMAX_PRIORITIES - 1)
    NULL,                   // Task handle
    app_cpu                 // Desired core (ESP-IDF only)
  );
  xTaskCreatePinnedToCore(  // Analogous to xTaskCreate() in vanilla FreeRTOS
    toggleLED2,             // Task function pointer
    "Toggle LED2",          // Task name
    1024,                   // Stack size (bytes for ESP-IDF, words in FreeRTOS)
                            // Minimum of 768 bytes for ESP-IDF
    NULL,                   // Optionally passsed parameter
    1,                      // Task priority (0 to configMAX_PRIORITIES - 1)
    NULL,                   // Task handle
    app_cpu                 // Desired core (ESP-IDF only)
  );

  // Since setup() and loop() are run from a separate task, the scheduler is
  // already initialized. In vanilla FreeRTOS, vTaskStartScheduler() would need
  // to be called after setting up all tasks.
  // ESP-IDF initializes setup() and loop() as a priority 1 task in core 1.
}

void loop2() {

}