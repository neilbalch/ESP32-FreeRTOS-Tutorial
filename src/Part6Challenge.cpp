#include "Part6Challenge.h"
// Likely required for vanilla FreeRTOS
// #include "semphr.h"

static SemaphoreHandle_t mutex;

void blinkLED(void* param) {
  int delay_period = *(int*)param;
  xSemaphoreGive(mutex);
  Serial.print("Received: ");
  Serial.println(delay_period);

  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(delay_period / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(delay_period / portTICK_PERIOD_MS);
  }
}

void setup6challenge() {
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);

  vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait a moment to start
  Serial.println("Enter a number for delay in ms");

  // Create mutex
  mutex = xSemaphoreCreateMutex();
  if(xSemaphoreTake(mutex, portMAX_DELAY) == pdFALSE) Serial.println("Error taking mutex!");

  // Wait for serial input and then parse it
  while(Serial.available() <= 0);
  int delay_period = Serial.parseInt();
  Serial.print("Sending: ");
  Serial.println(delay_period);

  // Start two duplicate increment tasks
  xTaskCreatePinnedToCore(blinkLED, "Blink LED Task", 1024, (void*)&delay_period, 1, NULL, app_cpu);
  Serial.println("Done!");

  xSemaphoreTake(mutex, portMAX_DELAY);

  // Delete the setup() and loop() task
  vTaskDelete(NULL);
}

void loop6challenge() {
}
