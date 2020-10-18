int A =50;  //initialise dummy sensor value

void setup() {
  Serial.begin(9600); // begin serial port 0 (for communication to terminal) for debugging
  Serial1.begin(9600); // begin serial port 1, wired connection to esp32

}
 
void loop() { 
  
  Serial1.write(A); //write data to serial every second
  delay(1000);

  while (Serial1.available()) {  //when data is available at RX pin 
    Serial.println(Serial1.read()); // print the data recieved to the terminal
   // delay(1000);   
  }
   
}
