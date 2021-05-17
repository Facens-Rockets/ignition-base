#ifndef INCLUDE_SENDER_WEIGHT_LORA_H__
#define INCLUDE_SENDER_WEIGHT_LORA_H__

#include "setup_tasks.hpp"

void send_weight_lora(float weight, uint64_t timer){
  LoRa.beginPacket();
  LoRa.write(0xFF);
  LoRa.write(0xF2);
  LoRa.write((uint8_t*)(&weight), sizeof(weight));
  LoRa.write((uint8_t*)(&timer), sizeof(timer));
  LoRa.endPacket();
  // Serial.println("send lora");
}

void sender_weight_lora_code(void* parameters) {
  // float weight_queue[10];
  float weight;
  uint64_t timer;

  while(true){
    xQueueReceive(sender_weight_queue, &weight, portMAX_DELAY);
    xQueueReceive(timer_weight_queue, &timer, portMAX_DELAY);
    send_weight_lora(weight, timer);
    vTaskDelay(1);
  }
}

#endif // INCLUDE_SENDER_WEIGHT_LORA_H__