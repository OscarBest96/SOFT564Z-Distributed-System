#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

const int pingPin = 2; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 4; // Echo Pin of Ultrasonic Sensor

char auth[] = "XbTdf0QfC_izQS0pDoo7gIOC5ytBVY4P";       // You should get Auth Token in the Blynk App.
char ssid[] = "SKY02082";                    // Your Wi-Fi Credentials
char pass[] = "FTDVNWCMMP";

long duration, cm; 

void setup() {  

  Serial.begin(9600); // Starting Serial Terminal
  pinMode(pingPin, OUTPUT); //init pin 2 as output
  pinMode(echoPin, INPUT); //init pin 4 as input
  Blynk.begin(auth, ssid, pass);


  
}
void loop(){
  
   Blynk.run();

       
   
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   
   duration = pulseIn(echoPin, HIGH);
   
   cm = (duration/2)*0.034;

   Blynk.virtualWrite(V0,cm);
   Serial.print(cm);
   Serial.println("cm");

}
