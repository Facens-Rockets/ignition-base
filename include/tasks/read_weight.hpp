#ifndef INCLUDE_READ_WEIGHT_H__
#define INCLUDE_READ_WEIGHT_H__
#include <HX711.h>

#include "sender_weight_lora.hpp"
#include "setup_tasks.hpp"

#define LOADCELL_DOUT_PIN 12
#define LOADCELL_SCK_PIN 22

#define LOADCELL_OFFSET 50682624
#define LOADCELL_DIVIDER 33874.877777

HX711 loadCell;

void calibrate_function() {
  // loadCell.begin (DT, SCK);
  // Serial.begin(115200);
  // Serial.print("Leitura da Tara:  ");
  // Serial.println(loadCell.read());  // Aguada o termino de verificação do
  // peso Serial.println("Aguarde!"); Serial.println("Iniciando ...");
  loadCell.set_scale();  // Utiliza uma loadCell padrão de verificação

  loadCell.tare(20);  // Fixa o peso como tara
  Serial.println("Insira o item para Pesar");

  uint8_t count = 0;
  float media = 0;

  while (true) {
    float weight = loadCell.get_units(1);
    media += weight;
    Serial.print("Valor da Leitura:  ");
    Serial.println(weight, 0);  // Retorna peso descontada a tara
    // vTaskDelay(portMAX_DELAY);
    // count++;
    // if (count >= 20) {
    //   Serial.println(media / 20);
    //   vTaskDelay(portMAX_DELAY);
    // }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void read_weight_code(void* parameters) {
  Serial.println("loadCell");

  loadCell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  if (!loadCell.is_ready()) {
    Serial.println("stoped");
    while (true) {
    }
  }
  uint64_t adc = loadCell.read();
  Serial.println(adc);

  // calibrate_function();

  loadCell.set_scale(LOADCELL_DIVIDER);
  loadCell.tare();
  // loadCell.set_offset(LOADCELL_OFFSET);

  uint64_t timer = 0;
  uint8_t count = 0;
  uint8_t lastCount = 0;
  float weight = 0;
  while (true) {
    weight = loadCell.get_units(2);
    Serial.println(weight, 3);
    xQueueSend(sender_weight_queue, &weight, portMAX_DELAY);
    xQueueSend(timer_weight_queue, &timer, portMAX_DELAY);
    timer = millis();
    // vTaskDelay(pdMS_TO_TICKS(40));
    // while(true){}
    // count++;
    // if(millis() > timer + 1000){
    //   lastCount = count - lastCount;
    //   Serial.println(lastCount);
    //   // while(true){}
    // }
  }
}

#endif  // INCLUDE_READ_WEIGHT_H__