@ECHO OFF
"C:\Program Files\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "C:\Users\mike\Documents\avr\calc\labels.tmp" -fI -W+ie -C V2E -o "C:\Users\mike\Documents\avr\calc\calc.hex" -d "C:\Users\mike\Documents\avr\calc\calc.obj" -e "C:\Users\mike\Documents\avr\calc\calc.eep" -m "C:\Users\mike\Documents\avr\calc\calc.map" "C:\Users\mike\Documents\avr\calc\calc.asm"
