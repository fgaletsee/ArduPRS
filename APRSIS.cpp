/*
APRS-IS Library by Franz G. Aletsee
dl6fcd@gmail.com
*/

#include <APRSIS.h>



APRS_IS::APRS_IS()
// Base library type
{
}


void APRS_IS::begin(char *psever, uint16_t port, char *pCall, char id)
// 
{
	APRS_IS::connect2Server(psever, port, pCall, id);

}


uint16_t APRS_IS::aprsPassGen(char *p)
{
  //Only Capital letter at the moment
  //Thanks to https://github.com/magicbug/PHP-APRS-Passcode/

  uint16_t hash = 0x73e2;
  uint8_t k = 0;
  uint8_t sleng;
  char *pstart;
  pstart = p;

  //Count length
  while (*p)
  {
    sleng++;
    p++;
  }
  p=pstart;
  while (k + 1 < sleng)
  {
    hash ^= *(p + k) << 8;
    hash ^= *(p + k + 1);
    k += 2;
  }
  return hash & 0x7fff;
}
uint8_t APRS_IS::connect2Server(char *psever, uint16_t port, char *pCall, char id)
{
	connect2Server(psever, port, pCall, id, "none");

}
uint8_t APRS_IS::connect2Server(char *psever, uint16_t port, char *pCall, char id,char *filter)
{
	char ret;
	ret=client.connect(psever, port);
   // Serial.print("Debug:pcall=");
   //Serial.println(pCall);
   uint16_t passc;
  //Serial.print("Debug:connect2Server:Pass:");
 
   
  passc=aprsPassGen(pCall);
 // Serial.println(passc);
  
   //Serial.print("Debug1:callid=");
   //Serial.println(callid);
   
   //Generate Call plus ID
   chearchar(callid,10);
   add2buff(callid,pCall);
   if (id!=0)
   {
   add2buff(callid,"-");
   uchar2buff(callid,id);
   }

  //Credentials
  chearchar();
  add2buff("user ");
  add2buff(callid);
  add2buff(" pass ");
  adduint162buff(passc, 5);
  add2buff(" vers ArduPRS V1.0 ");
  add2buff("filter ");
  add2buff(filter);
  Serial.println(txdata);
  client.println(txdata);
  return(ret);
}
void APRS_IS::send()
{
	Serial.println(txdata);
	client.println(txdata);

}


char APRS_IS::readServer()
{
	chearchar();
	uint8_t cnt=0;
  //Serial.print("ReadServer:");
  while (client.available() && cnt<buflen) {
    char c = client.read();
	 addchar2buff(c);
    Serial.print(c);
	cnt++;
  }
	
  return 0;
}

char * APRS_IS::getBuffPointer()
{
	return(txdata);
}



void APRS_IS::sendItem(char *pname, char *pNS, char *pEW, char sym, char symtable, char *pcomment)
{
  if (getLength(pname) > 9)
  {
    Serial.println("Debug:Item name too long!");
    return;
  }

  if (!checkPos(pNS, 1))
  {
    Serial.println("Debug:Position NS incorrect!");
    return;
  }

  if (!checkPos(pEW, 2))
  {
    Serial.println("Debug:Position EW incorrect!");
    return;
  }
  chearchar();
  add2buff(callid);
  add2buff(">APZFGA,TCPIP*:)");
  add2buff(pname);
  add2buff("!");
  add2buff(pNS);
  char2buff(symtable);
  add2buff(pEW);
  char2buff(sym);
  add2buff(pcomment);
  Serial.println(txdata);
  client.println(txdata);
}

void APRS_IS::sendCall(char *pcall, char *pNS, char *pEW, char sym, char symtable, char *pcomment)
{
  
  if (getLength(pcall) > 9)
  {
    Serial.println("Debug:Call  too long!");
    return;
  }

  

  if (!checkPos(pNS, 1))
  {
    Serial.println("Debug:Position NS incorrect!");
    return;
  }

  if (!checkPos(pEW, 2))
  {
    Serial.println("Debug:Position EW incorrect!");
    return;
  }
  chearchar();
  add2buff(pcall);
  add2buff(">APZFGA,TCPIP*:");
  add2buff("!");
  add2buff(pNS);
  char2buff(symtable);
  add2buff(pEW);
  char2buff(sym);
  add2buff(pcomment);
  Serial.println(txdata);
  client.println(txdata);
}
void APRS_IS::sendCall(char *pcall, char *pcomment)
{
  
  if (getLength(pcall) > 9)
  {
    Serial.println("Debug:Call  too long!");
    return;
  }
   chearchar();
  add2buff(pcall);
  add2buff(">APZFGA,TCPIP*:");
  add2buff(">");
  add2buff(pcomment);


  Serial.println(txdata);
  client.println(txdata);
}

