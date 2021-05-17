#ifndef INCLUDE_TASKS_SENDER_LORA_H__
#define INCLUDE_TASKS_SENDER_LORA_H__

#include "setup_tasks.hpp"
#include "lora_struct/lora_struct.hpp"

// extern QueueHandle_t relay_response_queue;
extern lora_packet message;

void send_lora_response (byte response) {
  int packet_rssi = LoRa.packetRssi();
  LoRa.beginPacket();
  LoRa.write(0xF1);
  LoRa.write(0xF2);
  LoRa.write(response);
  LoRa.write((uint8_t*)(&packet_rssi), sizeof(packet_rssi));
  LoRa.endPacket();
  Serial.println("send response");
}

void response_lora_code (void* parameters) {
  byte response;
  message.destination = 0xF2;
  message.local_address = 0xF3;

  while (1) {
    xQueueReceive(relay_response_queue, &response, portMAX_DELAY);
    xSemaphoreTake(lora_semaphore, portMAX_DELAY);
    send_lora_response(count_ignition);
    // vTaskResume(read_weight_task);
    // xTaskCreatePinnedToCore(read_weight_code, "weight", 1000, NULL, 2, &read_weight_task, 1);
    vTaskDelete(receiver_lora_task);
    xTaskCreatePinnedToCore(sender_weight_lora_code, "weight", 1000, NULL, 2, &sender_weight_task, 0);
    vTaskSuspend(response_lora_task);
    xSemaphoreGive(lora_semaphore);
    vTaskDelay(1);
  }
}

#endif // INCLUDE_TASKS_SENDER_LORA_H__