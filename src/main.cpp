#include <Arduino.h>

#include "heltec.h"
#include "setup_lora.hpp"
#include "setup_tasks.hpp"

void setup() {
  Heltec.begin(true, true, true, true, LORA_BAND);

  setup_lora();

  setup_tasks();
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(1);
}
