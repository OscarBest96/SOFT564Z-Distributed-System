#include <WiFi.h>

WiFiClient RemoteClient;

const uint ServerPort = 23;
WiFiServer Server(ServerPort);



void CheckForConnections()
{
  if (Server.hasClient())
  {
    // If we are already connected to another computer, 
    // then reject the new connection. Otherwise accept
    // the connection. 
    if (RemoteClient.connected())
    {
      Serial.println("Connection rejected");
      Server.available().stop();
    }
    else
    {
      Serial.println("Connection accepted");
      RemoteClient = Server.available();
    }
  }
}


void setup()
{
  Server.begin();
}

void loop() 
{
  CheckForConnections();
  
  delay(50);
}







 
