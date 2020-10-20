#include <Servo.h>
#include <SimpleTimer.h>
#include <NewPing.h>
 
#define TRIGGER_PIN 46
#define ECHO_PIN 44
#define MAX_DISTANCE 200

SimpleTimer t;
SimpleTimer t1;

Servo myservo;  // create servo object to control a servo



int pos = 30;    // variable to store the servo position
int counter = 1;


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 


int getPing(){
  int distance = sonar.ping_cm();
  return distance;
}


void sweep_servo() {
counter++;
float rad = counter*(3.14159/180);
pos = 45*sin(rad)+75;
myservo.write(pos);              // tell servo to go to position in variable 'pos'
}


void setup() {
  
 
  t.setInterval(20, sweep_servo);
  t1.setInterval(200, getPing);
  Serial.begin(9600);
  Serial1.begin(9600);
  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel B pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel B pin
  myservo.attach(7);  // attaches the servo on pin 9 to the servo object
}



void loop() {
  
  t.run();
  int distance = getPing();
  Serial.print(distance);
  Serial.println("cm"); 
 
   
 

 if (Serial1.available()) {  //When data is available in the buffer..
    char command = Serial1.read();  //store value in 'C'
    Serial.print(command);       //print recieved data to terminal
    
    
          switch(command){
            
          
                case 'w':
                //motor A: FORWARD
                digitalWrite(12, LOW); //Establishes forward direction of Channel A
                digitalWrite(9, LOW);   //Disengage the Brake for Channel A
                analogWrite(3, 200);   //Spins the motor on Channel A at full speed
                //motor B: FORWARD
                digitalWrite(13, HIGH); //Establishes forward direction of Channel A
                digitalWrite(8, LOW);   //Disengage the Brake for Channel A
                analogWrite(11, 200);   //Spins the motor on Channel A at full speed
                break;
        
                case 'a':
                //motor A: FORWARD
                digitalWrite(12, LOW); //Establishes forward direction of Channel A
                digitalWrite(9, LOW);   //Disengage the Brake for Channel A
                analogWrite(3, 255);   //Spins the motor on Channel A at full speed
                //motor B: BREAK
                digitalWrite(13, LOW); //Establishes forward direction of Channel A
                digitalWrite(8, LOW);   //Disengage the Brake for Channel A
                analogWrite(11, 200);   //Spins the motor on Channel A at full speed
                break;
        
                case 'd':
                //motor A: B
                digitalWrite(12, HIGH); //Establishes forward direction of Channel A
                digitalWrite(9, LOW);   //Disengage the Brake for Channel A
                analogWrite(3, 200);   //Spins the motor on Channel A at full speed
                //motor B: FORWARD
                digitalWrite(13, HIGH); //Establishes forward direction of Channel A
                digitalWrite(8, LOW);   //Disengage the Brake for Channel A
                analogWrite(11, 255);   //Spins the motor on Channel A at full speed
                break;
        
                case 's':
                //motor A: BACKWARD
                digitalWrite(12, HIGH); //Establishes forward direction of Channel A
                digitalWrite(9, LOW);   //Disengage the Brake for Channel A
                analogWrite(3, 200);   //Spins the motor on Channel A at full speed
                //motor B: BACKWARD
                digitalWrite(13, LOW); //Establishes forward direction of Channel A
                digitalWrite(8, LOW);   //Disengage the Brake for Channel A
                analogWrite(11, 200);   //Spins the motor on Channel A at full speed
                break;
              
                //BOTH BRAKE
                case 'b':
                digitalWrite(9, HIGH);   //Disengage the Brake for Channel A
                digitalWrite(8, HIGH);   //Disengage the Brake for Channel A
                break;   
                }

       }
  

}
