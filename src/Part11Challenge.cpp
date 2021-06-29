#include "Part11challenge.h"

TickType_t cs_wait_challenge = 250;    // Time spent in critical section (ms)
TickType_t med_wait_challenge = 5000;  // Time medium task spends working (ms)

// *****************************************************************************
// NOTE: This program triggers the ESP-IDF tick timer interrupt watchdog timer
// because too much time is spent in a critical section and ESP-IDF assumes
// something has broken. (crashes and resets the MCU) The same is true of the
// official solution:
// https://github.com/ShawnHymel/introduction-to-rtos/blob/main/11-priority-inversion/esp32-freertos-11-solution-critical-section/esp32-freertos-11-solution-critical-section.ino
// REF: https://github.com/espressif/esp-idf/issues/7199
// *****************************************************************************

static portMUX_TYPE spin_lock =
    portMUX_INITIALIZER_UNLOCKED;  // Note that the use of spinlocks and
                                   // portENTER_CRITICAL() / portEXIT_CRITICAL()
                                   // disable interrupts and the scheduler in
                                   // the task's core.

// Task L (low priority)
void doTaskL_challenge(void *parameters) {
  TickType_t timestamp;

  // Do forever
  while (1) {
    // Take lock
    Serial.println("Task L trying to take lock...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    portENTER_CRITICAL(&spin_lock);

    // Say how long we spend waiting for a lock
    Serial.print("Task L got lock. Spent ");
    Serial.print((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp);
    Serial.println(" ms waiting for lock. Doing some work...");

    // Hog the processor for a while doing nothing (uses while loop to avoid
    // yielding to another task)
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    Serial.println("2");
    while ((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp <
           cs_wait_challenge)
      ;
    Serial.println("3");

    // Release lock
    Serial.println("Task L releasing lock.");
    portEXIT_CRITICAL(&spin_lock);

    // Go to sleep
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task M (medium priority)
void doTaskM_challenge(void *parameters) {
  TickType_t timestamp;

  // Do forever
  while (1) {
    // Hog the processor for a while doing nothing (uses while loop to avoid
    // yielding to another task)
    Serial.println("Task M doing some work...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp <
           med_wait_challenge)
      ;

    // Go to sleep
    Serial.println("Task M done!");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task H (high priority)
void doTaskH_challenge(void *parameters) {
  TickType_t timestamp;

  // Do forever
  while (1) {
    // Take lock
    Serial.println("Task H trying to take lock...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    portENTER_CRITICAL(&spin_lock);

    // Say how long we spend waiting for a lock
    Serial.print("Task H got lock. Spent ");
    Serial.print((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp);
    Serial.println(" ms waiting for lock. Doing some work...");

    // Hog the processor for a while doing nothing (uses while loop to avoid
    // yielding to another task)
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp <
           cs_wait_challenge)
      ;

    // Release lock
    Serial.println("Task H releasing lock.");
    portEXIT_CRITICAL(&spin_lock);

    // Go to sleep
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup11challenge() {
  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println(
      "--- FreeRTOS Priority Inversion Spin Lock / Critical Section ---");

  // The order of starting the tasks matters to force priority inversion

  // Start Task L (low priority)
  xTaskCreatePinnedToCore(doTaskL_challenge, "Task L", 1024, NULL, 1, NULL,
                          app_cpu);

  Serial.println("A");
  // Introduce a delay to force priority inversion
  vTaskDelay(1 / portTICK_PERIOD_MS);

  Serial.println("B");
  // Start Task H (high priority)
  xTaskCreatePinnedToCore(doTaskH_challenge, "Task H", 1024, NULL, 3, NULL,
                          app_cpu);

  Serial.println("C");
  // Start Task M (medium priority)
  xTaskCreatePinnedToCore(doTaskM_challenge, "Task M", 1024, NULL, 2, NULL,
                          app_cpu);

  Serial.println("D");
  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop11challenge() {}
