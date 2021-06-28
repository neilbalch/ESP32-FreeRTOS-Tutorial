#define MULTICORE
#include "Part12.h"

// Core definitions (assuming you have dual-core ESP32)
static const BaseType_t pro_cpu = 0;
static const BaseType_t app_cpu = 1;

static const uint32_t task_0_delay = 500;  // Time (ms) Task 0 blocks itself
static SemaphoreHandle_t bin_sem;

// Task in Core 0
void doTask0(void *parameters) {
  // Configure pin
  pinMode(led_pin, OUTPUT);

  // Do forever
  while (1) {
    // Notify other task
    xSemaphoreGive(bin_sem);

    // Yield processor for a while
    vTaskDelay(task_0_delay / portTICK_PERIOD_MS);
  }
}

// Task in Core 1
void doTask1(void *parameters) {
  // Do forever
  while (1) {
    // Wait for semaphore
    xSemaphoreTake(bin_sem, portMAX_DELAY);

    // Toggle LED
    digitalWrite(led_pin, !digitalRead(led_pin));
  }
}

void setup12() {
  // Create binary semaphore before starting tasks
  bin_sem = xSemaphoreCreateBinary();

  pinMode(led_pin, OUTPUT);

  // Start Task 0 (in Core 0)
  xTaskCreatePinnedToCore(doTask0, "Task 0", 1024, NULL, 1, NULL, pro_cpu);

  // Start Task 1 (in Core 1)
  xTaskCreatePinnedToCore(doTask1, "Task 1", 1024, NULL, 1, NULL, app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop12() {}
