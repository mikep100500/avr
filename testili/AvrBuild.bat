@ECHO OFF
"C:\Program Files\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "C:\Users\user\Documents\avr\testili\labels.tmp" -fI -W+ie -C V2E -o "C:\Users\user\Documents\avr\testili\testili.hex" -d "C:\Users\user\Documents\avr\testili\testili.obj" -e "C:\Users\user\Documents\avr\testili\testili.eep" -m "C:\Users\user\Documents\avr\testili\testili.map" "C:\Users\user\Documents\avr\testili\testili.asm"