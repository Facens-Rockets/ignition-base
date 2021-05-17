#ifndef INCLUDE_LORA_STRUCT_LORA_STRUCT_H__
#define INCLUDE_LORA_STRUCT_LORA_STRUCT_H__

#include <Arduino.h>

struct lora_packet {
  byte destination;
  byte local_address;
  int message_counter;
  int message_length;
  String payload;
} message;

#endif // INCLUDE_LORA_STRUCT_LORA_STRUCT_H__
