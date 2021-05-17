#ifndef INCLUDE_TASKS_RECEIVER_LORA_H__
#define INCLUDE_TASKS_RECEIVER_LORA_H__

#include "setup_tasks.hpp"
#include "lora_callback.hpp"

void recever_lora_code(void* parameters) {
  // Heltec.LoRa.onReceive(on_receive_lora_callback);
  // vTaskSuspend(read_weight_task);

  while (true) {
    if(xSemaphoreTake(lora_semaphore, 1)) {
      on_receive_lora_callback(LoRa.parsePacket());
      xSemaphoreGive(lora_semaphore);
    }
    vTaskDelay(1);
  }
}

#endif // __RECEIVER_LORA_H__
