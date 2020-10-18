#include "WiFi.h" // ESP32 WiFi include
#include "wifiConfig.h" // My WiFi configuration. 
 
WiFiServer wifiServer(80);
int x = 100;
void setup() {
 
  Serial.begin(9600);
 
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
 
  WiFiClient client = wifiServer.available();
 
  if (client) {
 
    while (client.connected()) {

 /*
      while (client.available()>0) {
        char c = client.read();
        Serial.write(c);
      }
 
     */
     client.println(x,DEC);
   
      delay(1000);
    }
 
    client.stop();
    Serial.println("Client disconnected");
 
  }
}
