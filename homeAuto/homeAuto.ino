#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
// Replace with your network credentials
const char* ssid = "Redmi";
const char* password = "pass1111";
 
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
 
String page = "";
int button = 2;
int LEDPin = 5;
int buttonStatus;

void setup(void){
  //the HTML of the web page
  page = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";
  //make the LED pin output and initially turned off
  pinMode(LEDPin, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(LEDPin, LOW);
  buttonStatus = digitalRead(button);
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
  server.on("/", [](){
    server.send(200, "text/html", page);
  });
  server.on("/LEDOn", [](){
    server.send(200, "text/html", page);
    digitalWrite(LEDPin, HIGH);
    Serial.println("LedOn");
    buttonStatus = 1;
    delay(1000);
  });
  server.on("/LEDOff", [](){
    server.send(200, "text/html", page);
    digitalWrite(LEDPin, LOW);
    Serial.println("LedOff");
    buttonStatus = 0;
    delay(1000); 
  });
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  /*
  if (digitalRead(button)== HIGH)
  {
    delay(10);
    if ((digitalRead(button)== HIGH) && (buttonStatus == LOW))
    {
      Serial.println("Button High");
      digitalWrite(LEDPin, HIGH);
      buttonStatus = HIGH;
    }
    else if ((digitalRead(button)== HIGH) && (buttonStatus == HIGH))
    {
      Serial.println("Button Low");
      digitalWrite(LEDPin, LOW);
      buttonStatus = LOW;
    }
  }
  */
  if ((digitalRead(button)== HIGH) && (buttonStatus == LOW))
    {
      Serial.println("Button High");
      digitalWrite(LEDPin, HIGH);
      buttonStatus = HIGH;
      delay(1000);
    }
    else if ((digitalRead(button)== HIGH) && (buttonStatus == HIGH))
    {
      Serial.println("Button Low");
      digitalWrite(LEDPin, LOW);
      buttonStatus = LOW;
      delay(1000);
    }
  server.handleClient();
}
