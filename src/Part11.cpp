#include "Part11.h"
// #define UNBOUNDED_PRIORITY_INVERSION
#define NO_UNBOUNDED_PRIORITY_INVERSION

TickType_t cs_wait = 250;    // Time spent in critical section (ms)
TickType_t med_wait = 5000;  // Time medium task spends working (ms)

static SemaphoreHandle_t lock;

// Tasks use portMAX_DELAY to force blocking behavior. Shorter timeouts could be
// used for deadlock recovery

// Task L (low priority)
void doTaskL(void *parameters) {
  TickType_t timestamp;

  // Do forever
  while (1) {
    // Take lock
    Serial.println("Task L trying to take lock...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    xSemaphoreTake(lock, portMAX_DELAY);

    // Say how long we spend waiting for a lock
    Serial.print("Task L got lock. Spent ");
    Serial.print((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp);
    Serial.println(" ms waiting for lock. Doing some work...");

    // Hog the processor for a while doing nothing (uses while loop to avoid
    // yielding to another task)
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < cs_wait)
      ;

    // Release lock
    Serial.println("Task L releasing lock.");
    xSemaphoreGive(lock);

    // Go to sleep
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task M (medium priority)
void doTaskM(void *parameters) {
  TickType_t timestamp;

  // Do forever
  while (1) {
    // Hog the processor for a while doing nothing (uses while loop to avoid
    // yielding to another task)
    Serial.println("Task M doing some work...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < med_wait)
      ;

    // Go to sleep
    Serial.println("Task M done!");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task H (high priority)
void doTaskH(void *parameters) {
  TickType_t timestamp;

  // Do forever
  while (1) {
    // Take lock
    Serial.println("Task H trying to take lock...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    xSemaphoreTake(lock, portMAX_DELAY);

    // Say how long we spend waiting for a lock
    Serial.print("Task H got lock. Spent ");
    Serial.print((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp);
    Serial.println(" ms waiting for lock. Doing some work...");

    // Hog the processor for a while doing nothing (uses while loop to avoid
    // yielding to another task)
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < cs_wait)
      ;

    // Release lock
    Serial.println("Task H releasing lock.");
    xSemaphoreGive(lock);

    // Go to sleep
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup11() {
  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("--- FreeRTOS Priority Inversion Demo ---");

  // Create semaphores and mutexes before starting tasks
  // A mutex isn't used because FreeRTOS mutexes contain a fix for this kind of
  // unbounded priority inversion
  #ifdef UNBOUNDED_PRIORITY_INVERSION
  lock = xSemaphoreCreateBinary();
  xSemaphoreGive(lock);  // Make sure binary semaphore starts at 1
  #endif
  #ifdef NO_UNBOUNDED_PRIORITY_INVERSION
  lock = xSemaphoreCreateMutex();
  #endif

  // The order of starting the tasks matters to force priority inversion

  // Start Task L (low priority)
  xTaskCreatePinnedToCore(doTaskL, "Task L", 1024, NULL, 1, NULL, app_cpu);

  // Introduce a delay to force priority inversion
  vTaskDelay(1 / portTICK_PERIOD_MS);

  // Start Task H (high priority)
  xTaskCreatePinnedToCore(doTaskH, "Task H", 1024, NULL, 3, NULL, app_cpu);

  // Start Task M (medium priority)
  xTaskCreatePinnedToCore(doTaskM, "Task M", 1024, NULL, 2, NULL, app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop11() {}
