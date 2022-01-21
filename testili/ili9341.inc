
.dseg 

dtt: .byte 4
x_size: .byte 2
y_size: .byte 2

.cseg

.equ DATA_PORT=PORTD
.equ DATA_DDR=DDRD
.equ DATA_PIN=PIND

.equ DATA2_PORT=PORTB
.equ DATA2_DDR=DDRB
.equ DATA2_PIN=PINB


.equ COMMAND_DDR=DDRC
.equ COMMAND_PORT=PORTC


.equ LCD_CS=3//Chip Select

.equ LCD_CD=2//Command/Data

.equ LCD_WR=1//LCD Write

.equ LCD_RD=0//LCD Read

.equ LCD_RESET=4//LCD Reset

.macro RESET_IDLE
  sbi COMMAND_PORT,LCD_RESET
.endm

.macro CS_IDLE
  sbi COMMAND_PORT,LCD_CS
.endm

.macro WR_IDLE
  sbi COMMAND_PORT,LCD_WR
.endm

.macro RD_IDLE
  sbi COMMAND_PORT,LCD_RD
.endm

.macro RESET_ACTIVE
  cbi COMMAND_PORT,LCD_RESET
.endm

.macro CS_ACTIVE
  cbi COMMAND_PORT,LCD_CS
.endm

.macro WR_ACTIVE
  cbi COMMAND_PORT,LCD_WR
.endm

.macro RD_ACTIVE
  cbi COMMAND_PORT,LCD_RD
.endm

.macro CD_COMMAND
  cbi COMMAND_PORT,LCD_CD
.endm

.macro CD_DATA
  sbi COMMAND_PORT,LCD_CD
.endm

.macro WR_STROBE
  cbi COMMAND_PORT,LCD_WR
  sbi COMMAND_PORT,LCD_WR
.endm


.equ BLACK=0x0000

.equ BLUE=0x001F

.equ RED=0x0F800

.equ GREEN=0x07E0

.equ CYAN=0x07FF

.equ MAGENTA=0xF81F

.equ YELLOW=0xFFE0

.equ WHITE=0xFFFF 

.macro setWriteDir 
  sbi DATA2_DDR,0
  sbi DATA2_DDR,1
  sbi DATA_DDR,2
  sbi DATA_DDR,3
  sbi DATA_DDR,4
  sbi DATA_DDR,5
  sbi DATA_DDR,6
  sbi DATA_DDR,7
.endm

.macro setReadDir 
  cbi DATA2_DDR,0
  cbi DATA2_DDR,1
  cbi DATA_DDR,2
  cbi DATA_DDR,3
  cbi DATA_DDR,4
  cbi DATA_DDR,5
  cbi DATA_DDR,6
  cbi DATA_DDR,7
.endm

.macro port_ini
  setWriteDir
  push temp
  clr temp
  out DATA_PORT,temp //todo, bits
  out DATA2_PORT,temp
  pop temp
  sbi COMMAND_DDR,0
  sbi COMMAND_DDR,1
  sbi COMMAND_DDR,2
  sbi COMMAND_DDR,3
  sbi COMMAND_DDR,4
.endm



TFT_OUTBYTE:
push temp
push value

andi value,0b11111100
in temp,DATA_PORT
andi temp,0b00000011
or temp,value
out DATA_PORT,temp

pop value
push value

andi value,0b00000011
in temp,DATA2_PORT
andi temp,0b11111100
or temp,value
out DATA2_PORT,temp

pop value
pop temp
ret

TFT_INBYTE:
push temp

in value,DATA_PIN
andi value,0b11111100
in temp,DATA2_PIN
andi temp,0b00000011
or value,temp

pop temp
ret


TFT_SENDCOMMAND:
  CD_COMMAND;//лапка в состоянии посылки команды
  RD_IDLE;//отключим чтение
  CS_ACTIVE;//выбор дисплея

  rcall TFT_OUTBYTE

  WR_STROBE;
  CS_IDLE;
  ret

//—————————————————————

TFT_SENDDATA:

  CD_DATA;//лапка в состоянии посылки данных
  RD_IDLE;//отключим чтение
  CS_ACTIVE;//выбор дисплея
  
  rcall TFT_OUTBYTE

  WR_STROBE;

  CS_IDLE; 
  ret


