#define F_CPU 16000000UL 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 


 #define swap(a,b) {int16_t t=a;a=b;b=t;}

#define DATA_DDR DDRD

#define DATA_PORT PORTD

#define DATA_PIN PIND

#define DATA2_DDR DDRB

#define DATA2_PORT PORTB

#define DATA2_PIN PINB


#define COMMAND_DDR DDRC

#define COMMAND_PORT PORTC

#define LCD_CS 3//Chip Select

#define LCD_CD 2//Command/Data

#define LCD_WR 1//LCD Write

#define LCD_RD 0//LCD Read

#define LCD_RESET 4//LCD Reset

#define RESET_IDLE COMMAND_PORT|=(1<<LCD_RESET)

#define CS_IDLE COMMAND_PORT|=(1<<LCD_CS)

#define WR_IDLE COMMAND_PORT|=(1<<LCD_WR)

#define RD_IDLE COMMAND_PORT|=(1<<LCD_RD)

#define RESET_ACTIVE COMMAND_PORT&=~(1<<LCD_RESET)

#define CS_ACTIVE COMMAND_PORT&=~(1<<LCD_CS)

#define WR_ACTIVE COMMAND_PORT&=~(1<<LCD_WR)

#define RD_ACTIVE COMMAND_PORT&=~(1<<LCD_RD)

#define CD_COMMAND COMMAND_PORT&=~(1<<LCD_CD)

#define CD_DATA COMMAND_PORT|=(1<<LCD_CD)

#define BLACK 0x0000

#define BLUE 0x001F

#define RED 0x0F800

#define GREEN 0x07E0

#define CYAN 0x07FF

#define MAGENTA 0xF81F

#define YELLOW 0xFFE0

#define WHITE 0xFFFF

#define setReadDir() { DATA_DDR&=0b00000011; DATA2_DDR&=0b11111100; }

#define setWriteDir() { DATA_DDR|=0b11111100; DATA2_DDR|=0b00000011; }

#define WR_STROBE {WR_ACTIVE;WR_IDLE;}

//void TFT9341_ini(void); 

void port_ini(void)

{

DATA_PORT=0x00;
DATA2_PORT=0x00;

DATA_DDR=0xFF;//Шина данных на выход
DATA2_DDR=0xFF;

COMMAND_DDR=0x1F;//Командные лапки также все на выход

} 

#define MYUBRR (F_CPU/16/9600-1)

void uart_init(void)
{


UBRR0L=(uint8_t)(MYUBRR%256);
UBRR0H=(uint8_t)(MYUBRR/256);
UCSR0B=1<<TXEN0;
UCSR0C=0b11 << UCSZ00;
UCSR0A=0;

}

void uart_send(uint8_t byte)
{
	while(!(UCSR0A & (1<<UDRE0))) ;
	UDR0=byte;
}

void print_num(uint8_t byte)
{
	if (byte>100) uart_send('0'+byte/100);
	if (byte%100>10) uart_send('0'+(byte%100)/10);
	uart_send('0'+byte%10);
}


 

void TFT9341_SendByte(unsigned char byte)
{
  DATA_PORT=(DATA_PORT & 0b00000011) | (byte & 0b11111100);
  DATA2_PORT=(DATA2_PORT & 0b11111100) |(byte & 0b00000011);
}

unsigned char TFT9341_ReadByte(void)
{
  return (DATA_PIN & 0b11111100) | (DATA2_PIN & 0b00000011);
}

 //—————————————————————

void TFT9341_SendCommand(unsigned char cmd)

{

  CD_COMMAND;//лапка в состоянии посылки команды

  RD_IDLE;//отключим чтение

  CS_ACTIVE;//выбор дисплея

  TFT9341_SendByte(cmd);

  WR_STROBE;

  CS_IDLE;

}

//—————————————————————

void TFT9341_SendData(unsigned char dt)

{

  CD_DATA;//лапка в состоянии посылки данных

  RD_IDLE;//отключим чтение

  CS_ACTIVE;//выбор дисплея

  TFT9341_SendByte(dt);

  WR_STROBE;

  CS_IDLE;

}

 //—————————————————————

void TFT9341_reset(void)

{

  CS_IDLE;

  WR_IDLE;

  RD_IDLE;

  RESET_ACTIVE;

  _delay_ms(2);

  RESET_IDLE;

  CS_ACTIVE;

  TFT9341_SendCommand(0x01); //Software Reset

  for (uint8_t i=0;i<3;i++) WR_STROBE;

  CS_IDLE;

}

//—————————————————————

  
//————————————————————— 

void TFT9341_Write8(unsigned char dt)

{

  TFT9341_SendByte(dt);

  WR_STROBE;

} 

unsigned long TFT9341_ReadReg(unsigned char r)

