#include <BlynkSimpleEsp32.h>
#include <SimpleTimer.h>
#include "WiFi.h" // ESP32 WiFi include
#include "wifiConfig.h" // My WiFi configuration. 
#include "DHT.h"
#include <IRremote.h>


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
const int RECV_PIN = 18;
char command;

IRrecv irrecv(RECV_PIN);
decode_results results;


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
  irrecv.enableIRIn();
  irrecv.blink13(true);
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
  T.run();
  H.run();
  IR_COMMAND();
if (Serial2.available()) {
serial2Event();
}
if(command=='1'){
startSensors=!startSensors;
}
 
  if (client) {
 //Serial.println ("YOOOO!");


      while (client.connected()) {
        
             IR_COMMAND();
                  
             if (client.available()) {
             clientEvent();
             }

       }
      
      client.stop();
      Serial.println("Client disconnected");
      }
}

void clientEvent(){
command = client.read();
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

 void IR_COMMAND(){
  
  if (irrecv.decode(&results)){
     Serial.println(results.value, HEX);
     switch(results.value){
  
      case 0xFFA25D:
      Serial2.write('1');
      command = '1';
      break;
      
      case 0xFF18E7:  
      Serial2.write('w');
      break;

      case 0xFF10EF:
      Serial2.write('a');
      break;

      case 0xFF5AA5:
      Serial2.write('d');
      break;

      case 0xFF4AB5:
      Serial2.write('s');
      break;
    
      case 0xFF38C7:
      Serial2.write('b');
      break;  

      case 0xFF6897:
      Serial2.write('n');
      break;
      }
    irrecv.resume();
  }
 }
