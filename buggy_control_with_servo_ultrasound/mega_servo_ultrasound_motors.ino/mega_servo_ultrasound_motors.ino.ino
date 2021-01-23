#include <Servo.h> //lib for servo control
#include <SimpleTimer.h> //lib for timers
#include "commands.h" //lib for motor control
#include <NewPing.h>  //lib for ultrasound sensor
#include <LiquidCrystal_I2C.h> //lib for lcd display

#define TRIGGER_PIN 46 //init trigger pin for ultrasound
#define ECHO_PIN 44    //init echo pin for ultrasound
#define MAX_DISTANCE 1000 // max distance of 1000cm

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  //create ultrasound object 'sonar'
LiquidCrystal_I2C lcd(0x27, 16, 2);   //init lcd display

SimpleTimer t;  //init timers, t, t1, t3
SimpleTimer t1;
SimpleTimer t2;

Servo myservo;  // create servo object to control a servo

int pos = 30;    //variable to store the servo position
int counter = 1; //variable to move servo
int water;       //variable to store water value
int distance;    //variable to store distance (cm)

bool autonomous = false;   //bool to control autonomous behaviour
bool startSensors = false; //bool to start/stop sensor function
bool writeLock = true;     //bool used as mutex lock for writing to uart data line
bool printLock = true;     //bool used to lock printing to lcd



//function to measure distance
void getPing(){
if(startSensors==true){       //execute function if sensors have started
  distance = sonar.ping_cm(); //store distance reading in variable 'distance'
  if(writeLock==true){        //if writelock is available, execute code
  writeLock=false;            //take lock
  Serial1.print('A');         //print char 'A' to uart
  Serial1.write(distance);    //write distance value to uart
  writeLock=true;             //release lock
      }
  }
  if(autonomous==true){       //if autonomous is enabled, execute code
      if(distance<=40){       //minimum distance before obstacle avoidance
        if(pos>=30 && pos<=75){left();}   //whilst ultrasound is facing right, buggy turns left 
        if(pos>=75 && pos<=120){right();} //whilst ultrasound is facing left, buggy turns right
        }
        else {forward();}     //while distance is above 40cm, continue forward
     }
}

//function to rotate servo with mounted ultrasound
void sweep_servo() {     
counter=counter+5;      //increase variable counter by 5
float rad = counter*(3.14159/180);  //convert counter value to radian and store in variable 'rad'
pos = 45*sin(rad)+75;   //sin function to cycle position between 75 - 120, creating sweep motion
myservo.write(pos);     // tell servo to go to position in variable 'pos'
}

//function to measure water value
void water_sensor(){
if(startSensors==true){ //execute function if sensors have started
water = analogRead(A5); //store water value in variable 'water'
if(water>255){water=255;}   //if water value is higher than 255 (higher than uart tranfer limit) assert value to max = 255
    if(writeLock==true){    //if writelock is available, execute number
      writeLock=false;      //take lock
      Serial1.print('B');   //print char 'B' to uart
      Serial1.write(water); //write water value to uart 
      writeLock=true;       //release lock
    }
  }
if(water>100){         //if water value is above 100
  autonomous = false;  //turn off autonomous mode
  brakes();            //stop the buggy
  }
}

//function to display warning message to lcd
void WATER_WARNING(){
if (water>100 && printLock==true){  //if water value is above 100 and lcd is available to print to
lcd.print("WARNING: WATER");        //print string to lcd
printLock=false;                    //disable printing to lcd
}
else if (water<100) {               //if water value is less than 100
  if(printLock==false){             //if printing is disabled
  lcd.clear();                      //clear lcd
  }
  printLock=true;                   //enable lcd printing
}
}

void setup() {
  t.setInterval(25, sweep_servo);    //timer  calls sweep_servo every 25ms
  t1.setInterval(100, getPing);      //timer calls getPing every 100ms
  t2.setInterval(300, water_sensor); //timer calls water_sensor every 300ms
  Serial.begin(9600);                //init serial baud rate 9600
  Serial1.begin(9600);               //init serial1 baud rate 9600
  motor_setup();                     //init motors
  myservo.attach(7);                 //attaches the servo on pin 9 to the servo object
  lcd.backlight();                   //init lcd backlight
  lcd.init();                        //init lcd
}




void loop() {
  t.run();   //start timers
  t1.run();
  t2.run();

 WATER_WARNING(); //call function to check water level
 COMMAND();       //check serial rx for command data
}




//function to send commands to motors
 void COMMAND(){
   if (Serial1.available()) {       //When data is available in the buffer..
    char command = Serial1.read();  //store value in 'command'
    switch(command){
            
          case '1':                  //if command value = '1', toggle sensor functionality
          startSensors=!startSensors;
          break;
          
          case 'w':                  //if command value = 'w', move buggy forward 
          forward();
          break;
  
          case 'a':                  //if command value = 'a', turn buggy left
          left();
          break;
  
          case 'd':                  //if command value = 'd', turn buggy right
          right();
          break;
  
          case 's':                  //if command value = 's', move buggy backwards
          backward();
          break;
        
          case 'b':                  //if command value = 'b', stop buggy
          brakes();
          break;  

          case 'n':                  //if command value = 'n', start sensors and toggle autonomous mode
          startSensors=true;
          autonomous = !autonomous;
          break;
      }
     }
 }
