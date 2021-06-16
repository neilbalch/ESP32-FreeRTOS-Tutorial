#include "Part4.h"

static bool msg_ready = false;
static char* msg_ptr = NULL;

// Task: listen for a message and save it to a buffer
void listenTask(void* param) {
  char curr;
  int buf_len = 25;
  char buf[buf_len];
  uint8_t buf_index = 0;

  // Clear the buffer
  memset(buf, 0, buf_len);

  while(1) {
    while (Serial.available()) {
      curr = Serial.read();

      if(curr == '\n') { // This is the last character in a message line
        buf[buf_index] = '\0';
        ++buf_index;

        if(!msg_ready) {
          msg_ptr = (char*)pvPortMalloc(buf_index * sizeof(char));
          // Ensure that the memory was allocates successfully
          configASSERT(msg_ptr);

          strcpy(msg_ptr, buf);
          msg_ready = true;
        }

        // Clear the buffer
        memset(buf, 0, buf_len);
        buf_index = 0;
      } else if(buf_index < buf_len - 1) {
        buf[buf_index] = curr;
        ++buf_index;
      }
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// Task: look for a full buffer and print it
void echoTask(void* param) {
  while(1) {
    if(!msg_ready) vTaskDelay(100 / portTICK_PERIOD_MS);
    else {
      Serial.println(msg_ptr);
      vPortFree(msg_ptr);
      msg_ptr = NULL;
      msg_ready = false;
    }
  }
};

void setup4challenge() {
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait a moment to start
  Serial.println("Enter a message:");

  // Start the other tasks
  xTaskCreatePinnedToCore(listenTask, "Listen Task", 1024, NULL, 1, NULL, app_cpu);
  xTaskCreatePinnedToCore(echoTask, "Echo Task", 1024, NULL, 1, NULL, app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop4challenge() {
}