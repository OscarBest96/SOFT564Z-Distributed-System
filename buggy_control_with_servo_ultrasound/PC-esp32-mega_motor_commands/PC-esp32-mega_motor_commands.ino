#include <BlynkSimpleEsp32.h>
#include <SimpleTimer.h>
#include "WiFi.h" // ESP32 WiFi include
#include "wifiConfig.h" // My WiFi configuration. 
#include "DHT.h"


#define BLYNK_PRINT Serial
#define DHTPIN 23        // what digital pin we're connected to
#define DHTTYPE DHT11     // DHT11
DHT dht(DHTPIN, DHTTYPE);


//192.168.0.77  <--- IP

#define RXD2 16 //these are the ports responisble for serial port 2
#define TXD2 17
 
WiFiServer wifiServer(80);
WiFiClient client;

SimpleTimer T;
SimpleTimer H;

bool startSensors = false;


void read_temperature(){
 if(startSensors==true){
  int temp = dht.readTemperature();
  Serial.println(temp);
  Blynk.virtualWrite(V1,temp);
  }
}
void read_humidity(){
  if(startSensors==true){
  int humidity = dht.readHumidity();
  Blynk.virtualWrite(V2,humidity); 
  }
}





void setup() {

  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // starts serial port 2, wired connection to arduino mega.
  delay(1000);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
 
  wifiServer.begin();
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  T.setInterval(5000, read_temperature);
  H.setInterval(5000, read_humidity);
}
 
void loop() {
  client = wifiServer.available();
  Blynk.run();
 
  if (client) {

      while (client.connected()) {
          T.run();
          H.run();
             
                  
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
if(command=='1'){
  startSensors=!startSensors;
  Serial.println(startSensors);
}
Serial2.write(command); //write data to serial every second
}

void serial2Event(){
char ID = Serial2.read();  //store value in 'C'
if (ID=='A'){
 //   if(Serial2.available()){
     int distance = Serial2.read();
     Blynk.virtualWrite(V0,distance);
 //   }
   }
    else if(ID=='B'){
 //    if(Serial2.available()){
      int water = Serial2.read();
     // Serial.println(water);
      Blynk.virtualWrite(V3,water);
      
  //   }
    }
}
