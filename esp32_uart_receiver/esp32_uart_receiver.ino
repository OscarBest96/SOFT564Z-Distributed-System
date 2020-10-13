/*
 * There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.
 * 
 * U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
 * U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though
 * U2UXD is unused and can be used for your projects.
 * 
*/
 
#define RXD2 16
#define TXD2 17
 
void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);

}
 
void loop() { //Choose Serial1 or Serial2 as required
  while (Serial1.available()) {
    int C = Serial1.read();
    //Serial.println(Serial1.read());     returns 255 if first
    Serial1.write(C);        // prints empty value -1 if write 
    Serial.println(C);
    
    
  }
}