TFT_RESET:

  CS_IDLE;
  WR_IDLE;
  RD_IDLE;
  RESET_ACTIVE;

  ldi vallow,2
  clr valhigh
  rcall DELAY_MS

  RESET_IDLE;
  CS_ACTIVE;

  ldi value,1
  rcall TFT_SENDCOMMAND; //Software Reset

  WR_STROBE;
  WR_STROBE;
  WR_STROBE;
  CS_IDLE;
  ret
} 

TFT_INI:
port_ini
rcall TFT_RESET
ldi valhigh,HIGH(1000)
ldi vallow,LOW(1000)
;rcall DELAY_MS
ldi value, 0xd3
rcall TFT_READREG
CS_ACTIVE
ldi value,1
rcall TFT_SENDCOMMAND

ldi value,0xCB
rcall TFT_SENDCOMMAND;//Power Control A

ldi value,0x39
rcall TFT_SENDDATA;

ldi value,0x2C
rcall TFT_SENDDATA;

ldi value,0x00
rcall TFT_SENDDATA;

ldi value,0x34
rcall TFT_SENDDATA;

ldi value,0x02
rcall TFT_SENDDATA;


ldi value,0xCF
rcall TFT_SENDCOMMAND;//Power Control B

ldi value,0x00
rcall TFT_SENDDATA;

ldi value,0xC1
rcall TFT_SENDDATA;

ldi value,0x30
rcall TFT_SENDDATA;

 


ldi value,0xE8
rcall TFT_SENDCOMMAND;//Driver timing control A

ldi value,0x85
rcall TFT_SENDDATA;

ldi value,0x00
rcall TFT_SENDDATA;

ldi value,0x78
rcall TFT_SENDDATA;

ldi value,0xEA
rcall TFT_SENDCOMMAND;//Driver timing control B

ldi value,0x00
rcall TFT_SENDDATA;

ldi value,0x00
rcall TFT_SENDDATA;

 

ldi value,0xED
rcall TFT_SENDCOMMAND;//Power on Sequence control

ldi value,0x64
rcall TFT_SENDDATA;

ldi value,0x03
rcall TFT_SENDDATA;

ldi value,0x12
rcall TFT_SENDDATA;

ldi value,0x81
rcall TFT_SENDDATA;

ldi value,0xF7
rcall TFT_SENDCOMMAND;//Pump ratio control

ldi value,0x20
rcall TFT_SENDDATA;

ldi value,0xC0
rcall TFT_SENDCOMMAND;//Power Control 1

ldi value,0x10
rcall TFT_SENDDATA;

ldi value,0xC1
rcall TFT_SENDCOMMAND;//Power Control 2

ldi value,0x10
rcall TFT_SENDDATA;

ldi value,0xC5
rcall TFT_SENDCOMMAND;//VCOM Control 1

ldi value,0x3E
rcall TFT_SENDDATA;

ldi value,0x28
rcall TFT_SENDDATA;

ldi value,0xC7
rcall TFT_SENDCOMMAND;//VCOM Control 2

ldi value,0x86
rcall TFT_SENDDATA;

clr value
rcall TFT_SETROTATION

ldi value,0x3A
rcall TFT_SENDCOMMAND;//Pixel Format Set

ldi value,0x55
rcall TFT_SENDDATA;//16bit

ldi value,0xB1
rcall TFT_SENDCOMMAND;

ldi value,0x00
rcall TFT_SENDDATA;

ldi value,0x18
rcall TFT_SENDDATA;// Частота кадров 79 Гц

ldi value,0xB6
rcall TFT_SENDCOMMAND;//Display Function Control

ldi value,0x08
rcall TFT_SENDDATA;

ldi value,0x82
rcall TFT_SENDDATA;

ldi value,0x27
rcall TFT_SENDDATA;//320 строк

ldi value,0xF2;//Enable 3G (пока не знаю что это за режим
rcall TFT_SENDCOMMAND

ldi value,0x00
rcall TFT_SENDDATA;//не включаем

