#include <Servo.h>
#include <SimpleTimer.h>
#include "commands.h"
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

#define TRIGGER_PIN 46
#define ECHO_PIN 44
#define MAX_DISTANCE 1000

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
LiquidCrystal_I2C lcd(0x27, 16, 2);

SimpleTimer t;
SimpleTimer t1;
SimpleTimer t2;

Servo myservo;  // create servo object to control a servo

const int RECV_PIN = 31;
int pos = 30;    // variable to store the servo position
int counter = 1;
int water;
int distance;

bool autonomous = false;
bool startSensors = false;
bool writeLock = true;
bool printLock = true;

IRrecv irrecv(RECV_PIN);
decode_results results;


void getPing(){
if(startSensors==true){
  distance = sonar.ping_cm();
  Serial.println(distance);
  if(writeLock==true){
  writeLock=false;
  Serial1.print('A');
  Serial1.write(distance);
  writeLock=true;
      }
  }
  if(autonomous==true){ 
      if(distance<=30){
        if(pos>=30 && pos<=75){left();}
        if(pos>=75 && pos<=120){right();}
        }
        else {forward();}
     }
}


void sweep_servo() {
counter=counter+5;
float rad = counter*(3.14159/180);
pos = 45*sin(rad)+75;
myservo.write(pos);              // tell servo to go to position in variable 'pos'
}


void water_sensor(){
if(startSensors==true){
water = analogRead(A5);
if(water>255){water=255;}
    if(writeLock==true){
      writeLock=false;
      Serial1.print('B');
      Serial1.write(water);
      writeLock=true;
    }
  }
if(water>100){
  autonomous = false;
  brakes();
  }
}


void WATER_WARNING(){
if (water>100 && printLock==true){
lcd.print("WARNING: WATER");
printLock=false;
}
else if (water<100) {
  if(printLock==false){
  lcd.clear();
  }
  printLock=true;
}
}

void setup() {
  t.setInterval(25, sweep_servo);
  t1.setInterval(100, getPing);
  t2.setInterval(300, water_sensor);
  Serial.begin(9600);
  Serial1.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  motor_setup();
  myservo.attach(7);  // attaches the servo on pin 9 to the servo object
  lcd.backlight();
  lcd.init();
}




void loop() {
  t.run();
  t1.run();
  t2.run();

 WATER_WARNING();
 PC_COMMAND();
 IR_COMMAND();
}





 void PC_COMMAND(){
   if (Serial1.available()) {  //When data is available in the buffer..
    char command = Serial1.read();  //store value in 'C'
    switch(command){
            
          case '1':
          startSensors=!startSensors;
          break;
          
          case 'w':
          forward();
          break;
  
          case 'a':
          left();
          break;
  
          case 'd':
          right();
          break;
  
          case 's':
          backward();
          break;
        
          case 'b':
          brakes();
          break;  

          case 'n':
          startSensors=true;
          autonomous = !autonomous;
          break;
      }
     }
 }

 void IR_COMMAND(){
  
  if (irrecv.decode(&results)){
    Serial.println(results.value, HEX);
    switch(results.value){
  
      case 0xFFA25D:
      startSensors=!startSensors;
      break;
      
      case 0xFF18E7:  
      forward();
      break;

      case 0xFF10EF:
      left();
      break;

      case 0xFF5AA5:
      right();
      break;

      case 0xFF4AB5:
      backward();
      break;
    
      case 0xFF38C7:
      brakes();
      break;  

      case 0xFF6897:
      startSensors=true;
      autonomous = !autonomous;
      break;
      }
      
  irrecv.resume();
  }
 }
