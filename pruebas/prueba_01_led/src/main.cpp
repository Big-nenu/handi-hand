#include <Arduino.h>

#define LED_PIN 4
#define BUTTON_PIN 5

void setup() {
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
    bool presionado = digitalRead(BUTTON_PIN) == LOW;

    digitalWrite(LED_PIN, presionado ? HIGH : LOW);

    delay(20);
}