/*
* Reading MD300C29 sensor ans sending over wifi to mqtt server
* 
* Author: Goran Mahovlic
*/

#include <WiFi.h>
#include <PubSubClient.h>

#define RXD2 16
#define TXD2 17

// Replace the next variables with your SSID/Password combination
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";


// Add your MQTT Broker IP address, example:
const char* mqtt_server = "YOUR_MQTT_SERVER";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
 
int counter = 0;
char bytes[200] = { '\0' };
boolean startlog = 0;
uint8_t spo2 = 0;
uint8_t pulse = 0;
String med_data;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("cm");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);    //If you want to use callback
  client.publish("cm/bytes", bytes); 
 
}
 
void loop() { //Choose Serial1 or Serial2 as required

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

while (!startlog){
  if (Serial2.available() > 0){  
    bytes[0] =  char(Serial2.read());
    if(bytes[0] == 255){
      counter++;
      startlog = 1;
      }
    }
}

  if (Serial2.available() > 0 && startlog) {
    if (counter < 17){
      bytes[counter] =  char(Serial2.read());
      counter++;
      }
    else{
      if(bytes[0] == 255 && bytes[1] == 255){
          Serial.print("SpO2: "); Serial.print(uint8_t(bytes[15])); Serial.print("  ");
          Serial.print("Pulse: "); Serial.print(uint8_t(bytes[16])); Serial.print("  ");
          Serial.println("uT");      
          spo2 = uint8_t(bytes[15]);
          pulse = uint8_t(bytes[16]);
          counter=0;
        }
      else{
            startlog = 0;
            counter = 0;
          }
      }
  }


  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    char CM_data[3];
    CM_data[0] = bytes[15];
    CM_data[1] = bytes[16];
    CM_data[2] = '\0';
    if(bytes[15] > 0 && bytes[16] > 0){
      client.publish("cm", CM_data);
      Serial.println("Send");
    }
  }
}