ldi value,0x26
rcall TFT_SENDCOMMAND;//Gamma set

ldi value,0x01;//Gamma Curve (G2.2) (Кривая цветовой гаммы
rcall TFT_SENDDATA

ldi value,0xE0
rcall TFT_SENDCOMMAND;//Positive Gamma Correction

ldi value,0x0F
rcall TFT_SENDDATA;

ldi value,0x31
rcall TFT_SENDDATA;

ldi value,0x2B
rcall TFT_SENDDATA;

ldi value,0x0C
rcall TFT_SENDDATA;

ldi value,0x0E
rcall TFT_SENDDATA;

ldi value,0x08
rcall TFT_SENDDATA;

ldi value,0x4E
rcall TFT_SENDDATA;

ldi value,0xF1
rcall TFT_SENDDATA;

ldi value,0x37
rcall TFT_SENDDATA;

ldi value,0x07
rcall TFT_SENDDATA;

ldi value,0x10
rcall TFT_SENDDATA;

ldi value,0x03
rcall TFT_SENDDATA;

ldi value,0x0E
rcall TFT_SENDDATA;

ldi value,0x09
rcall TFT_SENDDATA;

ldi value,0x00
rcall TFT_SENDDATA;

ldi value,0xE1
rcall TFT_SENDCOMMAND;//Negative Gamma Correction

ldi value,0x00
rcall TFT_SENDDATA;

ldi value,0x0E
rcall TFT_SENDDATA;

ldi value,0x14
rcall TFT_SENDDATA;

ldi value,0x03
rcall TFT_SENDDATA;

ldi value,0x11
rcall TFT_SENDDATA;

ldi value,0x07
rcall TFT_SENDDATA;

ldi value,0x31
rcall TFT_SENDDATA;

ldi value,0xC1
rcall TFT_SENDDATA;

ldi value,0x48
rcall TFT_SENDDATA;

ldi value,0x08
rcall TFT_SENDDATA;

ldi value,0x0F
rcall TFT_SENDDATA;

ldi value,0x0C
rcall TFT_SENDDATA;

ldi value,0x31
rcall TFT_SENDDATA;

ldi value,0x36
rcall TFT_SENDDATA;

ldi value,0x0F
rcall TFT_SENDDATA;

 

  ldi value,0x11
rcall TFT_SENDCOMMAND;//Выйдем из спящего режим

ldi vallow,150
clr valhigh
rcall DELAY_MS


  ldi value,0x29
rcall TFT_SENDCOMMAND;//Включение дисплея

  ldi value,0x2C
rcall TFT_SENDDATA;

ldi vallow,150
clr valhigh
rcall DELAY_MS

ret

TFT_WRITE8:
rcall TFT_OUTBYTE
WR_STROBE
ret

DELAY_50US:
  ldi temp,0  ; $017700 - даст задержку в 30мс при 16МГц
  delay_50us_loop:
  subi temp,1 
  brne delay_50us_loop
  ret

DELAY_5US:
  ldi temp,25  ; $017700 - даст задержку в 30мс при 16МГц
  delay_5us_loop:
  subi temp,1 
  brne delay_5us_loop
  ret


DELAY_1US:
  ldi temp,5  ; $017700 - даст задержку в 30мс при 16МГц
  delay_1us_loop:
  subi temp,1 
  brne delay_1us_loop
ret

TFT_READREG: ; value - reg #, out val0-7 reg value
 CS_ACTIVE;//выбор дисплея
 CD_COMMAND;//лапка в состоянии посылки команды 
 rcall TFT_WRITE8
 setReadDir
 CD_DATA
 rcall DELAY_50US

 ldi ZL,LOW(dtt+4)
 ldi ZH,HIGH(dtt+4)

 cpi value,0xef
 brne readreg_3


 RD_ACTIVE;
 rcall DELAY_5US
 rcall TFT_INBYTE
 mov number,value
 rcall OUTNUM
 RD_IDLE; 

