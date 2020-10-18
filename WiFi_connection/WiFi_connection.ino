#include "WiFi.h" // ESP32 WiFi include
#include "wifiConfig.h" // My WiFi configuration.
 
void ConnectToWiFi()
{
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WiFiPassword);
  Serial.print("Connecting to "); Serial.println(SSID);
 
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
 
    if ((++i % 16) == 0)
    {
      Serial.println(F(" still trying to connect"));
    }
  }
 
  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());
}




void setup() {

 Serial.begin(9600);
 
 ConnectToWiFi();

}

void loop() {
  // put your main code here, to run repeatedly:

}
