#include "Part5.h"

static const uint8_t msg_queue_len = 5;
static QueueHandle_t msg_queue;

void printMessages(void* param) {
  int item;

  while(1) {
    // See if theree's a message in the queue in a non-blocking manner
    if(xQueueReceive(msg_queue, (void*)&item, 0 /* tick timeout */) == pdTRUE) {
      Serial.println(item);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup5() {
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait a moment to start
  Serial.println();
  Serial.println("--- FreeRTOS Queue Demo ---");

  // Create the queue object
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  // Start the print messages task
  xTaskCreatePinnedToCore(printMessages, "Print Messages", 1024, NULL, 1, NULL, app_cpu);
}

void loop5() {
  static int num = 0;

  // Try to add the an item to the queue for 10 ticks, failing if the queue is full
  if(xQueueSend(msg_queue, (void*)&num, 10) != pdTRUE) {
    Serial.println("Queue send fail");  // Note that it's bad practice for
                                        // multiple tasks to share a resource
  }

  ++num;

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  // Makes the queue fill up too fast for the task to process them
  // vTaskDelay(500 / portTICK_PERIOD_MS);
}
