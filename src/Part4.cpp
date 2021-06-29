#include "Part4.h"

namespace Part4 {
// Task: perfrom a mundane task
void testTask(void* param) {
  while (1) {
    int a = 1;
    int b[100];  // This 4 * 100 = 400 byte array, which should overflow the
                 // 1024 - 768 = 256 byte stack (after task overhead is added)

    // Do something with the array to avoid compiler optimization
    for (int i = 0; i < 100; ++i) {
      b[i] = a + 1;
    }

    Serial.println(b[0]);
    vTaskDelay(50 / portTICK_PERIOD_MS);

    // Print out remaining stack memory (words)
    Serial.print("High water mark (words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    // Print out number of free heap memory bytes before malloc
    Serial.print("Heap before malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());
    int* ptr = (int*)pvPortMalloc(1024 * sizeof(int));

    // One way to prevent heap overflow is to check the malloc output
    if (ptr == NULL) {
      Serial.println("Not enough heap.");
      vPortFree(NULL);
    } else {
      // Do something with the memory so it's not optimized out by the compiler
      for (int i = 0; i < 1024; i++) {
        ptr[i] = 3;
      }
    }

    // Print out number of free heap memory bytes after malloc
    Serial.print("Heap after malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    // Free up our allocated memory
    vPortFree(ptr);

    // Wait for a while
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);  // Wait a moment to start
  Serial.println();
  Serial.println("--- FreeRTOS Memory Demo ---");

  // Start the other task
  // xTaskCreatePinnedToCore(testTask, "Test Task", 500, NULL, 1, NULL,
  // app_cpu); // Too little stack memory
  xTaskCreatePinnedToCore(testTask, "Test Task", 1024, NULL, 1, NULL, app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {}
}  // namespace Part4
