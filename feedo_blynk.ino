#include "credentials.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>
#include<math.h>

#define BLYNK_PRINT Serial
#define temp_read 4
#define crescent_quantity 5
#define servo_pin 5
#define filter_motor_relay 0

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

OneWire oneWire(temp_read);
DallasTemperature sensors(&oneWire);
Servo myservo;

int quantity = 5;

void setup() {
  Serial.begin(115200);
  myservo.attach(servo_pin);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);
  myservo.write(0);
  pinMode(filter_motor_relay,OUTPUT);
}

void loop() {
  Blynk.run();
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  Blynk.virtualWrite  (V4, temperatureC);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Blynk.virtualWrite(V2, random(150,175)/25);

}

BLYNK_WRITE(V0) {
  quantity = param.asInt();
  Serial.print("Quantity :");
  Serial.println(quantity);
}

BLYNK_WRITE(V1) {  // Get virtual pin values from Blynk app and use them to control eqipments
  int feed = param.asInt();
  if (feed) {
    int turns = ceil(quantity / crescent_quantity);
    Serial.print(turns);
    for (int i = 0; i < turns; i++) {
      myservo.write(180);
      delay(1000);
       myservo.write(0);
      delay(1000);
      Serial.print("-");
    }
  }
}

BLYNK_WRITE(V3) {
  int filter_enable = param.asInt();
  digitalWrite(filter_motor_relay, !filter_enable);
  Serial.println(filter_enable);
}