readreg_3:

 RD_ACTIVE;
 rcall DELAY_5US
 rcall TFT_INBYTE
 st -Z,value
 RD_IDLE; 

 RD_ACTIVE;
 rcall DELAY_5US
 rcall TFT_INBYTE
 st -Z,value
 RD_IDLE; 

 RD_ACTIVE;
 rcall DELAY_5US
 rcall TFT_INBYTE
 st -Z,value
 RD_IDLE; 

 RD_ACTIVE;
 rcall DELAY_5US
 rcall TFT_INBYTE
 st -Z,value
 RD_IDLE; 


 CS_IDLE;
 setWriteDir

  ldi vallow,1
  clr valhigh
  rcall DELAY_MS

 ret

TFT_SETROTATION: //value rot

push value
ldi value,0x36
rcall TFT_SENDCOMMAND
pop value

ldi val320,320%256
ldi val240,240
clr r0
clr r1
inc r1

and value,value //0
brne set_rot_1
ldi value,0x48
rcall TFT_SENDDATA
sts x_size,val240
sts x_size+1,r0
sts y_size,val320
sts y_size+1,r1
ret

set_rot_1:
dec value //1
brne set_rot_2
ldi value,0x28
rcall TFT_SENDDATA
sts x_size,val320
sts x_size+1,r1
sts y_size,val240
sts y_size+1,r0
ret

set_rot_2:
dec value //2
brne set_rot_3
ldi value,0x88
rcall TFT_SENDDATA
sts x_size,val240
sts x_size+1,r0
sts y_size,val320
sts y_size+1,r1

ret

set_rot_3:
ldi value,0xE8
rcall TFT_SENDDATA
sts x_size,val320
sts x_size+1,r1
sts y_size,val240
sts y_size+1,r0

ret


TFT_FLOOD: //vallow,valhigh - color, val0-val2 - len

CS_ACTIVE;
CD_COMMAND; 
ldi value,0x2C
rcall TFT_WRITE8
CD_DATA; 
mov value,valhigh
rcall TFT_WRITE8
mov value,vallow
rcall TFT_WRITE8

subi val0,1
sbci val1,0
sbci val2,0


cp valhigh,vallow
brne lpfullcyclefill

lpshorcyclefill:
WR_STROBE
WR_STROBE
subi val0,1
sbci val1,0
sbci val2,0
brne lpshorcyclefill
CS_IDLE
ret

lpfullcyclefill:
mov value,valhigh
rcall TFT_WRITE8
mov value,vallow
rcall TFT_WRITE8
subi val0,1
sbci val1,0
sbci val2,0
brne lpfullcyclefill
CS_IDLE
ret

TFT_WRITEREGISTER32: // value=reg,reg32val=val0-3

  CS_ACTIVE;
  CD_COMMAND;
  rcall TFT_WRITE8
  CD_DATA;
  rcall DELAY_1US
  mov value,val3
  rcall TFT_WRITE8
  rcall DELAY_1US
  mov value,val2
  rcall TFT_WRITE8
  rcall DELAY_1US
  mov value,val1
  rcall TFT_WRITE8
  rcall DELAY_1US
  mov value,val0
  rcall TFT_WRITE8
  CS_IDLE; 
  ret

TFT_SETADDRWINDOW: //x1=X,x2=val1:0,y1=Y,y2=val3:2

CS_ACTIVE
push val3
push val2
mov val3,XH
mov val2,XL
ldi value,0x2A
rcall TFT_WRITEREGISTER32

pop val0
pop val1
mov val3,YH
mov val2,YL
ldi value,0x2B
rcall TFT_WRITEREGISTER32

CS_IDLE;
ret

TFT_FILLSCREEN: //vallow,valhigh=color
clr XL
clr XH
clr YL
clr YH
lds val0,x_size
lds val1,x_size+1

subi val0,1
sbci val1,0

lds val2,y_size
lds val3,y_size+1

subi val2,1
sbci val3,0

rcall TFT_SETADDRWINDOW

ldi val2,1
ldi val1,0x2c
clr val0

rcall TFT_FLOOD
ret


TFT_DRAWPIXEL: //X,Y - pos, valow valhigh color


lds temp,x_size
cp XL,temp
lds temp,x_size+1
cpc XH,temp
brcc lpdrawpixelexit

lds temp,y_size
cp YL,temp
lds temp,y_size+1
cpc YH,temp
brcc lpdrawpixelexit

