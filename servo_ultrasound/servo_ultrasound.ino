#include <Servo.h>
Servo myservo;
const int pingPin = 2; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 3; // Echo Pin of Ultrasonic Sensor

int pos = 0; 

long duration, cm;

void ultrasound() {
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm = (duration/2)*0.034;
   Serial.print(cm);
   Serial.println("cm");
}

void setup() {
   Serial.begin(9600); // Starting Serial Terminal
   pinMode(pingPin, OUTPUT);
   pinMode(echoPin, INPUT);
   myservo.attach(11);

}

void loop() {

  for (pos = 30; pos <= 120; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    ultrasound();
    delay(30);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 120; pos >= 30; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    ultrasound();
    delay(30);                       // waits 15ms for the servo to reach the position
  }
   

 
}
