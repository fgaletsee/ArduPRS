
//Author: Franz G. Aletsee
//Call: DL6FCD K1AFR
//Mail: franz.aletsee@dl6fcd.de
//Date: 08/2015

//Config your Call, ID and IS-Server
#define call "DL6FCD"
#define server "vancouver.aprs2.net"
#define serverport 14580
//Location
#define NS "4817.96N"
#define EW "01054.70E"

#include <SPI.h>
#include <Ethernet.h>
#include <APRSIS.h>
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
if (aprsis.connect2Server(server, serverport, call, 5))
  {
    Serial.println("TCP/IP connected");
  }
  else {
    Serial.println("TCP/IP connection failed");
  }
//while there is a connection
  while (client.connected())
  {
    aprsis.sendItem("FranzT", "4817.30N", EW, 'K', '/', "Test1234");
    delay(1000);
    aprsis.sendCall("DL6FCD-1", NS, EW, 'P', '/', "Test1234");
    //Wait 5 minutes
    delay(1000*60*5);
  }
}

