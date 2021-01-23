
#define RXD2 16 //these are the ports responisble for serial port 2
#define TXD2 17
 
void setup() {

  Serial.begin(9600); //starts serial for printing to terminal (serial port 0)
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // starts serial port 2, wired connection to arduino mega.

}
 
void loop() { 
  while (Serial2.available()) {  //When data is available in the buffer..
    int C = Serial2.read();  //store value in 'C'
    Serial2.write(C);        //send received value back to mega
    Serial.println(C);       //print recieved data to terminal
    
    
  }
}
