#include "Part6.h"
// Likely required for vanilla FreeRTOS
// #include "semphr.h"

static int shared_var = 0;
static SemaphoreHandle_t mutex;

void incTask(void* param) {
  int item;

  while(1) {
    // Try to take the mutex before entering the critical section
    if(xSemaphoreTake(mutex, 0 /* ticks */) == pdFALSE) {
      // Do something else
      continue;
    }

    // A horrendously overcomplicated and bad variable increment routine
    item = shared_var;
    ++item;
    vTaskDelay(random(100, 500) / portTICK_PERIOD_MS);
    shared_var = item;

    // Return the mutex after the critical section
    xSemaphoreGive(mutex);

    Serial.println(shared_var);
  }
}

void setup6() {
  Serial.begin(115200);

  // Seed the RNG
  randomSeed(analogRead(1));

  vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait a moment to start
  Serial.println();
  Serial.println("--- FreeRTOS Mutex/Race Condition Demo ---");

  // Create mutex
  mutex = xSemaphoreCreateMutex();

  // Start two duplicate increment tasks
  xTaskCreatePinnedToCore(incTask, "Increment Task 1", 1024, NULL, 1, NULL, app_cpu);
  xTaskCreatePinnedToCore(incTask, "Increment Task 2", 1024, NULL, 1, NULL, app_cpu);

  // Delete the setup() and loop() task
  vTaskDelete(NULL);
}

void loop6() {
}