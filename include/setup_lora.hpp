#ifndef INCLUDE_SETUP_LORA_H__
#define INCLUDE_SETUP_LORA_H__

#include "heltec.h"

#define LORA_BAND 910E6

void setup_lora() { 
  LoRa.setSpreadingFactor(9);
  LoRa.setSignalBandwidth(500E3);
  LoRa.setCodingRate4(6);
  // LoRa.setPreambleLength(2);
  // LoRa.setSyncWord(0x12);
  LoRa.disableCrc();
  LoRa.enableCrc();
  LoRa.setTxPower(20,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.receive();
}

#endif // INCLUDE_SETUP_LORA_H__
