

# Driver for waveshare e-Paper Module

https://www.waveshare.com/wiki/1.54inch_e-Paper_Module
https://github.com/waveshare/e-Paper

# Hardware Connection

| e-Paper	| MSP-EXP430G2	| Function |
| -------	| ------------	| -------- 	|
|Vcc		| VCC			|3.3V 	|
|GND	| GND			|GND 	|
|DIN	| P1.7			|SPI MOSI|
|CLK	| P1.5			|SPI SCK 	|
|CS		| P1.4			|SPI CS 	|
|DC		| P2.5			|Digital Out |
|RST	| P2.4			|Digital Out |
|BUSY	| P2.3			|Digital In |

# mspdebug
## Start GDB
opt gdb_loop true
gdb

## Flash elf file
prog ~/Workspaces/MSP430LaunchPad/epaper/build/bin/app.elf
run

## Code Size 
   
|text | data | bss | dec | hex | languague  | optimization | 
|--|--|--|--|--|--|--|
|2800 | 4 | 26 | 2830 | b0e | C | -O0 | 
|1790 | 4 | 26 | 1820 | 71c | C | -O0 | 
|2442 | 144 | 12 | 2598 | a26| C++ V0 | -O1 |

