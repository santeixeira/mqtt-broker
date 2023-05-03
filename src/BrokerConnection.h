#ifndef BrokerConnection
#define BrokerConnection

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

bool mqttStatus = 0;

const char *MQTT_BROKER = "test.mosquitto.org";
const int MQTT_PORT = 1883;
const char *MQTT_CLIENTID = "teste";
const char *MQTT_USERNAME = "";
const char *MQTT_PASSWORD = "";

//    inserir nome e senha da rede    //
const char *ssid = "ALHN-58A0";
const char *password = "linda123";

WiFiClient esp8266Client;
PubSubClient client(esp8266Client);

bool connectBroker(char *TITULO_TOPICO, float VALOR_TOPICO);
void loopBroker(bool mqttStatus, const char *TITULO_TOPICO, float VALOR_TOPICO);
void publishTopic(const char *topic, float measure);
void callback(char *topic, byte *payload, unsigned int length);
void connectWifi();
void reconnectWifi();

bool connectBroker(const char *TITULO_TOPICO, float VALOR_TOPICO)
{
  connectWifi();
  byte counter = 0; // Numero de tentativas de Conexão
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);
  do
  {
    String client_id = "LCF-" + String(WiFi.macAddress());
    if (client.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD))
    {
      digitalWrite(16, 1);
      Serial.printf("\nCliente %s foi conectado ao Broker\n", client_id.c_str());
    }
    else
    {
      Serial.printf("\nFalha ao conectar: %d", client.state());
      Serial.printf("\nTentativa: %d", counter);
      delay(3000);
    }
    counter++;
  } while (!client.connected() && counter < 5);

  if (counter > 5)
  {
    Serial.print("Não conectado.");
    return 0;
  }
  publishTopic(TITULO_TOPICO, VALOR_TOPICO);
  return 1;
}

void loopBroker(bool mqttStatus, const char *TITULO_TOPICO, float VALOR_TOPICO) {
  static long long pooling = 0;
  if (mqttStatus)
  {
    client.loop();
    if (millis() > pooling + 1000)
    {
      pooling = millis();
      publishTopic(TITULO_TOPICO, VALOR_TOPICO);
    }
  }

  if (!client.connected())
  {
    reconnectWifi();
  }
}

void connectWifi()
{
  Serial.print("Conectando ao WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.printf(".");
  }
  Serial.print("\nConectado a Rede WiFi de IP: ");
  Serial.print(WiFi.localIP());
}

void reconnectWifi()
{
  while (!client.connected())
  {
    client.connect(MQTT_CLIENTID);
  }
}

void publishTopic(const char *topic, float measure)
{
  delay(1000);
  client.publish(topic, String(measure).c_str(), true);
  client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.printf("\nMensagem recebida pelo topico: %s", topic);
  Serial.print(" \nMessage:");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
}

#endif