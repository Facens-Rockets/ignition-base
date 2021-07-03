#ifndef INCLUDE_SETUP_TASKS_H__
#define INCLUDE_SETUP_TASKS_H__

TaskHandle_t receiver_lora_task;
TaskHandle_t response_lora_task;
TaskHandle_t relay_action_task;
TaskHandle_t read_weight_task;
TaskHandle_t sender_weight_task;

QueueHandle_t relay_action_queue;
QueueHandle_t relay_response_queue;
QueueHandle_t sender_weight_queue;
QueueHandle_t timer_weight_queue;
QueueHandle_t sender_weight_queue_init;

SemaphoreHandle_t lora_semaphore;

#include "tasks/receiver_lora.hpp"
#include "tasks/relay_action.hpp"
#include "tasks/response_lora.hpp"
#include "tasks/read_weight.hpp"

void core_zero(int core = 0) {
  xTaskCreatePinnedToCore(recever_lora_code, "receive", 1000, NULL, 2, &receiver_lora_task, core);
  xTaskCreatePinnedToCore(response_lora_code, "send", 1000, NULL, 1, &response_lora_task, core);
}
void core_one(int core = 1) {
  xTaskCreatePinnedToCore(relay_action_code, "relay", 1000, NULL, 1, &relay_action_task, core);
  xTaskCreatePinnedToCore(read_weight_code, "weight", 1000, NULL, 2, &read_weight_task, core);
}

void setup_tasks() {
  relay_action_queue = xQueueCreate(2, sizeof(uint8_t));
  relay_response_queue = xQueueCreate(2, sizeof(uint8_t));
  sender_weight_queue = xQueueCreate(1, sizeof(float));
  timer_weight_queue = xQueueCreate(1, sizeof(uint64_t));
  sender_weight_queue_init = xQueueCreate(1, sizeof(bool));

  lora_semaphore = xSemaphoreCreateMutex();

  core_zero();
  core_one();
  // vTaskSuspend(read_weight_task);

}

#endif // INCLUDE_SETUP_TASKS_H__