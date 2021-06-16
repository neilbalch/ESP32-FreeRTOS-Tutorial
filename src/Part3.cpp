#include "Part3.h"

// Random string to print
const char msg[] = "Barkadeer brig Arr booty rum.";

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

// Task 1: print to the serial terminal, with a lower priority
void startTask1(void* param) {
  int msg_len = strlen(msg);

  while(1) {
    // Print the string one character at a time on each line
    Serial.println();
    for(int i = 0; i < msg_len; ++i)
      Serial.print(msg[i]);
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task 2: print to the serial terminal, with a higher priority
void startTask2(void* param) {
  while(1) {
    Serial.print("*");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup3() {
  Serial.begin(300); // Go slow to visually see the task preemption

  vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait a moment to start
  Serial.println();
  Serial.println("--- FreeRTOS Task Demo ---");

  // Print self priority
  Serial.print("Setup and loop task running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority ");
  Serial.print(uxTaskPriorityGet(NULL));

  // Start the other tasks
  xTaskCreatePinnedToCore(startTask1, "Task 1", 1024, NULL, 1, &task_1, app_cpu);
  xTaskCreatePinnedToCore(startTask2, "Task 2", 1024, NULL, 2, &task_2, app_cpu);
}

void loop3() {
  // Periodically suspend the higher priority task for some intervals
  for(int i = 0; i < 3; ++i) {
    vTaskSuspend(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  // Delete the lower priority task
  if(task_1 != NULL) {
    vTaskDelete(task_1);
    task_1 = NULL;
  }
}