/*MOVEPMEM SOMEPOS,BOARD,BSIZE

ldi r16,4
ldi r17,15
 
rcall FINDWAY

andi r16,0xff
breq FINISH*/
;ldi valhigh,HIGH(CYAN)
;ldi vallow,LOW(CYAN)
;rcall TFT_FILLSCREEN
;ldi valhigh,HIGH(RED)
;ldi vallow,LOW(RED)
ldi XL,100
;clr XH
ldi YL,100
;clr YH
;rcall TFT_DRAWPIXEL

;clr XL
clr XH
;clr YL
clr YH
ldi valhigh,HIGH(RED)
ldi vallow,LOW(RED)
;ldi value,'Ï'
;rcall TFT_DRAWCHAR
ldi ZH,HIGH(FUCKOFF*2)
ldi ZL,LOW(FUCKOFF*2)
;rcall TFT_DRAWSTRING

ldi YL,100
clr YH
ldi valhigh,HIGH(CYAN)
ldi vallow,LOW(CYAN)

clr XL
clr XH

lds val0,x_size
lds val1,x_size+1

;rcall TFT_DRAWHLINE

ldi XL,50
clr XH
ldi valhigh,HIGH(BLUE)
ldi vallow,LOW(BLUE)

ldi YL,50
clr YH

ldi val2,100
ldi val3,0


;rcall TFT_DRAWVLINE
;rcall DRAWGRID



;ldi vallow,LOW(CYAN)
;ldi valhigh,HIGH(CYAN)
;ldi val0,7
;ldi val1,9
;rcall DRAWBALL
