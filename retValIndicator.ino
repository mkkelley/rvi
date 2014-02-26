#include "tx.h"
#define SUCCESS_LED_PIN 2
#define FAILURE_LED_PIN 3
#define SWITCH_PIN 12

boolean is_switch_on() {
  int buttonState = digitalRead(SWITCH_PIN);
  if (buttonState == HIGH) {
    return true;
  }
  return false;
}

void light_failure() {
  digitalWrite(SUCCESS_LED_PIN, LOW);
  digitalWrite(FAILURE_LED_PIN, HIGH);
}

void light_success() {
  digitalWrite(FAILURE_LED_PIN, LOW);
  digitalWrite(SUCCESS_LED_PIN, HIGH);
}

void clear_lights() {
  digitalWrite(FAILURE_LED_PIN, LOW);
  digitalWrite(SUCCESS_LED_PIN, LOW);
}

void clearSerialBuffer() {
  while (Serial.available()) {
    Serial.read();
  }
}

void setup() {
  pinMode(SUCCESS_LED_PIN, OUTPUT);
  pinMode(FAILURE_LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  if (!is_switch_on()) {
    clear_lights();
    clearSerialBuffer();
    delay(100);
    return;
  }
  
  Transmission tx;
  
  if (tx.type() == RET) {
    byte ret_val = get_ret_tx_val(tx);
    if (ret_val == 0) {
      light_success();
    } else {
      light_failure();
    }
  }
}