#include <SimpleTimer.h>

int X =123;  //initialise dummy sensor value
int Y =30;
SimpleTimer t;
SimpleTimer t2;

bool serialLock = true;

void A(){
if(serialLock==true){
  serialLock = false;
  Serial1.print('A');
  Serial1.write(X); //write data to serial every second
  serialLock = true;
  }
}
void B(){
  if(serialLock==true){
  serialLock = false;
  Serial1.print('B');
  Serial1.write(Y); //write data to serial every second
  serialLock = true;
  }
}

void setup() {
  Serial.begin(9600); // begin serial port 0 (for communication to terminal) for debugging
  Serial1.begin(9600); // begin serial port 1, wired connection to esp32
  t.setInterval(500, A);
  t2.setInterval(1000, B);
}
 
void loop() { 
  t.run();
  t2.run();


  while (Serial1.available()) {  //when data is available at RX pin 
    Serial.println(Serial1.read()); // print the data recieved to the terminal
   // delay(1000);   
  }
   
}