//Check if postion data string is valid
uint8_t APRS_IS::checkPos(char *pos, uint8_t type)
{
  //type 1 =NorthSouth 2=EastWest
  if (type == 1) // NS
  {
    if (!isNum(*pos))
      return 0;
    if (!isNum(*(pos + 1)))
      return 0;
    if (!isNum(*(pos + 2)))
      return 0;
    if (!isNum(*(pos + 3)))
      return 0;
    if ((*(pos + 4)) != '.')
      return 0;
    if (!isNum(*(pos + 5)))
      return 0;
    if (!isNum(*(pos + 6)))
      return 0;
    if ((*(pos + 7)) != 'N' && (*(pos + 7)) != 'S' )
      return 0;
    return 1;
  }
  else if (type == 2)
  {
    if (!isNum(*pos))
      return 0;
    if (!isNum(*(pos + 1)))
      return 0;
    if (!isNum(*(pos + 2)))
      return 0;
    if (!isNum(*(pos + 3)))
      return 0;
    if (!isNum(*(pos + 4)))
      return 0;
    if ((*(pos + 5)) != '.')
      return 0;
    if (!isNum(*(pos + 6)))
      return 0;
    if (!isNum(*(pos + 7)))
      return 0;
    if ((*(pos + 8)) != 'E' && (*(pos + 8)) != 'W' )
      return 0;
    return 1;

  }
  else {
    return 0;
  }

}

//check if character is a number
uint8_t APRS_IS::isNum(char x)
{
  if (x >= 0x30 && x <= 0x39)
    return 1;
  return 0;
}

uint8_t APRS_IS::getLength(char *pchar)
{
  uint8_t leng = 0;
  while (*pchar)
  {
    pchar++;
    leng++;
  }
  return leng;
}

//charbuffer stuff

void APRS_IS::uchar2buff(char *p, unsigned char val)
{
  char leng=1;
  if (val>=10 && val<100)
  leng=2;
  if (val >=100)
  leng=3;
  uchar2buff(p,val,leng);
  }

void APRS_IS::uchar2buff(char *p, unsigned char val, char leng)
{
  p = findend(p);
  char k = 0;
  char tmp = 0;
  for (k = leng; k > 0; k--)
  {
    tmp = val / (pow(10, (k - 1)));
    if (tmp > 0)
      val = val - tmp * (pow(10, (k - 1)));
    *p = 0x30 + tmp;
    p++;
  }
}

void APRS_IS::uint162buff(char *p, uint16_t val, char leng)
{
  char k = 0;
  uint16_t tmp = 0;
  for (k = leng; k > 0; k--)
  {
    tmp = val / (pow(10, (k - 1)));
    if (tmp > 0)
      val = val - tmp * (pow(10, (k - 1)));
    *p = 0x30 + tmp;
    p++;
  }
}

void APRS_IS::adduint162buff(uint16_t val, char leng)
{
  char *p = findend();
  char k = 0;
  uint16_t tmp = 0;
  for (k = leng; k > 0; k--)
  {
    tmp = val / (pow(10, (k - 1)));
    if (tmp > 0)
      val = val - tmp * (pow(10, (k - 1)));
    *p = 0x30 + tmp;
    p++;
  }
}



void APRS_IS::chearchar(char *p,uint8_t leng)
{
  uint8_t k = 0;
  for (k = 0; k < leng; k++)
  {
    *p = 0x00;
    p++;
  }
}
void APRS_IS::chearchar()
{
  chearchar(txdata,buflen);
}



void APRS_IS::add2buff(char *ps)
{
add2buff(txdata,ps);
}

void APRS_IS::addchar2buff(char data)
{
	char *ps;
	ps = findend(txdata);
	*ps=data;
}

void APRS_IS::add2buff(char *p,char *ps)
{
  p = findend(p);
  while (*ps)
  {
    *p = *ps;
    ps++;
    p++;
  }
}
void APRS_IS::char2buff(char ps)
{
  char *p;
  p = findend();
  *p=ps;
}

char * APRS_IS::findend(char *p)
{

  while (*p)
  {
    p++;
  }
  return p;
}


char * APRS_IS::findend()
{
return(findend(txdata));
}

//Funtion sucht nach string ind string strstr 