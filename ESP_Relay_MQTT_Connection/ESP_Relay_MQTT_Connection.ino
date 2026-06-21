#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

const char* ssid = "MEKATRONIK UBL"; 
const char* password = "MEKATRONIK123";

const char* mqtt_server = "ycdb2f7b.ala.dedicated.aws.emqxcloud.com";
const int mqtt_port = 8883;
const char* mqtt_user = "esp32_kipas";
const char* mqtt_pass = "Esp32Kipas123!";

const char* topic_control = "kipas/kontrol";
const char* topic_status  = "kipas/status";

const int relayPin = 27;

WiFiClientSecure espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];

  Serial.print("Pesan masuk dari topic [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  if (msg == "ON") {
    digitalWrite(relayPin, LOW);
    client.publish(topic_status, "ON");
    Serial.println("Relay diaktifkan (kipas ON)");
  } else if (msg == "OFF") {
    digitalWrite(relayPin, HIGH);
    client.publish(topic_status, "OFF");
    Serial.println("Relay dimatikan (kipas OFF)");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT broker... ");
    if (client.connect("ESP32Kipas", mqtt_user, mqtt_pass)) {
      Serial.println("TERHUBUNG ke MQTT broker!");
      client.subscribe(topic_control);
      Serial.print("Subscribe ke topic: ");
      Serial.println(topic_control);
    } else {
      Serial.print("GAGAL, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi 2 detik...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("=== ESP32 Kontrol Kipas - Mulai ===");

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  Serial.print("Menghubungkan ke WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi TERHUBUNG!");
  Serial.print("IP Address ESP32: ");
  Serial.println(WiFi.localIP());

  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}