{

  unsigned long id;
  unsigned char x;


 CS_ACTIVE;//выбор дисплея
 CD_COMMAND;//лапка в состоянии посылки команды
 TFT9341_Write8(r); 
 setReadDir(); 
 CD_DATA;
 _delay_us(50); 
 RD_ACTIVE;
 _delay_us(5); 
 x=TFT9341_ReadByte();
 print_num(x);
 RD_IDLE;
 id=x;
 id<<=8; 

 RD_ACTIVE;

_delay_us(5);

 x=TFT9341_ReadByte();
 print_num(x);

RD_IDLE;

id|=x;

id<<=8;

RD_ACTIVE;

_delay_us(5);

 x=TFT9341_ReadByte();
 print_num(x);

RD_IDLE;

id|=x;

id<<=8;

RD_ACTIVE;

_delay_us(5);

 x=TFT9341_ReadByte();
 print_num(x);

RD_IDLE;

id|=x; 

 if(r==0xEF)

{

  id<<=8;

  RD_ACTIVE;

  _delay_us(5);

 x=TFT9341_ReadByte();
 print_num(x);

  RD_IDLE;

  id|=x;

} 
 CS_IDLE;

setWriteDir(); 
   _delay_us(150);//stabilization time

  return id; 

} 


unsigned long dtt=0; 
unsigned int X_SIZE = 0;
unsigned int Y_SIZE = 0; 

 void TFT9341_SetRotation(unsigned char r)

{

  TFT9341_SendCommand(0x36);

  switch(r)

  {

 case 0:

TFT9341_SendData(0x48);

X_SIZE = 240;

Y_SIZE = 320;

break; 

 case 1:

TFT9341_SendData(0x28);

X_SIZE = 320;

Y_SIZE = 240;

break;

 

case 2:

TFT9341_SendData(0x88);

X_SIZE = 240;

Y_SIZE = 320;

break;

 


case 3:

TFT9341_SendData(0xE8);

X_SIZE = 320;

Y_SIZE = 240;

break; 

} 
}


void TFT9341_ini(void)

{

  port_ini(); 
  TFT9341_reset();
  _delay_ms(1000); 
  dtt=TFT9341_ReadReg(0xD3); 
TFT9341_SendCommand(0x01);//Software Reset

TFT9341_SendCommand(0xCB);//Power Control A

TFT9341_SendData(0x39);

TFT9341_SendData(0x2C);

TFT9341_SendData(0x00);

TFT9341_SendData(0x34);

TFT9341_SendData(0x02); 
 TFT9341_SendCommand(0xCF);//Power Control B

TFT9341_SendData(0x00);

TFT9341_SendData(0xC1);

TFT9341_SendData(0x30); 
 TFT9341_SendCommand(0xE8);//Driver timing control A

TFT9341_SendData(0x85);

TFT9341_SendData(0x00);

TFT9341_SendData(0x78);

TFT9341_SendCommand(0xEA);//Driver timing control B

TFT9341_SendData(0x00);

TFT9341_SendData(0x00); 
 TFT9341_SendCommand(0xED);//Power on Sequence control

TFT9341_SendData(0x64);

TFT9341_SendData(0x03);

TFT9341_SendData(0x12);

TFT9341_SendData(0x81);

TFT9341_SendCommand(0xF7);//Pump ratio control

TFT9341_SendData(0x20);

TFT9341_SendCommand(0xC0);//Power Control 1

TFT9341_SendData(0x10);

TFT9341_SendCommand(0xC1);//Power Control 2

TFT9341_SendData(0x10);

TFT9341_SendCommand(0xC5);//VCOM Control 1

TFT9341_SendData(0x3E);

TFT9341_SendData(0x28);

TFT9341_SendCommand(0xC7);//VCOM Control 2

TFT9341_SendData(0x86);

TFT9341_SetRotation(0);

TFT9341_SendCommand(0x3A);//Pixel Format Set

TFT9341_SendData(0x55);//16bit

TFT9341_SendCommand(0xB1);

TFT9341_SendData(0x00);

TFT9341_SendData(0x18);// Частота кадров 79 Гц

TFT9341_SendCommand(0xB6);//Display Function Control

TFT9341_SendData(0x08);

TFT9341_SendData(0x82);

TFT9341_SendData(0x27);//320 строк

TFT9341_SendCommand(0xF2);//Enable 3G (пока не знаю что это за режим)

TFT9341_SendData(0x00);//не включаем

TFT9341_SendCommand(0x26);//Gamma set

TFT9341_SendData(0x01);//Gamma Curve (G2.2) (Кривая цветовой гаммы)

TFT9341_SendCommand(0xE0);//Positive Gamma Correction

TFT9341_SendData(0x0F);

TFT9341_SendData(0x31);

TFT9341_SendData(0x2B);

TFT9341_SendData(0x0C);

TFT9341_SendData(0x0E);

TFT9341_SendData(0x08);

TFT9341_SendData(0x4E);

TFT9341_SendData(0xF1);

TFT9341_SendData(0x37);

TFT9341_SendData(0x07);

TFT9341_SendData(0x10);

TFT9341_SendData(0x03);

TFT9341_SendData(0x0E);

TFT9341_SendData(0x09);

TFT9341_SendData(0x00);

TFT9341_SendCommand(0xE1);//Negative Gamma Correction

TFT9341_SendData(0x00);

TFT9341_SendData(0x0E);

TFT9341_SendData(0x14);

TFT9341_SendData(0x03);

TFT9341_SendData(0x11);

TFT9341_SendData(0x07);

TFT9341_SendData(0x31);

TFT9341_SendData(0xC1);

TFT9341_SendData(0x48);

TFT9341_SendData(0x08);

TFT9341_SendData(0x0F);

TFT9341_SendData(0x0C);

TFT9341_SendData(0x31);

TFT9341_SendData(0x36);

TFT9341_SendData(0x0F); 


TFT9341_SendCommand(0x11);//Выйдем из спящего режим

  _delay_ms(150);


  TFT9341_SendCommand(0x29);//Включение дисплея

  TFT9341_SendData(0x2C);

 CS_ACTIVE;//выбор дисплея
 CD_COMMAND;//лапка в состоянии посылки команды
 TFT9341_Write8(0x29); 
 CD_DATA;
 TFT9341_Write8(0x2C);
 CS_IDLE;
  _delay_ms(150); 
}

