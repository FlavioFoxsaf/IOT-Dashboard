#include "thingProperties.h"
#include <DHT.h>

#define PIN_BUTTON     4
#define PIN_POT        5
#define PIN_LDR        6
#define PIN_DHT        7
#define PIN_BUZZER    15
#define PIN_RGB_R     16
#define PIN_RGB_G     17
#define PIN_RGB_B     18

#define DHTTYPE DHT22
DHT dht(PIN_DHT, DHTTYPE);

const bool RGB_COMMON_ANODE = false;
unsigned long lastUpdate = 0;

void setColor(int r, int g, int b) {
  if (RGB_COMMON_ANODE) {
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
  }

  analogWrite(PIN_RGB_R, r);
  analogWrite(PIN_RGB_G, g);
  analogWrite(PIN_RGB_B, b);
}

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);

  digitalWrite(PIN_BUZZER, LOW);
  dht.begin();

  temperature = 24.5;
  humidity = 66.0;
  luminosity = 800;
  lamp_state = true;
  cloud_command = "Sistema Ativo";

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setColor(0, 0, 255);
}

void loop() {
  ArduinoCloud.update();

  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    temperature = isnan(t) ? 24.5 : t;
    humidity = isnan(h) ? 66.0 : h;
    luminosity = analogRead(PIN_LDR);

    lamp_state = true;

    setColor(0, 0, 255);

    ArduinoCloud.update();
  }
}

void onLampStateChange() {
  lamp_state = true;
  setColor(0, 0, 255);
}

void onCloudCommandChange() {
  cloud_command.trim();

  if (cloud_command == "Vermelho") {
    setColor(255, 0, 0);
  } else if (cloud_command == "Amarelo") {
    setColor(255, 255, 0);
  } else if (cloud_command == "Azul") {
    setColor(0, 0, 255);
  } else {
    setColor(0, 0, 255);
  }
}

void onTemperatureChange() {}
void onHumidityChange() {}
void onLuminosityChange() {}
