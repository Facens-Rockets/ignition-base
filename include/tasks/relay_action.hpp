#ifndef INCLUDE_TASKS_RELAY_ACTION_H__
#define INCLUDE_TASKS_RELAY_ACTION_H__

#include "setup_tasks.hpp"
#include "lora_callback.hpp"
#include "tasks/read_weight.hpp"

#define relay_pin 13

extern QueueHandle_t relay_action_queue;
extern QueueHandle_t relay_response_queue;

void relay_action_code (void* parameters) {
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, HIGH);

  byte message;
  byte response = 1;

  while (true) {
    xQueueReceive(relay_action_queue, &message, portMAX_DELAY);

    if(message == 0x0A){
      digitalWrite(relay_pin, LOW);
      vTaskDelay(pdMS_TO_TICKS(100));
      digitalWrite(relay_pin, HIGH);
      xQueueSend(relay_response_queue, &response, portMAX_DELAY);
    }

    vTaskDelay(1);
  }
}

#endif // __RECEIVER_LORA_H__
