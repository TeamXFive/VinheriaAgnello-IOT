#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// CONFIGURAÇÕES DOS SENSORES

#define DHT_PIN 15
#define DHT_TYPE DHT22

#define LDR_PIN 34

DHT dht(DHT_PIN, DHT_TYPE);


// CONFIGURAÇÕES WI-FI

// Wi-Fi padrão utilizado pelo Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";


// CONFIGURAÇÕES MQTT

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;


// Tópicos utilizados pela aplicação

const char* topicTemperatura =
  "Fiap/iot/3ESOA-2026/xfive/temperatura";

const char* topicUmidade =
  "Fiap/iot/3ESOA-2026/xfive/umidade";

const char* topicLuminosidade =
  "Fiap/iot/3ESOA-2026/xfive/luminosidade";

const char* topicSensores =
  "Fiap/iot/3ESOA-2026/xfive/todos";


// OBJETOS DE CONEXÃO

WiFiClient espClient;
PubSubClient mqttClient(espClient);


// CONTROLE DO INTERVALO DE LEITURA

unsigned long ultimaLeitura = 0;

const unsigned long intervaloLeitura = 4000;


// CONEXÃO WI-FI

void conectarWiFi() {

  Serial.print("Conectando ao Wi-Fi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

    Serial.println(WiFi.status());
  }

  Serial.println();

  Serial.println("Wi-Fi conectado!");

  Serial.print("IP: ");

  Serial.println(WiFi.localIP());
}


// CONEXÃO COM O BROKER MQTT

void conectarMQTT() {

  while (!mqttClient.connected()) {

    Serial.print("Conectando ao broker MQTT...");

    // ID aleatório para evitar conflito entre diferentes ESP32
    String clientId = "VinheriaAgnello-";

    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str())) {

      Serial.println(" conectado!");

    } else {

      Serial.print(" erro: ");

      Serial.println(mqttClient.state());

      delay(2000);
    }
  }
}


// SETUP

void setup() {

  Serial.begin(115200);

  dht.begin();

  pinMode(LDR_PIN, INPUT);

  conectarWiFi();

  mqttClient.setServer(mqtt_server, mqtt_port);

  Serial.println("Sistema Vinheria Agnello iniciado.");
}


// LOOP PRINCIPAL

int contador = 0;

void loop() {

  if (!mqttClient.connected()) {

    conectarMQTT();
  }

  mqttClient.loop();


  unsigned long tempoAtual = millis();


  // Executa leitura a cada 4000 ms

  contador = contador + 1;

  if (tempoAtual - ultimaLeitura >= intervaloLeitura) {

    Serial.println(contador);
    contador = 0;

    ultimaLeitura = tempoAtual;


    // LEITURA DO DHT22

    float temperatura = dht.readTemperature();

    float umidade = dht.readHumidity();


    // LEITURA DO LDR

    int valorLDR = analogRead(LDR_PIN);


    // Converte leitura analógica para percentual

    int luminosidade = map(
      valorLDR,
      4095,
      0,
      0,
      100
    );


    // VALIDAÇÃO DO DHT22

    if (isnan(temperatura) || isnan(umidade)) {

      Serial.println("Erro ao ler DHT22");

      return;
    }


    // JSON PARA SERIAL E MQTT

    String json = "{";

    json += "\"temperatura\":";
    json += String(temperatura, 1);

    json += ",";

    json += "\"umidade\":";
    json += String(umidade, 1);

    json += ",";

    json += "\"luminosidade\":";
    json += String(luminosidade);

    json += "}";


    // Exibe JSON na porta serial

    Serial.println(json);


    // PUBLICAÇÃO MQTT

    mqttClient.publish(
      topicTemperatura,
      String("{\"temperatura\": " +  String(temperatura, 1) + "}").c_str()
    );

    mqttClient.publish(
      topicUmidade,
      String("{\"umidade\": " +  String(umidade, 1) + "}").c_str()
    );

    mqttClient.publish(
      topicLuminosidade,
      String("{\"luminosidade\": " +  String(luminosidade) + "}").c_str()
    );

    mqttClient.publish(
      topicSensores,
      json.c_str()
    );
  }
}