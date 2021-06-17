#include "Part5Challenge.h"

static const uint8_t terminal_queue_len = 5;
static QueueHandle_t terminal_queue;
static const uint8_t led_queue_len = 5;
static QueueHandle_t led_queue;

static int led_delay = 500;
static int led_blink_count = 0;

struct TerminalMessage {
  char* msg;
  int msg_len;
  int num_blinks;
};

void terminal(void* param) {
  TerminalMessage item;
  char curr;
  int buf_len = 25;
  char buf[buf_len];
  uint8_t buf_index = 0;

  while(1) {
    if(xQueueReceive(terminal_queue, (void*)&item, 0 /* tick timeout */) == pdTRUE) {
      if(strcmp(item.msg, "Blinked") == 0) {
        Serial.print("LED has blinked ");
        Serial.print(item.num_blinks);
        Serial.println(" times");
      }
    }

    while(Serial.available()) {
      curr = Serial.read();

      if(curr == '\n') { // This is the last character in a message line
        buf[buf_index] = '\0';
        ++buf_index;
        Serial.println();

        if(strncmp(buf, "delay", 5) == 0) {
          char delay_str[5];
          strcpy(delay_str, buf + 6);
          int delay = atoi(delay_str);
          xQueueSend(led_queue, (void*)&delay, 0);
        }

        // Clear the buffer
        memset(buf, 0, buf_len);
        buf_index = 0;
      } else if(buf_index < buf_len - 1) {
        buf[buf_index] = curr;
        ++buf_index;
        Serial.print(curr);
      }
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void ledControl(void* param) {
  int new_delay;

  while(1) {
    if(xQueueReceive(led_queue, (void*)&new_delay, 0 /* tick timeout */) == pdTRUE) {
      led_delay = new_delay;
    }

    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    ++led_blink_count;

    if(led_blink_count % 100 == 0) {
      TerminalMessage msg = {"Blinked", 7, led_blink_count};
      xQueueSend(terminal_queue, (void*)&msg, 0);
    }
  }
}

void setup5challenge() {
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);

  vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait a moment to start
  Serial.println("Enter \'delay XXX\' where XXX is a desired LED blink delay in ms");

  // Create the queue object
  terminal_queue = xQueueCreate(terminal_queue_len, sizeof(TerminalMessage));
  led_queue = xQueueCreate(led_queue_len, sizeof(int));

  // Start the print messages task
  xTaskCreatePinnedToCore(terminal, "Terminal", 1024, NULL, 1, NULL, app_cpu);
  xTaskCreatePinnedToCore(ledControl, "LED Control", 1024, NULL, 1, NULL, app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop5challenge() {
}