mov val0,XL
mov val1,XH
mov val2,YL
mov val3,YH


rcall TFT_SETADDRWINDOW


CS_ACTIVE
CD_COMMAND
ldi value,0x2C
rcall TFT_WRITE8

CD_DATA

mov value,valhigh
rcall TFT_WRITE8
mov value,vallow
rcall TFT_WRITE8

CS_IDLE
lpdrawpixelexit:
ret


TFT_DRAWIMAGE: //X,Y pos val0,1 palette ,val2,3 - image, vallow/high- size
push val0
push val1
push val2
push val3

mul vallow,valhigh

clr temp
dec vallow
dec valhigh

mov val0,XL
mov val1,XH
add val0,vallow
adc val1,temp
mov val2,YL
mov val3,YH
add val2,valhigh
adc val3,temp

rcall TFT_SETADDRWINDOW


CS_ACTIVE;
CD_COMMAND; 
ldi value,0x2C
rcall TFT_WRITE8
CD_DATA;

pop ZH //image
pop ZL
pop XH  //palette
pop XL
mov val0,r0
mov val1,r1


lpdrawimage:
mov YL,XL
mov YH,XH
lpm value,Z+
lsl value
add YL,value
clr temp
adc YH,temp
ld vallow,Y+
ld valhigh,Y	
 
mov value,valhigh
rcall TFT_WRITE8
mov value,vallow
rcall TFT_WRITE8

subi val0,1
sbci val1,0


brne lpdrawimage
CS_IDLE
ret


 


TFT_DRAWCHAR: //X,Y pos vallow,high-color, value char
push ZH
push ZL
push YH
push YL
push XH
push XL

ldi temp,8
mov r0,temp

ldi ZL,LOW(font*2)
ldi ZH,HIGH(font*2)

mov r2,value
clr r3
lsl r2
rol r3
lsl r2
rol r3
lsl r2
rol r3

add ZL,r2
adc ZH,r3

lpdrawcharr0:
ldi temp,0b10000000
mov r1,temp
push XL
push XH


lpm r4,Z+


lpdrawcharr1:

mov temp,r4
and temp,r1
breq lpdrawcharnopoint
rcall TFT_DRAWPIXEL

lpdrawcharnopoint:
subi XL,-1
sbci XH,-1
lsr r1
brcc lpdrawcharr1
pop XH
pop XL
subi YL,-1
sbci YH,-1
dec r0
brne lpdrawcharr0
pop XL
pop XH
pop YL
pop YH
pop ZL
pop ZH
ret


TFT_DRAWSTRING: //X,Y pos,Z:string,vallowhigh color

lpdrawstring:
lpm value,Z+
and value,value
breq lpexitdrawstring
rcall TFT_DRAWCHAR
subi XL,-8
sbci XH,-1
rjmp lpdrawstring
lpexitdrawstring:
ret

TFT_DRAWHLINE: //X,Y pos, val0,val1 - len vallow high color

push val0
push val1

subi val0,1
add val0,XH
adc val1,XL

mov val2,YL
mov val3,YH

rcall TFT_SETADDRWINDOW

pop val1
pop val0


CS_ACTIVE
CD_COMMAND
ldi value,0x2C
rcall TFT_WRITE8

CD_DATA

lpdrawhline:


mov value,valhigh
rcall TFT_WRITE8
mov value,vallow
rcall TFT_WRITE8

subi val0,1
sbci val1,0
brne lpdrawhline

CS_IDLE
ret

TFT_DRAWVLINE: //X,Y pos, val2,val3 - length vallow high color

push val2
push val3

subi val2,1
add val2,YH
adc val3,YL


mov val0,XL
mov val1,XH


rcall TFT_SETADDRWINDOW

pop val1
pop val0


CS_ACTIVE
CD_COMMAND
ldi value,0x2C
rcall TFT_WRITE8

CD_DATA

lpdrawvline:


mov value,valhigh
rcall TFT_WRITE8
mov value,vallow
rcall TFT_WRITE8

subi val0,1
sbci val1,0
brne lpdrawvline

CS_IDLE
ret
