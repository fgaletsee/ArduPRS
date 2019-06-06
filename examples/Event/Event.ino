
//Author: Franz G. Aletsee
//Call: DL6FCD K1AFR
//Mail: franz.aletsee@dl6fcd.de
//Date: 08/2015

//Config your Call, ID and IS-Server
#define call "DL6FCD"
#define server "vancouver.aprs2.net"
#define serverport 14580
//Location
//Format must have same number of digits!
#define NS "4817.96N"
#define EW "01054.70E"

//Hight is important to calculate pressure @ sealevel
#define ALTITUDE 500.0

#define comment "Temp in shack"

#include <SPI.h>
#include <Ethernet.h>
#include <APRSIS.h>
#include <Wire.h>

APRS_IS aprsis;

byte mac[] = { 0xFA, 0x21, 0x06, 0x19, 0x88, 0x10 };
EthernetClient client;

void setup()
{
  Serial.begin(115200);
  Serial.println("REBOOT");

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (1);
  }
  

}

void loop() {

  if (aprsis.connect2Server(server, serverport, call, 5,"b/dl6fcd"))
  {
    
  }
  else {
    Serial.println("TCP/IP connection failed");
  }

  while (client.connected())
  {

    //Here we are doing something
    aprsis.readServer();
	processPacket();
	delay(300);


  }
}

void processPacket()
{
  char *p;

  p = strstr(aprsis.getBuffPointer(), "\n");
  if (p != NULL)
  {
    p = strstr(aprsis.getBuffPointer(), ":DL6FCD   :F");
    if (p != NULL)
    {
      Serial.println("Event ausgeloesst");
      Serial.println(*(p + 12));
      if (*(p + 12) == '1')
      {
        
        Serial.println("LED an");
      }
      else
      {
        
        Serial.println("LED aus");
      }
    }
  }
}






