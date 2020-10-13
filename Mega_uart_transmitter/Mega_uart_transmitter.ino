int A =50;

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(9600);
  Serial1.begin(9600);

}
 
void loop() { //Choose Serial1 or Serial2 as required
  
  Serial1.write(A);
  delay(1000);

  while (Serial1.available()) {
    Serial.println(Serial1.read());
   // delay(1000);
   }
   
}
