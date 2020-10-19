#include "DHT.h"

#define DHTPIN 48        // what digital pin we're connected to
#define DHTTYPE DHT11     // DHT11
DHT dht(DHTPIN, DHTTYPE);

float temp, humidity;
 
void setup(){
 
  Serial.begin(9600);
  dht.begin();
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor
 
}//end "setup()"
 
void loop(){
  //Start of Program 

    temp = dht.readTemperature();
    humidity = dht.readHumidity();
    
    Serial.print(humidity);
    Serial.print("%  ");
    
    Serial.print(temp); 
    Serial.println("C  ");
    
    delay(5000);//Wait 5 seconds before accessing sensor again.
 
  //Fastest should be once every two seconds.
 
}// end loop(
