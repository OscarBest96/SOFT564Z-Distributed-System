/*
3rd party libraries used:
<BlynkSimpleEsp32.h> //lib for Blynk app
<SimpleTimer.h>      //lib for timer 
"WiFi.h"             //ESP32 WiFi include
"DHT.h"              //lib for DHT sensor
<IRremote.h>         //lib for IR receiver

Links to these libraries are included in the reference section of the documentation.
*/

#include <BlynkSimpleEsp32.h> //lib for Blynk app
#include <SimpleTimer.h>      //lib for timer 
#include "WiFi.h"             //ESP32 WiFi include
#include "wifiConfig.h"       //my WiFi configuration 
#include "DHT.h"              //lib for DHT sensor
#include <IRremote.h>         //lib for IR receiver


#define BLYNK_PRINT Serial //define serial comunication for Blynk
#define DHTPIN 23          // what digital pin DHT sensor is connected to
#define DHTTYPE DHT11      // DHT11
DHT dht(DHTPIN, DHTTYPE);  //create object dht


//192.168.0.77  <--- IP adress for esp32 on my network 

#define RXD2 16 //these are the ports responsible for serial port 2
#define TXD2 17
 
WiFiServer wifiServer(80); //server object listening to port 80
WiFiClient client;         //client object created 

SimpleTimer T;     //init timer T
SimpleTimer H;     //init timer H

bool startSensors = false;  //init sensors as OFF
const int RECV_PIN = 18;    //receive pin for IR
char command;               //init variable command

IRrecv irrecv(RECV_PIN);  //object irrecv at pin 18 
decode_results results;   //object for IR results

//function to measure temperature
void read_temperature(){

 if(startSensors==true){            //if sensors are turned on
  int temp = dht.readTemperature(); //take temperature reading and save in variable 'temp'
  Blynk.virtualWrite(V1,temp);      //write temp value to Blynk app - pin V1
  }
}
//function to measure humidity
void read_humidity(){
  if(startSensors==true){            //if sensors are turned on
  int humidity = dht.readHumidity(); //take humidity reading and save in variable 'humidity'
  Blynk.virtualWrite(V2,humidity);   //write humidity value to blynk app - pin V2
  }
}





void setup() {

  Serial.begin(9600); //start serial with baud rate 9600
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // starts serial port 2, wired connection to arduino mega.
  T.setInterval(5000, read_temperature); // call read_temperature every 5 seconds
  H.setInterval(5000, read_humidity);    // call read_humidity every 5 seconds
  irrecv.enableIRIn();      //init IR reciever input 
  irrecv.blink13(true);
  delay(1000);
  WiFi.begin(ssid, pass); //init WiFi connection using login provided in wifiConfig.h file
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");  //print this every second while connecting
  }
  Serial.println("Connected to the WiFi network"); //print this once connection is made
  Serial.println(WiFi.localIP()); //print the IP address for esp32 server
 
  wifiServer.begin(); //start server
  Blynk.begin(auth, ssid, pass); //initialise Blynk connection to esp32
  dht.begin(); //begin dht sensor measure 
}
 
void loop() {
  client = wifiServer.available(); //store wifiServer availability into client object
  Blynk.run(); //run Blynk
  T.run();     //run timers T,H
  H.run();
  IR_COMMAND();      //run function IR_COMMAND
  if(command=='1'){  //if command veriable has value 1
  startSensors=!startSensors; //toggle startSensors - ON/OFF
  if (Serial2.available()) {  //if data is at rx of serial2
  serial2Event();             // run function serial2Event

}
}
 
  if (client) {  //if wifi server is available 


      while (client.connected()) { //when connected to the server

             T.run(); //run timers T,H
             H.run();
             IR_COMMAND(); //run function IR_command
                  
             if (client.available()) { //if data is available for client to read
             clientEvent();            //run function clientEvent
             }

             if (Serial2.available()) { //if data is received at rx of serial2
             serial2Event();            //run function serial2Event
             }

       }
      
      client.stop();                         //disconnect from server
      Serial.println("Client disconnected"); //print disconnect message to terminal
      }
}

//function to read data from client and store into variable 'command'
void clientEvent(){
command = client.read(); //data from client stored in command variable
Serial2.write(command); //write command variable data to tx line of serial2
if(command=='1'){       //if command received ==1,  
  startSensors=!startSensors; //toggle sensors on esp32
}
}

//function takes unique char, followed by data & stores into corresponding variables
void serial2Event(){
char ID = Serial2.read();  //read data at rx or serial2 and store into variable ID
if (ID=='A'){              //if ID == A
     int distance = Serial2.read();   //store the next byte received into variable 'distance'
     Blynk.virtualWrite(V0,distance); //write distance value to Blynk app pin V0 
 //   }
   }
    else if(ID=='B'){      //if ID == B
      int water = Serial2.read(); //store the next byte into variable 'water'
      Blynk.virtualWrite(V3,water); //write water value to Blynk app pin V3
      
  //   }
    }
}

//Function reads result from IR transmitter, compares the result, and writes to serial2
//values written to serial2 will be used by the arduino in switch-case to drive the motors
 void IR_COMMAND(){
  
  if (irrecv.decode(&results)){          //if IR code is received 
     Serial.println(results.value, HEX); //convert code to HEX value
     switch(results.value){              //evaluate value
  
      case 0xFFA25D:              //code 0xFFA25D (1 on IR remote) writes '1' to serial2 and stores value '1' in variable command
      Serial2.write('1');
      command = '1';
      break;
      
      case 0xFF18E7:              //code 0xFF18E7 (up arrow on IR remote) writes 'w' to serial2
      Serial2.write('w');
      break;

      case 0xFF10EF:              //code 0xFF10EF (left arrow on IR remote) writes 'a' to serial2
      Serial2.write('a');
      break;

      case 0xFF5AA5:              //code 0xFF5AA5 (right arrow on IR remote) writes 'd' to serial2
      Serial2.write('d');
      break;

      case 0xFF4AB5:              //code 0xFF4AB5 (down arrow on IR remote) writes 's' to serial2
      Serial2.write('s');
      break;
    
      case 0xFF38C7:              //code 0xFF38C7 (OK on IR remote) writes 'b' to serial2
      Serial2.write('b');
      break;  

      case 0xFF6897:              //code 0xFF6897 (* on IR remote) writes 'n' to serial2
      Serial2.write('n');
      break;
      }
    irrecv.resume();              //exit result decoding and resume function
  }
 }
