@ECHO OFF
"C:\Program Files\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "C:\Users\mike\Documents\avr\linex\labels.tmp" -fI -W+ie -C V2E -o "C:\Users\mike\Documents\avr\linex\linex.hex" -d "C:\Users\mike\Documents\avr\linex\linex.obj" -e "C:\Users\mike\Documents\avr\linex\linex.eep" -m "C:\Users\mike\Documents\avr\linex\linex.map" "C:\Users\mike\Documents\avr\linex\linex.asm"
