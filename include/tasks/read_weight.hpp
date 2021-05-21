#ifndef INCLUDE_READ_WEIGHT_H__
#define INCLUDE_READ_WEIGHT_H__
#include <HX711.h>

#include "sender_weight_lora.hpp"
#include "setup_tasks.hpp"

#define LOADCELL_DOUT_PIN 12
#define LOADCELL_SCK_PIN 22

#define LOADCELL_OFFSET 50682624
#define LOADCELL_DIVIDER 33874.87

HX711 loadCell;

void calibrate_function_rockets() {
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

float Y1 = 65.0;  // calibrated mass to be added
long x1 = 0L;
long x0 = 0L;
float avg_size = 10.0;  // amount of averages for each mass measurement

void calibration_motta() {
  for (int ii = 0; ii < int(avg_size); ii++) {
    delay(10);
    x0 += loadCell.read();
  }
  x0 /= long(avg_size);
  Serial.println("Add Calibrated Mass");
  // calibration procedure (mass should be added equal to Y1)
  int ii = 1;
  while (true) {
    if (loadCell.read() < x0 + 10000) {
    } else {
      ii++;
      delay(2000);
      for (int jj = 0; jj < int(avg_size); jj++) {
        x1 += loadCell.read();
      }
      x1 /= long(avg_size);
      break;
    }
  }
  Serial.println("Calibration Complete");
}

void read_hx711_motta() {
  long reading = 0;
  for (int jj = 0; jj < int(avg_size); jj++) {
    reading += loadCell.read();
  }
  reading /= long(avg_size);
  // calculating mass based on calibration and linear fit
  float ratio_1 = (float)(reading - x0);
  float ratio_2 = (float)(x1 - x0);
  float ratio = ratio_1 / ratio_2;
  float mass = Y1 * ratio;
  // Serial.print("Raw: ");
  // Serial.print(reading);
  // Serial.print(", ");
  Serial.println(mass);
}

void read_weight_code(void* parameters) {
  bool init = false;
  Serial.println("loadCell");

  loadCell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  if (!loadCell.is_ready()) {
    Serial.println("stoped");
    while (true) {
    }
  }
  uint64_t adc = loadCell.read();
  Serial.println(adc);

  // calibrate_function_rockets();
  // calibration_motta();

  loadCell.set_scale(LOADCELL_DIVIDER);
  loadCell.tare();
  // loadCell.set_offset(LOADCELL_OFFSET);

  uint64_t timer = 0;
  uint8_t count = 0;
  uint8_t lastCount = 0;
  float weight = 0;
  xQueueReceive(sender_weight_queue_init, &init, portMAX_DELAY);
  uint64_t initial_timer = millis();
  while (init) {
    // read_hx711_motta();
    weight = loadCell.get_units(1);
    timer = millis() - initial_timer;
    Serial.println(weight, 3);
    xQueueSend(sender_weight_queue, &weight, portMAX_DELAY);
    xQueueSend(timer_weight_queue, &timer, portMAX_DELAY);
    // vTaskDelay(pdMS_TO_TICKS(40));
    // while(true){}
    // count++;
    // if(millis() > initial_timer + 1000){
    //   // lastCount = count - lastCount;
    //   // Serial.println(lastCount);
    //   Serial.println(count);
    //   while(true){}
    // }
    vTaskDelay(1);
    // vTaskDelay(pdMS_TO_TICKS(100));
  }
}

#endif  // INCLUDE_READ_WEIGHT_H__