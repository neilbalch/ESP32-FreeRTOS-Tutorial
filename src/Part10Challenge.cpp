#include "Part10challenge.h"
// #define TIMEOUT_SOLUTION
#define HIERARCHY_SOLUTION

namespace Part10Challenge {
TickType_t mutex_timeout = 1000 / portTICK_PERIOD_MS;

enum { NUM_TASKS = 5 };
enum { TASK_STACK_SIZE = 2048 };  // Bytes in ESP32, words in vanilla FreeRTOS

static SemaphoreHandle_t bin_sem;   // Wait for parameters to be read
static SemaphoreHandle_t done_sem;  // Notifies the main task when done
static SemaphoreHandle_t chopsticks[NUM_TASKS];

void eat(void* param) {
  int num;
  char buf[50];

  // Copy the parameter and increment the binary semaphore signal
  num = *(int*)param;
  xSemaphoreGive(bin_sem);

#ifdef TIMEOUT_SOLUTION
  while (1) {
    // Take left chopstick
    if (xSemaphoreTake(chopsticks[num], mutex_timeout) == pdFALSE) {
      sprintf(buf,
              "Philosopher %i timeed out while trying to take chopstick %i",
              num, num);
      Serial.println(buf);

      // Release kernel objects (none taken yet) and return to the top of the
      // loop later
      vTaskDelay(50 / portTICK_PERIOD_MS);
      continue;
    }
    sprintf(buf, "Philosopher %i took chopstick %i", num, num);
    Serial.println(buf);

    // Delay to force deadlock condition
    vTaskDelay(1 / portTICK_PERIOD_MS);

    // Take right chopstick
    if (xSemaphoreTake(chopsticks[(num + 1) % NUM_TASKS], mutex_timeout) ==
        pdFALSE) {
      sprintf(buf,
              "Philosopher %i timeed out while trying to take chopstick %i",
              num, (num + 1) % NUM_TASKS);
      Serial.println(buf);

      // Release kernel objects and return to the top of the loop later
      xSemaphoreGive(chopsticks[num]);
      vTaskDelay(50 / portTICK_PERIOD_MS);
      continue;
    }
    sprintf(buf, "Philosopher %i took chopstick %i", num,
            (num + 1) % NUM_TASKS);
    Serial.println(buf);

    // Do some "eating"
    sprintf(buf, "Philosopher %i is eating", num);
    Serial.println(buf);
    vTaskDelay(10 / portTICK_PERIOD_MS);

    // Put down right chopstick
    xSemaphoreGive(chopsticks[(num + 1) % NUM_TASKS]);
    sprintf(buf, "Philosopher %i returned chopstick %i", num,
            (num + 1) % NUM_TASKS);
    Serial.println(buf);

    // Put down left chopstick
    xSemaphoreGive(chopsticks[num]);
    sprintf(buf, "Philosopher %i returned chopstick %i", num, num);
    Serial.println(buf);

    // Notify main task and delete self
    xSemaphoreGive(done_sem);
    vTaskDelete(NULL);
    return;
  }
#endif

#ifdef HIERARCHY_SOLUTION
  uint8_t left = (num + 1) % NUM_TASKS;
  uint8_t right = num;

  // Decide which of the chopsticks available to the philosopher is lower in the
  // hierarchy (index in this case)
  if (right < left) {
    // Take right chopstick
    while (xSemaphoreTake(chopsticks[right], mutex_timeout) == pdFALSE)
      vTaskDelay(50 / portTICK_PERIOD_MS);
    sprintf(buf, "Philosopher %i took chopstick %i", num, right);
    Serial.println(buf);

    // Delay to force deadlock condition
    vTaskDelay(1 / portTICK_PERIOD_MS);

    // Take left chopstick
    while (xSemaphoreTake(chopsticks[left], mutex_timeout) == pdFALSE)
      vTaskDelay(50 / portTICK_PERIOD_MS);
    sprintf(buf, "Philosopher %i took chopstick %i", num, left);
    Serial.println(buf);
  } else {
    // Take left chopstick
    while (xSemaphoreTake(chopsticks[left], mutex_timeout) == pdFALSE)
      vTaskDelay(50 / portTICK_PERIOD_MS);
    sprintf(buf, "Philosopher %i took chopstick %i", num, left);
    Serial.println(buf);

    // Delay to force deadlock condition
    vTaskDelay(1 / portTICK_PERIOD_MS);

    // Take right chopstick
    while (xSemaphoreTake(chopsticks[right], mutex_timeout) == pdFALSE)
      vTaskDelay(50 / portTICK_PERIOD_MS);
    sprintf(buf, "Philosopher %i took chopstick %i", num, right);
    Serial.println(buf);
  }

  // Do some "eating"
  sprintf(buf, "Philosopher %i is eating", num);
  Serial.println(buf);
  vTaskDelay(10 / portTICK_PERIOD_MS);

  // Put down right chopstick
  xSemaphoreGive(chopsticks[right]);
  sprintf(buf, "Philosopher %i returned chopstick %i", num,
          (num + 1) % NUM_TASKS);
  Serial.println(buf);

  // Put down left chopstick
  xSemaphoreGive(chopsticks[left]);
  sprintf(buf, "Philosopher %i returned chopstick %i", num, num);
  Serial.println(buf);

  // Notify main task and delete self
  xSemaphoreGive(done_sem);
  vTaskDelete(NULL);
#endif
}

void setup() {
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);  // Wait a moment to start
  Serial.println();
  Serial.println("--- FreeRTOS \"Dining Philosophers\" Demo ---");

  char task_name[20];

  // Create kernel objects before starting tasks
  bin_sem = xSemaphoreCreateBinary();
  done_sem = xSemaphoreCreateCounting(NUM_TASKS, 0);
  for (int i = 0; i < NUM_TASKS; ++i) chopsticks[i] = xSemaphoreCreateMutex();

  // Have the philosophers start eating
  for (int i = 0; i < NUM_TASKS; ++i) {
    sprintf(task_name, "Philosopher %i", i);
    xTaskCreatePinnedToCore(eat, task_name, TASK_STACK_SIZE, (void*)&i, 1, NULL,
                            app_cpu);
    xSemaphoreTake(bin_sem, portMAX_DELAY);
  }

  // Wait until the philosophers are done
  for (int i = 0; i < NUM_TASKS; ++i) xSemaphoreTake(done_sem, portMAX_DELAY);

  // Say that the program completed without deadlock
  Serial.println("Done without deadlock!");
}

void loop() {}
}  // namespace Part10Challenge
