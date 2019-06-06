
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
#include <SFE_BMP180.h>

SFE_BMP180 pressure;
double T,P;

APRS_IS aprsis;

byte mac[] = { 0xFA, 0x21, 0x06, 0x19, 0x88, 0x10 };
EthernetClient client;

void setup()
{
  Serial.begin(115200);
  Serial.println("REBOOT");
  
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (1);
  }
  //Initalize APRS IS
  aprsis.begin(server, serverport, call, 11);

}

void loop() {

  if (aprsis.connect2Server(server, serverport, call, 5)) 
  {
    Serial.println("TCP/IP connected");
  }
  else {
    Serial.println("TCP/IP connection failed");
  }

  while (client.connected())
  {
    //do this every 10 minutes!
    sendPacket();
    uint16_t k = 0;
    for (k = 0; k < 1200; k++)
    {
      delay(500);
      aprsis.readServer();
    }
  }
}






void sendPacket()
{
doMeas();
  if (!aprsis.checkPos(NS, 1))
  {
    Serial.println("Error:Position NS incorrect!");
    return;
  }
  if (!aprsis.checkPos(EW, 2))
  {
    Serial.println("Error:Position EW incorrect!");
    return;
  }
  T=(9.0/5.0)*T+32.0; // in Farhenheit
  P=P*10.0;
  aprsis.chearchar();
  aprsis.add2buff(call);
  aprsis.add2buff("-7");
  aprsis.add2buff(">APZFGA,TCPIP*:!");
  aprsis.add2buff(NS);
  aprsis.add2buff("/");
  aprsis.add2buff(EW);
  aprsis.add2buff("_.../...g...t");
  aprsis.adduint162buff(T, 3);
  aprsis.add2buff("b");
  aprsis.adduint162buff(P, 5);
  aprsis.add2buff("h..");
  aprsis.send();
  
  aprsis.chearchar();
  aprsis.add2buff(call);
  aprsis.add2buff("-7");
  aprsis.add2buff(">APZFGA,TCPIP*:>");
  aprsis.add2buff(comment);
  aprsis.send();  
  
  
}
