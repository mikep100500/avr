#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

char fick[] PROGMEM="fuckoff";

int main(void)
{

 
#define XTAL 8000000L
#define baudrate 9600L
#define bauddivider (XTAL/(16*baudrate)-1)
#define HI(x) ((x)>>8)
#define LO(x) ((x)& 0xFF)
 
UBRR0L = LO(bauddivider);
UBRR0H = HI(bauddivider);
UCSR0A = 0;
UCSR0B = 1<<RXEN0|1<<TXEN0|1<<RXCIE0|0<<TXCIE0;
UCSR0C = 1<<UCSZ00 | 1<<UCSZ01; 

char *iPtr;

iPtr=(char *)malloc(100);
strcpy_P(iPtr,fick);


}
