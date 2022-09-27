/** Criado por Felipe Moreira, Ricardo Sommer e Danilo Cesar;
   ESP32 + DHT22 Example for Wokwi
   
   https://wokwi.com/arduino/projects/322410731508073042
*/
#include "WiFi.h"
#include "DHTesp.h"
#include "HTTPClient.h"

#define LED 2

const int relayPin_1 = 2;
const int relayPin_2 = 4;
const int relayPin_3 = 12;
const int relayPin_4 = 13;
const int DHT_PIN = 15;
const int pinoLED = 17;
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const String url = "https://api.thingspeak.com/update?api_key=SGMVNNLNIZ8QFSLK&";

HTTPClient p;
DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status());
    Serial.println("Finalizando Setup");
  

  }
  pinMode(LED, OUTPUT);
  pinMode(relayPin_1, OUTPUT);
  pinMode(relayPin_2, OUTPUT);
  pinMode(relayPin_3, OUTPUT);
  pinMode(relayPin_4, OUTPUT);
}

void loop() {
  //conectando ao servidor:
  float temperaturaa = 24.0;
  float umidadee = 30.0;

  String path = url + "field1=" + String(temperaturaa) + "&field2=" + String(umidadee);
  p.begin(path);
  int httpCode = p.GET();
  String payload = p.getString();
  Serial.println("HttpCode:" + String(httpCode));  
  Serial.println("Payload:" + payload);

  
  //capturando temperatura e umidade
  delay(1000);
  float temperatura = dhtSensor.getTemperature();
  float umidade = dhtSensor.getHumidity();

  //imprimindo dados capturados:
  digitalWrite(relayPin_1, HIGH);
  delay(800);
  Serial.println("Temperatura: " + String(temperatura, 2) + "°C");
  Serial.println("---");
  digitalWrite(pinoLED, !digitalRead(pinoLED));
  digitalWrite(relayPin_1, LOW);

  digitalWrite(relayPin_2, HIGH);
  delay(800);
  Serial.println("Umidade: " + String(umidade, 1) + "%");
  Serial.println("---");
  digitalWrite(relayPin_2, LOW);

  digitalWrite(relayPin_3, HIGH);
  delay(800);
  Serial.println("Temperatura: " + String(temperatura, 2) + "°C");
  Serial.println("---");
  digitalWrite(pinoLED, !digitalRead(pinoLED));
  digitalWrite(relayPin_3, LOW);

  digitalWrite(relayPin_4, HIGH);
  delay(800);
  Serial.println("Umidade: " + String(umidade, 1) + "%");
  Serial.println("---");
  digitalWrite(relayPin_4, LOW);  
  
}