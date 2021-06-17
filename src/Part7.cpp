#include "Part7.h"
// Likely required for vanilla FreeRTOS
// #include "semphr.h"

static SemaphoreHandle_t bin_sem;
static SemaphoreHandle_t sem_params;

void blinkLED2(void* param) {
  int delay_period = *(int*)param;
  // Add one to the semaphore value
  xSemaphoreGive(bin_sem);
  Serial.print("Received: ");
  Serial.println(delay_period);

  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(delay_period / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(delay_period / portTICK_PERIOD_MS);
  }
}

struct Message {
  char body[20];
  int len;
};
static const int num_tasks = 5;

void myTask(void* param) {
  // Copy the message struct from the parameter to a local variable
  Message msg = *(Message*)param;

  // Increment the semaphore to indicate that the parameter has been read
  xSemaphoreGive(sem_params);

  // Print out the message contents
  Serial.print("Received: ");
  Serial.print(msg.body);
  Serial.print(" | len: ");
  Serial.println(msg.len);

  // Wait for a little while and then self-delete
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL);
}

void setup7() {
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);

  vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait a moment to start
  Serial.println();
  Serial.println("--- FreeRTOS Semaphore Demo ---");
  Serial.println("Enter a number for delay in ms");

  // Create bin_sem
  bin_sem = xSemaphoreCreateBinary();
  // The binary semaphore is initialized to zero, so there is no need to take it
  // before starting the task

  // Wait for serial input and then parse it
  while(Serial.available() <= 0);
  int delay_period = Serial.parseInt();
  Serial.print("Sending: ");
  Serial.println(delay_period);

  // Start two duplicate increment tasks
  xTaskCreatePinnedToCore(blinkLED2, "Blink LED Task", 1024, (void*)&delay_period, 1, NULL, app_cpu);
  Serial.println("Done!");

  // Do nothing until the binary semaphore can be returned
  xSemaphoreTake(bin_sem, portMAX_DELAY);

  // ---------------------------------------------------------------------------
  //                        Counting Semaphore Demo
  // ---------------------------------------------------------------------------

  char task_name[12];
  Message msg;
  char text[20] = "All your base";
  sem_params = xSemaphoreCreateCounting(num_tasks /* max value */, 0 /* initial value */);

  strcpy(msg.body, text);
  msg.len = strlen(text);

  // Start identical tasks
  for(int i = 0; i < num_tasks; ++i) {
    // Generate unique task name
    sprintf(task_name, "Task %i", i);

    // Start the task and pass in the common message arguemnt
    xTaskCreatePinnedToCore(myTask, task_name, 1024, (void*)&msg, 1, NULL, app_cpu);
  }

  // Wait until all tasks have returned the semaphore
  for(int i = 0; i < num_tasks; ++i) {
    xSemaphoreTake(sem_params, portMAX_DELAY);
  }
  Serial.println("All tasks created");

  // Delete the setup() and loop() task
  vTaskDelete(NULL);
}

void loop7() {
}
