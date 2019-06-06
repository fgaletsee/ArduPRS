/*
APRS_IS by Franz G. Aletsee
dl6fcd@gmail.com
*/

#ifndef APRS_IS_h
#define APRS_IS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>
#include <Ethernet.h>

#define buflen 128
extern EthernetClient client;
class APRS_IS
{
	public:
		APRS_IS(); // base type

		void begin(char *psever, uint16_t port, char *pCall, char id);
		uint8_t connect2Server(char *psever, uint16_t port, char *pCall, char id);
		uint8_t connect2Server(char *psever, uint16_t port, char *pCall, char id,char * filter);
		
		void sendCall(char *pcall, char *pcomment);
		void sendCall(char *pcall, char *pNS, char *pEW, char sym, char symtable, char *pcomment);
		void sendItem(char *pname, char *pNS, char *pEW, char sym, char symtable, char *pcomment);
		uint8_t checkPos(char *pos, uint8_t type);
		void char2buff(char ps);
		void add2buff(char *p,char *ps);
		void add2buff(char *ps);
		void addchar2buff(char data);
		void chearchar();
		void chearchar(char *p,uint8_t leng);
		void adduint162buff(uint16_t val, char leng);
		void uint162buff(char *p, uint16_t val, char leng);
		void uchar2buff(char *p, unsigned char val, char leng);
		void uchar2buff(char *p, unsigned char val);
		char readServer();
		void send();
		char txdata[buflen];
		char * getBuffPointer();
		
		
	private:
	
		
			// connect to IS-Server
		uint16_t aprsPassGen(char *p);
			//Generate Password
			
		uint8_t isNum(char x);
		
		uint8_t getLength(char *pchar);
		
		
		char * findend();
		char * findend(char *p);

		
		
		
		//exactly 9 charaters
		char *callid = "123456789";
		
		
};


#endif
