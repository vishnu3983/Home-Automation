#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Redmi";
const char* password = "pass1111";
const char* mqtt_server = "broker.mqtt-dashboard.com";
char bufferMsg[100];
int switch1 = 5;   //D1
int device1 = 16;    //D0

int switch2 = 14;   //D5
int device2 = 4;    //D2

int switch1Status;
int device1Status;
int switch2Status;
int device2Status;
const char* topic1 = "groundFloor/hall/light1";
const char* topic2 = "groundFloor/hall/fan";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  StaticJsonDocument<256> doc;
  deserializeJson(doc, payload, length);
  
  Serial.println();

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  if (strcmp(topic,topic1) ==0 ){
    int receivedStatus = doc["status"];
    if (receivedStatus != device1Status){
      digitalWrite(device1, receivedStatus);
      Serial.print(receivedStatus);
      device1Status = receivedStatus;
    }
  }
  else if (strcmp(topic,topic2) ==0 ){
    int receivedStatus = doc["status"];
    if (receivedStatus != device2Status){
      digitalWrite(device2, receivedStatus);
      Serial.print(receivedStatus);
      device2Status = receivedStatus;
    }
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = topic1;
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(topic1);
      client.subscribe(topic2);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(switch1, INPUT);
  pinMode(device1, OUTPUT);
  pinMode(switch2, INPUT);
  pinMode(device2, OUTPUT);
  Serial.begin(115200);
  switch1Status = 0;
  device1Status = 0;
  switch1Status = 0;
  device2Status = 0;
  digitalWrite(device1, LOW);
  digitalWrite(device2, LOW);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  StaticJsonDocument<200> doc;
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int newswitch1Status = digitalRead(switch1);
  int newswitch2Status = digitalRead(switch2);
  
  if (newswitch1Status != switch1Status)
  {
    switch1Status = newswitch1Status;
    device1Status = !(device1Status);
    digitalWrite(device1, device1Status);
    doc["name"] = "light1";
    doc["topic"] = topic1;
    doc["status"] = device1Status;
    size_t n = serializeJson(doc, bufferMsg);
    client.publish(topic1, bufferMsg);
  }
  else if (newswitch2Status != switch2Status)
  {
    switch2Status = newswitch2Status;
    device2Status = !(device2Status);
    digitalWrite(device2, device2Status);
    doc["name"] = "fan";
    doc["topic"] = topic2;
    doc["status"] = device2Status;
    size_t n = serializeJson(doc, bufferMsg);
    client.publish(topic2, bufferMsg);
  }
  delay(100);

}
