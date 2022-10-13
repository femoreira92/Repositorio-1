/**
   ESP32 + DHT22 Example for Wokwi
   
   https://wokwi.com/arduino/projects/322410731508073042
*/
#include "WiFi.h"
#include "DHTesp.h"
#include "PubSubClient.h"
#include "Arduino_JSON.h"

#define LED 2

const int relayPin_1 = 2;
const int relayPin_2 = 4;
const int relayPin_3 = 12;
const int relayPin_4 = 13;
const int DHT_PIN = 15;
const int pinoLED = 17;
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const String url = "https://api.thingspeak.com/update?api_key=0M7K9850R6HKN9PQ&";

//configurações do mqtt broker
int port = 1883; //1883
const char* server = "broker.hivemq.com";
const char* topico = "esp32SA3"; //"senai_tec_des"; //testtopic/2

JSONVar data;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando Setup");

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status());
    Serial.println(".");
  }
  Serial.println("Conectado à rede Wifi :)");

//conexão com broker
  Serial.println("Conectando ao Broker");
  mqttClient.setServer(server, port);
  char* clientId = "clientId-G1PBAoOhEG";//"clientID-15ioioi&7778sdfasW23"; //"esp343#edssfsd_pok" //clientId-esfwBrgOga  //clientID-29ioioi&7778sdfasW23

  mqttClient.connect(clientId);
  mqttClient.subscribe(topico);

  while(!mqttClient.connected()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Broker está conectado");  
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  pinMode(LED, OUTPUT);
  pinMode(relayPin_1, OUTPUT);
  pinMode(relayPin_2, OUTPUT);
  pinMode(relayPin_3, OUTPUT);
  pinMode(relayPin_4, OUTPUT);  
}

void loop() {
  const int relayPin_1 = 2;
  const int relayPin_2 = 4;
  const int relayPin_3 = 12;
  const int relayPin_4 = 13;
  const int DHT_PIN = 15;
  const int pinoLED = 17;

  //conectando ao servidor:
  float temperatura = dhtSensor.getTemperature();
  float umidade = dhtSensor.getHumidity();

  data["temperatura"] = temperatura;
  data["umidade"] = umidade;
  String jsonString = JSON.stringify(data);
  if (!temperatura && !umidade){
    Serial.println("problemas ao enviar mensagem!"); 
  } else {
    
    //capturando temperatura e umidade
    delay(1000);
  
    //imprimindo dados capturados:
    digitalWrite(relayPin_1, HIGH);
    delay(800);
    Serial.println("Temperatura: " + String(temperatura, 2) + "°C");
    Serial.println("---");
    digitalWrite(pinoLED, !digitalRead(pinoLED));
    digitalWrite(relayPin_1, LOW);

    digitalWrite(relayPin_2, HIGH);
    delay(800);
    Serial.println("Umidade: " + String(umidade, 4) + "%");
    Serial.println("---");
    digitalWrite(relayPin_2, LOW);

    digitalWrite(relayPin_3, HIGH);
    delay(800);
    Serial.println("Temperatura: " + String(temperatura, 12) + "°C");
    Serial.println("---");
    digitalWrite(pinoLED, !digitalRead(pinoLED));
    digitalWrite(relayPin_3, LOW);

    digitalWrite(relayPin_4, HIGH);
    delay(800);
    Serial.println("Umidade: " + String(umidade, 13) + "%");
    Serial.println("---");
    digitalWrite(relayPin_4, LOW);
  }

  boolean msg = mqttClient.publish(topico, jsonString.c_str());;
  if (!msg) {
    Serial.println("Problemas ao enviar msg");
    mqttClient.connect("clientId-esfwBrgOga");
    mqttClient.subscribe(topico);
  } else {
    Serial.println("Mensagem enviada com sucesso!!!");
  }
  delay(800);
  
}