void TFT9341_Flood(unsigned short color, unsigned long len)

{

  unsigned short blocks;

  unsigned char i, hi = color>>8, lo=color; 
 CS_ACTIVE;

CD_COMMAND; 
 TFT9341_Write8(0x2C);

CD_DATA;

TFT9341_Write8(hi);

TFT9341_Write8(lo); 

 len--;

blocks=(unsigned short)(len/64);//64 pixels/block 

if (hi==lo)

{ //if

 while(blocks--)

{ //while

  i=16;

  do

  {

    WR_STROBE;WR_STROBE;WR_STROBE;WR_STROBE;//2bytes/pixel

    WR_STROBE;WR_STROBE;WR_STROBE;WR_STROBE;//x4 pixel

  } while (--i);

} //while

//Fill any remaining pixels(1 to 64)

  for (i=(unsigned char)len&63;i--;)

  {

    WR_STROBE;

    WR_STROBE;

  }

} //if  


else

{ //if

 while(blocks--)

{ //while

  i=16;

  do

  {

   TFT9341_Write8(hi);TFT9341_Write8(lo);TFT9341_Write8(hi);TFT9341_Write8(lo);

    TFT9341_Write8(hi);TFT9341_Write8(lo);TFT9341_Write8(hi);TFT9341_Write8(lo); 

  } while (--i);

} //while

 //Fill any remaining pixels(1 to 64)

  for (i=(unsigned char)len&63;i--;)

  {

    TFT9341_Write8(hi);

    TFT9341_Write8(lo);

  }

} //if


CS_IDLE;

} 


 //—————————————————————

void TFT9341_WriteRegister32(unsigned char r, unsigned long d)

{

  CS_ACTIVE;

  CD_COMMAND;

  TFT9341_Write8(r);

  CD_DATA;

  _delay_us(1);

  TFT9341_Write8(d>>24);

  _delay_us(1);

  TFT9341_Write8(d>>16);

  _delay_us(1);

  TFT9341_Write8(d>>8);

  _delay_us(1);

  TFT9341_Write8(d);

  CS_IDLE;

}

//————————————————————— 

 //—————————————————————

void TFT9341_SetAddrWindow(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)

{

  unsigned long t;

  CS_ACTIVE;

  t = x1;

  t<<=16;

  t |= x2;

  TFT9341_WriteRegister32(0x2A,t);//Column Addres Set

  t = y1;

  t<<=16;

  t |= y2;

  TFT9341_WriteRegister32(0x2B,t);//Page Addres Set

  CS_IDLE;

}

//————————————————————— 

 //—————————————————————

void TFT9341_FillScreen(unsigned int color)

{

  TFT9341_SetAddrWindow(0,0,X_SIZE-1,Y_SIZE-1);

  TFT9341_Flood(color,(long)X_SIZE*(long)Y_SIZE);

}

//————————————————————— 

int main(void)
{
	uart_init();
	TFT9341_ini();
//	uart_send('A');
//	while(1) ;
	
	TFT9341_FillScreen(BLACK);

	return 0;
}
