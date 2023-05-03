#include <Arduino.h>
#include <BrokerConnection.h>

#define STATUS_LED_PIN D0

const char *TOPICO_TEMPERATURA = "lab-lcf/TEMPERATURA";

float tempCelsius = 0.0f;
String deviceId;

void adcValToTemperatures(float adcVal, float *celsius);
void printTempToSerial(float adcVal, float tempCelsius);

void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  mqttStatus = connectBroker(TOPICO_TEMPERATURA, tempCelsius);
}

void loop()
{
  float adcVal = analogRead(A0);
  adcValToTemperatures(adcVal, &tempCelsius);
  loopBroker(mqttStatus, TOPICO_TEMPERATURA, tempCelsius);
}

void adcValToTemperatures(float adcVal, float *celsius) {
  *celsius = ((adcVal * (3300.0/1024)) - 500) / 10;
}

void printTempToSerial(float adcVal, float tempCelsius) {
  Serial.print("\nadc=");
  Serial.print(adcVal);
  Serial.print(" celsius=");
  Serial.print(tempCelsius);
  Serial.print("\n");
  delay(200);
}
