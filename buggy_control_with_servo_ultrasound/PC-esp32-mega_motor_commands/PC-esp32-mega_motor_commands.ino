#include <SimpleTimer.h>

#include "WiFi.h" // ESP32 WiFi include
#include "wifiConfig.h" // My WiFi configuration. 


//192.168.0.77  <--- IP

#define RXD2 16 //these are the ports responisble for serial port 2
#define TXD2 17
 
WiFiServer wifiServer(80);
WiFiClient client;

void setup() {

  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // starts serial port 2, wired connection to arduino mega.
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
 
  wifiServer.begin();
}
 
void loop() {
  client = wifiServer.available();
 
  if (client) {

      while (client.connected()) {
                  
             if (client.available()) {
             clientEvent();
             }
             
             if (Serial2.available()) {
              serial2Event();
             }
       }
      
      client.stop();
      Serial.println("Client disconnected");
      }
}

void clientEvent(){
char command = client.read();
Serial2.write(command); //write data to serial every second
}

void serial2Event(){
int value = Serial2.read();
Serial.println(value);
}
