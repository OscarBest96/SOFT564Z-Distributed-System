#include <Servo.h>
#include <SimpleTimer.h>
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
 
#define TRIGGER_PIN 46
#define ECHO_PIN 44
#define MAX_DISTANCE 1000
#define SPEED 150
#define TURNSPEED 255

SimpleTimer t;
SimpleTimer t1;
SimpleTimer t2;


Servo myservo;  // create servo object to control a servo


int pos = 30;    // variable to store the servo position
int counter = 1;
bool autonomous = false;
bool printLock = true;



NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 


int getPing(){
  int distance = sonar.ping_cm();
  if(autonomous==true){ 
      if(distance<=40){
        
        if(pos>=30 && pos<=75){
          //motor A: FORWARD
            digitalWrite(12, LOW); //Establishes forward direction of Channel A
            digitalWrite(9, LOW);   //Disengage the Brake for Channel A
            analogWrite(3, TURNSPEED);   //Spins the motor on Channel A at full speed
            //motor B: BREAK
            digitalWrite(13, LOW); //Establishes forward direction of Channel A
            digitalWrite(8, LOW);   //Disengage the Brake for Channel A
            analogWrite(11, TURNSPEED);   //Spins the motor on Channel A at full speed
          }
            
           if(pos>=75 && pos<=120){
            digitalWrite(12, HIGH); //Establishes forward direction of Channel A
            digitalWrite(9, LOW);   //Disengage the Brake for Channel A
            analogWrite(3, TURNSPEED);   //Spins the motor on Channel A at full speed
            //motor B: FORWARD
            digitalWrite(13, HIGH); //Establishes forward direction of Channel A
            digitalWrite(8, LOW);   //Disengage the Brake for Channel A
            analogWrite(11, TURNSPEED);   //Spins the motor on Channel A at full speed
            }
          }
     else {
            digitalWrite(12, LOW); //Establishes forward direction of Channel A
            digitalWrite(9, LOW);   //Disengage the Brake for Channel A
            analogWrite(3, SPEED);   //Spins the motor on Channel A at full speed
            //motor B: FORWARD
            digitalWrite(13, HIGH); //Establishes forward direction of Channel A
            digitalWrite(8, LOW);   //Disengage the Brake for Channel A
            analogWrite(11, SPEED);   //Spins the motor on Channel A at full speed
            
            }
            
  }
  return distance;
}


void sweep_servo() {
counter=counter+5;
float rad = counter*(3.14159/180);
pos = 45*sin(rad)+75;
myservo.write(pos);              // tell servo to go to position in variable 'pos'
}


int water_sensor(){
int water = analogRead(A5);
if(water>100){
  autonomous = false;
  digitalWrite(9, HIGH);   //Disengage the Brake for Channel A
  digitalWrite(8, HIGH);   //Disengage the Brake for Channel A
  Serial.println("i'm here!");
}
return water;

 
}

void setup() {
  
 
  t.setInterval(25, sweep_servo);
  t1.setInterval(100, getPing);
  t2.setInterval(200, water_sensor);


  Serial.begin(9600);
  Serial1.begin(9600);
  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel B pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel B pin
  myservo.attach(7);  // attaches the servo on pin 9 to the servo object
  lcd.backlight();
  lcd.init();
}



void loop() {
  t.run();
  t1.run();
  t2.run();

  int distance = getPing();
  int water = water_sensor();
 // Serial.print(distance);
 // Serial.println("cm"); 
 //  Serial.println(water);
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
 // Serial.println(pos); 
 
   
 

 if (Serial1.available()) {  //When data is available in the buffer..
    char command = Serial1.read();  //store value in 'C'
    Serial.print(command);       //print recieved data to terminal
    
    
          switch(command){
            
          
                case 'w':
                //motor A: FORWARD
                digitalWrite(12, LOW); //Establishes forward direction of Channel A
                digitalWrite(9, LOW);   //Disengage the Brake for Channel A
                analogWrite(3, SPEED);   //Spins the motor on Channel A at full speed
                //motor B: FORWARD
                digitalWrite(13, HIGH); //Establishes forward direction of Channel A
                digitalWrite(8, LOW);   //Disengage the Brake for Channel A
                analogWrite(11, SPEED);   //Spins the motor on Channel A at full speed
                break;
        
                case 'a':
                //motor A: FORWARD
                digitalWrite(12, LOW); //Establishes forward direction of Channel A
                digitalWrite(9, LOW);   //Disengage the Brake for Channel A
                analogWrite(3, TURNSPEED);   //Spins the motor on Channel A at full speed
                //motor B: BREAK
                digitalWrite(13, LOW); //Establishes forward direction of Channel A
                digitalWrite(8, LOW);   //Disengage the Brake for Channel A
                analogWrite(11, TURNSPEED);   //Spins the motor on Channel A at full speed
                break;
        
                case 'd':
                //motor A: B
                digitalWrite(12, HIGH); //Establishes forward direction of Channel A
                digitalWrite(9, LOW);   //Disengage the Brake for Channel A
                analogWrite(3, TURNSPEED);   //Spins the motor on Channel A at full speed
                //motor B: FORWARD
                digitalWrite(13, HIGH); //Establishes forward direction of Channel A
                digitalWrite(8, LOW);   //Disengage the Brake for Channel A
                analogWrite(11, TURNSPEED);   //Spins the motor on Channel A at full speed
                break;
        
                case 's':
                //motor A: BACKWARD
                digitalWrite(12, HIGH); //Establishes forward direction of Channel A
                digitalWrite(9, LOW);   //Disengage the Brake for Channel A
                analogWrite(3, SPEED);   //Spins the motor on Channel A at full speed
                //motor B: BACKWARD
                digitalWrite(13, LOW); //Establishes forward direction of Channel A
                digitalWrite(8, LOW);   //Disengage the Brake for Channel A
                analogWrite(11, SPEED);   //Spins the motor on Channel A at full speed
                break;
              
                //BOTH BRAKE
                case 'b':
                digitalWrite(9, HIGH);   //Disengage the Brake for Channel A
                digitalWrite(8, HIGH);   //Disengage the Brake for Channel A
                break;  

                case '1':
                autonomous = !autonomous;
                break;

                
                }

       }
  

}
