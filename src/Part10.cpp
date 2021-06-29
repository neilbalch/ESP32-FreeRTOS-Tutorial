#include "Part10.h"
// #define TIMEOUT_SOLUTION
#define HIERARCHY_SOLUTION

#ifdef TIMEOUT_SOLUTION
TickType_t mutex_timeout = 1000 / portTICK_PERIOD_MS;
#endif

namespace Part10 {
static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;

// Task A (high priority)
void doTaskA(void *parameters) {
  // Loop forever
  while (1) {
#ifdef TIMEOUT_SOLUTION
    // Take mutex 1
    if (xSemaphoreTake(mutex_1, mutex_timeout) == pdTRUE) {
      // Say we took mutex 1 and wait (to force deadlock)
      Serial.println("Task A took mutex 1");
      vTaskDelay(1 / portTICK_PERIOD_MS);

      // Take mutex 2
      if (xSemaphoreTake(mutex_2, mutex_timeout) == pdTRUE) {
        // Say we took mutex 2
        Serial.println("Task A took mutex 2");

        // Critical section protected by 2 mutexes
        Serial.println("Task A doing some work");
        vTaskDelay(500 / portTICK_PERIOD_MS);

        // Give back mutex
        xSemaphoreGive(mutex_2);
      } else {
        Serial.println("Task A timed out waiting for mutex 2");
      }

      // Give back mutex
      xSemaphoreGive(mutex_1);
    } else {
      Serial.println("Task A timed out waiting for mutex 1");
    }
#endif

#ifdef HIERARCHY_SOLUTION
    // Take mutex 1 (introduce wait to force deadlock)
    xSemaphoreTake(mutex_1, portMAX_DELAY);
    Serial.println("Task A took mutex 1");
    vTaskDelay(1 / portTICK_PERIOD_MS);

    // Take mutex 2
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    Serial.println("Task A took mutex 2");

    // Critical section protected by 2 mutexes
    Serial.println("Task A doing some work");
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Give back mutexes (in reverse order that we took them)
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);
#endif

    // Wait to let the other task execute
    Serial.println("Task A going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task B (low priority)
void doTaskB(void *parameters) {
  // Loop forever
  while (1) {
#ifdef TIMEOUT_SOLUTION
    // Take mutex 2
    if (xSemaphoreTake(mutex_2, mutex_timeout) == pdTRUE) {
      // Say we took mutex 2 and wait (to force deadlock)
      Serial.println("Task B took mutex 2");
      vTaskDelay(1 / portTICK_PERIOD_MS);

      // Take mutex 1
      if (xSemaphoreTake(mutex_1, mutex_timeout) == pdTRUE) {
        // Say we took mutex 1
        Serial.println("Task B took mutex 1");

        // Critical section protected by 2 mutexes
        Serial.println("Task B doing some work");
        vTaskDelay(500 / portTICK_PERIOD_MS);

        // Give back mutex
        xSemaphoreGive(mutex_2);
      } else {
        Serial.println("Task B timed out waiting for mutex 1");
      }

      // Give back mutex
      xSemaphoreGive(mutex_1);
    } else {
      Serial.println("Task B timed out waiting for mutex 2");
    }
#endif

#ifdef HIERARCHY_SOLUTION
    // Take mutex 1 (introduce wait to force deadlock)
    xSemaphoreTake(mutex_1, portMAX_DELAY);
    Serial.println("Task B took mutex 1");
    vTaskDelay(1 / portTICK_PERIOD_MS);

    // Take mutex 2
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    Serial.println("Task B took mutex 2");

    // Critical section protected by 2 mutexes
    Serial.println("Task B doing some work");
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Give back mutexes (in reverse order that we took them)
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);
#endif

    // Wait to let the other task execute
    Serial.println("Task B going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);  // Wait a moment to start
  Serial.println();
  Serial.println("--- FreeRTOS Deadlock Demo ---");

  // Create mutexes before starting tasks
  mutex_1 = xSemaphoreCreateMutex();
  mutex_2 = xSemaphoreCreateMutex();

  // Start Task A (high priority)
  xTaskCreatePinnedToCore(doTaskA, "Task A", 1024, NULL, 2, NULL, app_cpu);

  // Start Task B (low priority)
  xTaskCreatePinnedToCore(doTaskB, "Task B", 1024, NULL, 1, NULL, app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {}
}  // namespace Part10
