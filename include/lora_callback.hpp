#ifndef INCLUDE_LORA_CALLBACK_H__
#define INCLUDE_LORA_CALLBACK_H__

#include "heltec.h"
#include "lora_struct/lora_struct.hpp"
#include "tasks/sender_weight_lora.hpp"
#include "setup_tasks.hpp"

extern QueueHandle_t relay_action_queue;
extern QueueHandle_t relay_response_queue;

extern lora_packet message;

byte relay_action_message;

uint8_t count_ignition = 0;

void on_receive_lora_callback(int packet_size){
  if(packet_size <= 0) return;
  byte response;

  message.destination = LoRa.read();
  
  if (message.destination == 0xF2) {
    message.local_address = LoRa.read();
    if (message.local_address == 0xF1) {
      relay_action_message = LoRa.read();
      count_ignition = LoRa.read();
      if (relay_action_message == 0x0A) {
        xQueueSend(relay_action_queue, &relay_action_message, portMAX_DELAY);
        // xQueueSend(relay_response_queue, &count_ignition, portMAX_DELAY);
        // xTaskCreatePinnedToCore(sender_weight_lora_code, "weight", 1000, NULL, 2, &sender_weight_task, 0);
      }
    }
  }else{
    // message.payload = LoRa.readString();
  }
  
  Serial.print(message.destination, HEX);
  Serial.print("\t");
  Serial.print(message.local_address, HEX);
  Serial.print("\t");
  Serial.print(relay_action_message, HEX);
  Serial.print("\t");
  Serial.print(count_ignition, HEX);
  Serial.print("\t");
  Serial.println(LoRa.packetRssi(), DEC);
  

}

#endif // INCLUDE_LORA_CALLBACK_H__
