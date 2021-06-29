#include "Part3Challenge.h"

namespace Part3Challenge {
static int led_delay = 500;

void inputTask(void* param) {
  while (1) {
    if (!Serial.available())
      vTaskDelay(100 / portTICK_PERIOD_MS);
    else {
      led_delay = Serial.parseInt();
      Serial.print("Updated LED delay to: ");
      Serial.print(led_delay);
      Serial.println(" ms");
    }
  }
}
void blinkTask(void* param) {
  while (1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);

  vTaskDelay(1000 / portTICK_PERIOD_MS);  // Wait a moment to start

  Serial.println("Enter a number in ms to change the LED delay to");

  // Start the other tasks
  xTaskCreatePinnedToCore(inputTask, "Input Task", 1024, NULL, 1, NULL,
                          app_cpu);
  xTaskCreatePinnedToCore(blinkTask, "Blink Task", 1024, NULL, 1, NULL,
                          app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {}
}  // namespace Part3Challenge
