#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int LDR_PIN = 34;

// ----- CONFIGURAÇÃO DO WI-FI -----
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ----- CONFIGURAÇÃO DOS BROKERS -----
const char* mqttServer1 = "54.221.163.3"; // Broker privado (MyMQTT)
const int mqttPort1 = 1883;
const char* mqttUser1 = "nicolas";
const char* mqttPassword1 = "banana";

const char* mqttServer2 = "broker.hivemq.com"; // Broker público (para dashboard)
const int mqttPort2 = 1883;

// ----- CLIENTES MQTT -----
WiFiClient espClient1;
WiFiClient espClient2;
PubSubClient client1(espClient1); // Broker MyMQTT
PubSubClient client2(espClient2); // Broker HiveMQ

unsigned long lastMsg = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LDR_PIN, INPUT);

  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.println(WiFi.localIP());

  // Configuração dos brokers
  client1.setServer(mqttServer1, mqttPort1);
  client2.setServer(mqttServer2, mqttPort2);
}

void reconnect(PubSubClient &client, const char* serverName) {
  while (!client.connected()) {
    Serial.print("Conectando ao broker: ");
    Serial.println(serverName);
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado com sucesso!");
    } else {
      Serial.print("Falha (");
      Serial.print(client.state());
      Serial.println("), tentando novamente...");
      delay(2000);
    }
  }
}

void loop() {
  if (!client1.connected()) reconnect(client1, mqttServer1);
  if (!client2.connected()) reconnect(client2, mqttServer2);

  client1.loop();
  client2.loop();

  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();
    int luminosidade = analogRead(LDR_PIN);

    if (isnan(temperatura) || isnan(umidade)) {
      Serial.println("Erro ao ler o sensor DHT!");
      return;
    }

    // Conversão dos dados para string
    String tempStr = String(temperatura, 2);
    String humStr = String(umidade, 2);
    String luzStr = String(luminosidade);

    // Exibe no monitor serial
    Serial.println("----- Dados -----");
    Serial.println("Temperatura: " + tempStr + " °C");
    Serial.println("Umidade: " + humStr + " %");
    Serial.println("Luminosidade: " + luzStr);
    Serial.println("-----------------");

    // Publica no MyMQTT
    client1.publish("esp32/temperatura", tempStr.c_str());
    client1.publish("esp32/umidade", humStr.c_str());
    client1.publish("esp32/luminosidade", luzStr.c_str());

    // Publica no HiveMQ (para o dashboard)
    client2.publish("esp32_dash/temperatura", tempStr.c_str());
    client2.publish("esp32_dash/umidade", humStr.c_str());
    client2.publish("esp32_dash/luminosidade", luzStr.c_str());
  }
}
