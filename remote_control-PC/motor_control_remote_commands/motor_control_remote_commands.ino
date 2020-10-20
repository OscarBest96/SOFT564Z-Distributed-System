
void setup() {

  Serial.begin(9600); //starts serial for printing to terminal (serial port 0)
  Serial1.begin(9600);
  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel B pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel B pin

}
 
void loop() { 
  while (Serial1.available()) {  //When data is available in the buffer..
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
